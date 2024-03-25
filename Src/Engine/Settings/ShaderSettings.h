/**
* @file ShaderSettings.h
*/
#ifndef SHADERSETTINGS_H_INCLUDED
#define SHADERSETTINGS_H_INCLUDED

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
	/// シェーダのロケーション番号
	/// </summary>
	namespace LocationNum
	{
		// ------------------------
		// オブジェクト, カメラ
		// ------------------------

		// オブジェクトの拡大率
		inline constexpr GLint scale = 0;

		// オブジェクトの座標
		inline constexpr GLint position = 1;

		// オブジェクトのX,Y軸回転
		inline constexpr GLint sinCosXY = 2;

		// アスペクト比と視野角による拡大率
		inline constexpr GLint aspectRatioAndScaleFov = 3;

		// カメラの座標
		inline constexpr GLint cameraPosition = 4;

		// カメラのY軸回転
		inline constexpr GLint cameraRotation = 5;

		// オブジェクトの色
		inline constexpr GLint color = 100;

		// オブジェクトの発光色
		inline constexpr GLint emissionColor = 101;

		// ---------------
		// ライト
		// ---------------

		// 使用するライトの数
		inline constexpr GLint lightCount = 110;

		// ライトの色
		// 配列のインデックス0番
		inline constexpr GLint lightColorAndFalloffAngle = 111;

		// ライトの座標と範囲の半径
		// 配列のインデックス0番
		inline constexpr GLint lightPositionAndRadius =
			lightColorAndFalloffAngle + MaxShaderCount::light;

		// ライト
		inline constexpr GLint lightDirectionAndConeAngle =
			lightColorAndFalloffAngle + MaxShaderCount::light * 2;

	} // LocationNum

} // namespace PokarinEngine

#endif // !SHADERSETTINGS_H_INCLUDED
