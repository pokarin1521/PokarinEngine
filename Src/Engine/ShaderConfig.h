/**
* @file ShaderConfig.h
*/
#ifndef POKARINENGINE_SHADERCONFIG_H_INCLUDED
#define POKARINENGINE_SHADERCONFIG_H_INCLUDED

#include "glad/glad.h"

namespace PokarinEngine
{
	/// <summary>
	/// シェーダ関連の設定情報
	/// </summary>
	namespace ShaderConfig
	{
		// 使用可能なライトの最大数
		inline constexpr size_t lightMax = 16;

		/// <summary>
		/// テクスチャイメージユニットの番号
		/// </summary>
		namespace Texture
		{
			// 通常の色用
			inline constexpr GLuint color = 0;

			// 発光色用
			inline constexpr GLuint emission = 1;
		}

		/// <summary>
		/// シェーダのユニフォーム変数のロケーション番号
		/// </summary>
		namespace Uniform
		{
			// --------------------------------
			// 座標、回転角度、拡大率
			// --------------------------------

			// 座標変換行列
			inline constexpr GLint transformMatrix = 0;

			// 法線変換行列
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

			/* ライト構造体の要素は配列なので
			配列の数分ロケーション番号をずらす必要がある 
			
			配列のインデックス0番に合わせる */

			// ライトの色と減衰開始角度
			inline constexpr GLint lightColorAndFalloffAngle = 111;

			// ライトの座標と範囲(半径)
			inline constexpr GLint lightPositionAndRange =
				lightColorAndFalloffAngle + lightMax;

			// ライトの向きと最大照射角度
			inline constexpr GLint lightDirectionAndSpotAngle =
				lightColorAndFalloffAngle + lightMax * 2;

		} // UniformLocation
	}
}

#endif // !POKARINENGINE_SHADERCONFIG_H_INCLUDED
