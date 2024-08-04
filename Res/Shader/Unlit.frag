/**
* @file Unlit.frag
*/
#version 450

// -----------------------
// シェーダへの入力
// -----------------------

layout(location = 0) in vec3 inPosition; // ワールド座標
layout(location = 1) in vec2 inTexcoord; // テクスチャ座標

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

// オブジェクトの色
layout(location = 100) uniform vec4 color;		

// 物体の発光色と
// エミッションテクスチャの識別番号
layout(location = 101) uniform vec4 emission;

// ------------------------
// シェーダからの出力
// ------------------------

// 出力する色データ
out vec4 outColor;

void main()
{
	// -------------------
	// テクスチャの設定
	// -------------------

	// サプライ変数が参照するユニットに割り当てられたテクスチャから、
	// テクスチャ座標のピクセル値を読み取って返す
	vec4 c = texture(texColor, inTexcoord);

	// CRTモニターのガンマ値
	const float crtGamma = 2.2; 

	// ほとんどの画像は、正しい明るさで出力されるように、
	// 1/2.2乗の明るさで作成されているので、ガンマ値で修正
	c.rgb = pow(c.rgb, vec3(crtGamma)); 

	// ガンマ値で修正した色を出力
	outColor = c * color;

	// ------------- 発光色を反映 --------------
	
	// w要素に識別番号が入っているため、
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
	outColor.rgb = pow(outColor.rgb, vec3(1 / crtGamma));
}
