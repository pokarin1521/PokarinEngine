/**
* @file Standard.frag
*/
#version 450

// -----------------------
// シェーダへの入力
// -----------------------

layout(location = 0) in vec3 inPosition; // ワールド座標
layout(location = 1) in vec2 inTexcoord; // テクスチャ座標
layout(location = 2) in vec3 inNormal;   // 法線ベクトル

// -------------------------
// テクスチャサンプラ
// -------------------------

// テクスチャイメージユニットに割り当てられたテクスチャを使うには
// sampler型のユニフォーム変数を使う
// ユニット番号はbinding識別子で指定

// テクスチャ
layout(binding = 0) uniform sampler2D texColor;

// エミッションテクスチャ
layout(binding = 1) uniform sampler2D texEmission;

// ----------------------------
// プログラムからの入力
// ----------------------------

/* ユニフォーム変数のロケーション番号は全ステージ共有なので
頂点シェーダと重複しないように100から指定する */

// オブジェクトの色
layout(location = 100) uniform vec4 color;		

// 物体の発光色と
// エミッションテクスチャの管理番号
layout(location = 101) uniform vec4 emission;

// 環境光

// 平行光源
struct DirectionalLight
{
	vec3 color;		// 色と明るさ
	vec3 direction; // 光の向き
};

// 平行光源
layout(location = 108) uniform DirectionalLight directionalLight;


// ライト
// 点光源とスポットライトに対応
struct Light
{
	// 明るさを反映した色, 減衰開始角度
	vec4 colorAndFalloffAngle[16];	
	
	// 座標と範囲(半径)
	vec4 positionAndRange[16];		
	
	// 向き, 最大照射角度
	vec4 directionAndSpotAngle[16];
};

// ライトの数
layout(location = 110) uniform int lightCount;

// ユニフォーム変数に構造体や配列を指定すると、
// メンバまたは配列要素ごとにロケーション番号が割り振られる
// (今回は、color = 111番, positionAndRange = 112番)

// ライト(ポイント・スポット)
layout(location = 111) uniform Light light;

// 出力する色データ
out vec4 outColor;

