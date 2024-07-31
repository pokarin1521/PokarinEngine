/**
* @file ShaderConfig.h
*/
#ifndef POKARINENGINE_SHADERCONFIG_H_INCLUDED
#define POKARINENGINE_SHADERCONFIG_H_INCLUDED

#include "glad/glad.h"

namespace PokarinEngine
{
	/// <summary>
	/// シェーダで使える個数
	/// </summary>
	namespace MaxShaderCount
	{
		// ライトの数
		inline constexpr size_t light = 16;
	}

	/// <summary>
	/// テクスチャイメージユニットの番号
	/// </summary>
	namespace TextureBinding
	{
		// 通常の色用
		inline constexpr GLuint color = 0;

		// 発光色用
		inline constexpr GLuint emission = 1;
	}

	/// <summary>
	/// シェーダのユニフォーム変数のロケーション番号
	/// </summary>
	namespace UniformLocation
	{
		// --------------------------------
		// 座標、回転角度、拡大率
		// --------------------------------

		// 座標変換ベクトルの行列
		inline constexpr GLint transformMatrix = 0;

		// 法線変換ベクトルの行列
		inline constexpr GLint normalMatrix = 1;

		// アスペクト比と視野角による拡大率
		inline constexpr GLint aspectRatioAndScaleFov = 3;

		// カメラの座標
		inline constexpr GLint cameraPosition = 4;

		// カメラの回転角度
		inline constexpr GLint cameraRotation = 5;

		// -------------------------
		// オブジェクト色
		// -------------------------

		// オブジェクトの色
		inline constexpr GLint color = 100;

		// オブジェクトの発光色
		inline constexpr GLint emissionColor = 101;

		// -------------------
		// ライト
		// -------------------

		// 環境光
		// 方向を持たないので色だけ
		inline constexpr GLint ambientLight = 107;

		// 平行光源の色
		inline constexpr GLint directionalLightColor = 108;

		// 平行光源の向き
		inline constexpr GLint directionalLightDirection = 109;

		// 使用するライトの数
		inline constexpr GLint lightCount = 110;

		// ライトの色
		// 配列のインデックス0番
		inline constexpr GLint lightColorAndFalloffAngle = 111;

		/* ライト構造体の要素は配列なので
		配列の数分ロケーション番号をずらす必要がある */

		// ライトの座標と範囲の半径
		// 配列のインデックス0番
		inline constexpr GLint lightPositionAndRange =
			lightColorAndFalloffAngle + MaxShaderCount::light;

		// ライト
		inline constexpr GLint lightDirectionAndSpotAngle =
			lightColorAndFalloffAngle + MaxShaderCount::light * 2;

	} // UniformLocation

} // namespace PokarinEngine

#endif // !POKARINENGINE_SHADERCONFIG_H_INCLUDED
