/**
* @file Standard.vert
*/
#version 450

// ---------------------
// シェーダへの入力
// ---------------------

layout(location = 0) in vec3 inPosition; //	頂点座標
layout(location = 1) in vec2 inTexcoord; // テクスチャ座標
layout(location = 2) in vec3 inNormal;   // 法線ベクトル

// -----------------------
// シェーダからの出力
// -----------------------

layout(location = 0) out vec3 outPosition; // ワールド座標
layout(location = 1) out vec2 outTexcoord; // テクスチャ座標
layout(location = 2) out vec3 outNormal; // 法線ベクトル

// ---------------------------
// プログラムからの入力
// ---------------------------

/* プログラムからシェーダにデータを送るにはユニフォーム変数を使う
ロケーション番号はデータの送り先番号
イン変数、アウト変数とは別の場所に作られるため重複可 */

// 座標変換ベクトルの行列
layout(location = 0) uniform mat4 transformMatrix;

// 法線変換ベクトルの行列
layout(location = 1) uniform mat3 normalMatrix;

// アスペクト比と視野角による拡大率
layout(location = 3) uniform vec2 aspectRatioAndScaleFov;

// カメラの位置
layout(location = 4) uniform vec3 cameraPosition;

// カメラの回転
layout(location = 5) uniform vec3 cameraRotation; 

/**
* ベクトルをZ軸->X軸->Y軸の順で回転させる
* 
* @param v				回転させるベクトル
* @param sinRotation	各軸のSin
* @param cosRotation    各軸のCos
* 
* @return ベクトルvを、Z軸->X軸->Y軸の順番で回転させたベクトル
*/
vec3 RotateZXY(vec3 v, vec3 sinRotation, vec3 cosRotation)
{
	/* 回転の順序は重要で、同じ角度を指定しても、
	回転させる順序が異なると全く違う向きになってしまう 
	そのため、毎回同じ順序で回転させることが重要
	
	順序は「姿勢制御のしやすさ」を考えて選ぶ
	一般的に「Z->X->Y」の順が扱いやすいとされる 
	
	3Dモデルを作成したときの向きや、
	ワールド座標系の軸の定義によっては、別の順序の方がいい場合もある */

	// -------------------------------
	// Y->X->Zの順で回転を計算する
	// -------------------------------

	/* カメラとオブジェクトの回転方向が逆になるので、
	ZXYの順で回転させたいときは、カメラはYXZの順で回転させないといけない */

	// ---------- Y軸回転 -----------

	v.xz = vec2(
		v.x * cosRotation.y + v.z * sinRotation.y,
		-v.x * sinRotation.y + v.z * cosRotation.y);

	// ---------- X軸回転 -----------

	v.yz = vec2(
		v.z * -sinRotation.x + v.y * cosRotation.x,
		v.z * cosRotation.x + v.y * sinRotation.x); 

	// ---------- Z軸回転 -----------
	
	v.xy = vec2(
		v.x * cosRotation.z + v.y * sinRotation.z,
		v.x * -sinRotation.z + v.y * cosRotation.z);

	return v;
}