void main()
{
	// -------------------------
	// テクスチャの設定
	// -------------------------

	// サプライ変数が参照するユニットに割り当てられたテクスチャから、
	// テクスチャ座標のピクセル値を読み取って返す
	vec4 c = texture(texColor, inTexcoord);

	// ほとんどの画像は、正しい明るさで出力されるように、
	// 1/2.2乗の明るさで作成されているので、ガンマ値で修正
	const float crtGamma = 2.2; // CRTモニターのガンマ値
	c.rgb = pow(c.rgb, vec3(crtGamma)); 

	outColor = c * color;

	// -----------------------------
	// 法線の正規化
	// -----------------------------

	/* 頂点シェーダの出力変数の値は、
	そのままフラグメントシェーダにコピーされるわけではない

	頂点シェーダの計算結果は「頂点座標における値」
	しかし、フラグメントシェーダは
	「頂点間にある全てのピクセル」に対して実行される 
	
	そこで、「ピクセルから頂点までの距離に応じて頂点の値を混合」することで、
	中間領域にあるピクセルの値を作り出す
	この操作は「線形補間」という方法で行われる

	例えば、頂点シェーダがoutColorに頂点の色を書き込み、
	フラグメントシェーダがinColorで受け取る場合

	頂点Aが赤(1, 0, 0)、頂点Bが青(0, 0, 1)をoutColorに代入したとき、
	AとBの中間にあるピクセルのinColorには(0.5, 0, 0.5)という値が代入される */

	// 線形補間によって長さが1ではなくなっているので、
	// 正規化して長さを1に復元する
	// (内積でcosθを求めるため)
	vec3 normal = normalize(inNormal);

	// ----------------------------------------
	// 光の明るさを計算(ランバート反射)
	// ----------------------------------------

	/* 現実世界の光を再現するには膨大な計算が必要で時間がかかり、
	ゲームのようなリアルタイム性を求められる分野には使えない
	なので、「簡単な計算でそれっぽい見た目になる方法」を使う 
	
	今回は、ランバート反射を使う
	物体が反射する光は「拡散反射」と「鏡面反射」に大別される
	ランバート反射は「拡散反射」を再現する
	
	ランバート反射は
	「入射光は物体の表面で全方位に均等に反射する」と仮定している
	(表面が非常に荒い物体の反射) */
	
	// 拡散光の明るさの合計
	vec3 diffuse = vec3(0);

	// ライトの計算
	for(int i = 0; i < lightCount; ++i)
	{
		// ----------------------------
		// 計算に必要な値を求める 
		// ----------------------------

		// ポイントライトの座標
		vec3 lightPosition = light.positionAndRange[i].xyz;

		// フラグメントから見たポイントライトの方向
		vec3 direction = lightPosition - inPosition;

		// 光源までの距離
		float sqrDistance = dot(direction, direction);
		float distance = sqrt(sqrDistance);

		// 方向を正規化して長さを1にする
		// (内積でcosθを求めるため)
		direction = normalize(direction);

		// ----------------------------------------------
		// ランベルトの余弦則を使って明るさを計算 
		// ----------------------------------------------
		
		/* ランバート反射の計算に「ランベルトの余弦則」という法則を利用する
		
		----- ランベルトの余弦則 -----
		物体のある点で反射した光(反射光)の強さは、
		その点の法線と光源方向のなす角θの余弦(cos)と正比例する */

		/* なす角の範囲は0 ～ 90なので、
		max関数でcosθが0以上になるようにしている
		(ベクトルを正規化しているので、dotの結果はcosθになる) */

		// 光源の方向ベクトルと法線ベクトルのなす角
		float theta = max(dot(direction, normal), 0);

		/* 拡散反射では入射光が「全方位に均等に反射」される
		このとき、ある方向に反射する光の量は入射光の「1/π倍」になる
		そのため、正しい明るさを得るには内積の結果をπで割る必要がある */

		// 照度
		float illnuminance = theta / 3.14159265;

		// ----------- スポットライトかどうか確認  -------------

		// 最大照射角度
		const float spotAngle = light.directionAndSpotAngle[i].w;

		// 照射角度が0より大きければスポットライトとみなす
		if(spotAngle > 0)
		{
			// スポットライトの向き
			vec3 lightDiretion = light.directionAndSpotAngle[i].xyz;

			//「ライトからフラグメントへ向かうベクトル」と
			//「スポットライトの向きベクトル」のなす角を計算
			float angle = acos(dot(-direction, lightDiretion));

			// 角度がspotAngle以上なら範囲外
			if(angle >= spotAngle)
			{
				// 範囲外なら計算の必要はない
				continue;
			}

			// 減衰開始角度
			const float falloffAngle = light.colorAndFalloffAngle[i].w;

			// 最大照射角度のとき0, 
			// 減衰開始角度のとき1になるように補間
			const float a = min(
				(spotAngle - angle) / (spotAngle - falloffAngle), 1);

			illnuminance *= a;

		} // if spotAngle

		// --------- ライトの最大距離を制限				   ---------
		// --------- 計算はUE4などで採用されている式を使う ---------

		/* 
				  saturate(1 - (distance / lightRadius)^4)^2
		fallof = ----------------------------------------------
								distance^2 + 1                    
		*/
	
		// ライトの範囲(半径)
		const float range = light.positionAndRange[i].w;

		// clamp関数で、0 ～ 1の範囲になるようにする
		const float smoothFactor = clamp(1 - pow(distance / range, 4), 0, 1);

		// ライトの最大距離を制限
		illnuminance *= smoothFactor * smoothFactor;

		// 逆2乗の法則によって明るさを減衰させる
		// 0除算が起きないように1を足している
		illnuminance /= sqrDistance + 1;

		// 明るさを反映した色
		vec3 lightColor = light.colorAndFalloffAngle[i].xyz;

		// 拡散光の明るさを加算(照度 * ライトの明るさ)
		diffuse += lightColor * illnuminance;

	} // for lightCount

	// --------- 平行光源の明るさを計算 ---------

	/* 平行光源には「光源の座標(発射点)」がないので、
	光の向きには「平行光源の向き」をそのまま使う
	
	明るさは「法線と向きの内積」で求める */

	/* なす角の範囲は0 ～ 90なので、
	max関数でcosθが0以上になるようにしている
	(ベクトルを正規化しているので、dotの結果はcosθになる) */

	// 平行光源の方向ベクトルと法線ベクトルのなす角
	// 「フラグメントから見た光の向き」が必要なので向きを逆にする
	float theta = max(dot(-directionalLight.direction, normal), 0);

	/* 拡散反射では入射光が「全方位に均等に反射」される
	このとき、ある方向に反射する光の量は入射光の「1/π倍」になる
	そのため、正しい明るさを得るには内積の結果をπで割る必要がある */

	// 照度
	float illnuminance = theta / 3.14159265;

	// 拡散光の明るさを加算
	diffuse += directionalLight.color * illnuminance;

	// ---------- 拡散光の影響を反映 ------------

	outColor.rgb *= diffuse;

	// ------------- 発光色を反映 --------------
	
	// w要素に管理番号が入っているため、
	// w要素が0より大きい場合、
	//「エミッションテクスチャあり」とみなす
	if(emission.w > 0)
	{
		// テクスチャを反映する
		outColor.rgb += texture(texEmission, inTexcoord).rgb * emission.rgb;
	}
	// エミッションテクスチャがない場合
	else
	{
		// 発光色のみ反映する
		outColor.rgb += emission.rgb;
	}

	// ガンマ補正を行う
	// 画面に表示される明るさは入力値の2.2乗になるという
	// ブラウン管(CRT)モニターの特性がある
	// なので、ライトの明るさを1/2.2乗することで、明るさを修正する
	outColor.rgb = pow(outColor.rgb, vec3(1 / 2.2));
}
