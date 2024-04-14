/**
* @file LightData.h
*/
#ifndef LIGHTDATA_H_INCLUDED
#define LIGHTDATA_H_INCLUDED

#include "Math/Vector.h"
#include "Math/Angle.h"
#include "GameObject.h"

#include <GLFW/glfw3.h>
#include <vector>

namespace PokarinEngine
{
	/// <summary>
	/// ライトのパラメータ管理用
	/// </summary>
	namespace LightParameter
	{
		// -------------------------
		// ライト
		// -------------------------

		/// <summary>
		/// ライトの種類
		/// </summary>
		enum class Type
		{
			pointLight,	 // 点光源
			spotLight,	 // スポット光源
			directional, // 平行光源
		};

		/// <summary>
		/// ライトの情報
		/// </summary>
		struct LightData
		{
			// ------------------
			// ライトの種類
			// ------------------

			// ライトの種類
			Type type = Type::pointLight;

			// ----------------- 
			// ライト全般 
			// -----------------

			Color color = { 1,1,1,1 };	    // 色
			float intensity = 5;		    // 明るさ
			Vector3 position = { 0,0,0 };	// 位置
			float range = 10;			    // ライトが届く最大範囲(半径)

			// ---------------------
			// スポットライト 
			// ---------------------

			// ライトの向き
			Vector3 direction = { 0,0,0 };

			// スポットライトの最大照射角度
			// 0以下なら点光源として扱う
			float spotAngle = Radians(30);

			// スポットライトの減衰開始角度
			float falloffAngle = Radians(20);
		};

		// ------------------
		// 関数
		// ------------------

		/// <summary>
		/// ライト情報をGPUにコピーする
		/// </summary>
		/// <param name="mainCamera"> メインカメラ </param>
		void CopyGPU(const GameObject& mainCamera);

		/// <summary>
		/// ライトデータを追加する
		/// </summary>
		/// <param name="lightData"> 追加するライトデータ </param>
		void AddLightData(LightData& lightData);

		/// <summary>
		/// ライトデータを削除する
		/// </summary>
		/// <param name="lightData"> 削除するライトデータ </param>
		void EraseLightData(LightData& lightData);

	} // namespace LightData

} // namespace PokarinEngine

#endif // !LIGHTDATA_H_INCLUDED