void main()
{
	// テクスチャ座標をフラグメントシェーダに渡す
	outTexcoord = inTexcoord;

	// ------------------------------------------
	// ローカル座標系からワールド座標系に変換
	// ------------------------------------------

	// 拡大率、X軸Y軸の回転、平行移動を反映
	// 計算をまとめた結果、ベクトル同士の乗算で座標変換できるようになった
	// ベクトルの配列を行列型で受け取ってるので、行列 * ベクトルで計算
	gl_Position = transformMatrix * vec4(inPosition, 1);

	// -------- フラグメントシェーダにワールド座標を渡す -------
	
	outPosition = gl_Position.xyz;

	// ------------ ワールド座標系の法線を計算 ---------------

	// 拡大率、X軸Y軸の回転を反映
	// 計算をまとめた結果、ベクトル同士の乗算で座標変換できるようになった
	// ベクトルの配列を行列型で受け取ってるので、行列 * ベクトルで計算
	outNormal = normalMatrix * inNormal;

	// ------------------------------------------------------
	// ワールド座標系からビュー座標系に変換(カメラ座標系)
	// ------------------------------------------------------

	/* カメラが回転する = カメラを中心にオブジェクトが回転する
	ということなので、カメラを原点としてオブジェクトを回転させる */

	// カメラからの相対座標
	vec3 pos = gl_Position.xyz - cameraPosition;

	// カメラの回転を反映
	pos = RotateZXY(pos, sin(cameraRotation), cos(cameraRotation));

	// カメラの回転を反映した座標を代入
	gl_Position.xyz = pos;

	// ---------------------------------------------
	// ビュー座標系からクリップ座標系に変換
	// ---------------------------------------------

	/* クリップ座標系は、
	画面に映らない部分を除去する処理(クリッピング)のための座標系
	
	頂点シェーダ(テッセレータやジオメトリシェーダも使う場合、
	それらの中で最後に実行されるシェーダ)から出力される座標は、
	この座標系で定義されていなければならない */

	// ------------ x座標をアスペクト比で割り --------------
	// ------------ 縦横の比率を1:1にする	  --------------
	
	// CPU側で 1 / アスペクト比 をしているので乗算
	const float aspectRatio = aspectRatioAndScaleFov.x;
	gl_Position.x *= aspectRatio;

	// --------- x,y座標それぞれを視野角による拡大率で割り -----------
	// --------- 表示する位置を調整(視野角を反映)		   -----------

	// CUP側で 1 / 視野角による拡大率 をしているので乗算
	const float scaleFov = aspectRatioAndScaleFov.y;
	gl_Position.xy *= scaleFov;

	// -------- 深度値の計算結果が-1～+1になるような ------------
	// -------- パラメータA,Bを計算					 ------------

	/* OpenGLの深度バッファは、最小値(手前)が-1、最大値(奥)が+1となる
	そこで、深度値がnearのとき-1、farのとき+1となるような計算式を求める
	(gl_Position.z / gl_Position.w の結果が-1 ～ +1になるようにする)
	
	定数A,Bがあるとして、次の条件を満たす式を求める
	
	-1 <= (A * 深度値 + B) <= +1 
	
	しかし、実際には深度値ではなく、
	gl_Position.wで除算したものが計算結果になる
	wは深度値そのものなので、正しい式は次のようになる
	
	-1 <= (A / 深度値 + B) <= +1
	
	深度値がnearのとき-1、farのとき+1になるのだから
	
	A / near + B = -1
	A / far + B = 1  
	
	を満たすAとBを求めればよいことになる
	計算すると
	
	A = 2 * far * near / (near - far)
	B = (far + near) / (far - near) 
	
	最後に分母を合わせるために、Aの式の分子分母に-1を掛ける
	
	A = -2 * far * near / -(near - far)
	A = -2 * far * near / (far - near)
	
	これでAとBを求めることができた 
	
	A = -2 * far * near / (near - far)
	B = (far + near) / (far - near) */
	
	// 最小描画範囲
	// オブジェクトの中身が見えないように
	// カメラの衝突判定の半径の「cos45」倍にする
	const float near = 0.35f; 
	
	// 最大描画範囲
	const float far = 1000;

	// -1 <= A * 深度値 + B <= 1 となる
	// 定数A,Bを求める
	const float A = -2 * far * near / (far - near);
	const float B = (far + near) / (far - near);
	
	// ------------ 遠近法を有効にする -------------

	/* GPUがgl_Positoin /= gl_Position.w 
	(クリップ座標系をNDC座標系に変換)をするので、
	wに距離(z)を代入し、距離が遠いほどxyの値が小さくなるようにする

	OpenGLは右手座標系で、GPUは左手座標系なので符号を反転させる */
	gl_Position.w = -gl_Position.z;
	
	/* 求めた定数A,Bを使って、深度値を補正する
	
	gl_Position.z / gl_Position.w = A / gl_Position.w + B
	
	が最終的な結果なので、ここからzの値を求める */

	gl_Position.z = -gl_Position.z * B + A;
}