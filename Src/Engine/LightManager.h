/**
* @file LightManager.h
*/
#ifndef POKARINENGINE_LIGHTMANAGER_H_INCLUDED
#define POKARINENGINE_LIGHTMANAGER_H_INCLUDED

#include "Math/Vector.h"
#include "Math/Angle.h"

#include "Color.h"

#include <vector>

namespace PokarinEngine
{
	// --------------------
	// 前方宣言
	// --------------------

	class Camera;

	/// <summary>
	/// ライト管理用
	/// </summary>
	namespace LightManager
	{
		// -------------------------
		// ライト
		// -------------------------

		/// <summary>
		/// ライトの種類
		/// </summary>
		enum class LightType
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
			LightType type = LightType::pointLight;

			// ----------------- 
			// ライト全般 
			// -----------------

			// 色
			Color color = Color::white;	    
			
			// 明るさ
			float intensity = 5;		 
			
			// 位置
			Vector3 position = Vector3::zero;
			
			// ライトが届く最大範囲(半径)
			float range = 10;			    

			// ---------------------
			// スポットライト 
			// ---------------------

			// ライトの向き
			Vector3 direction = Vector3::zero;

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
		/// <param name="[in] camera"> 使用するカメラ </param>
		void CopyGPU(const Camera& camera);

		/// <summary>
		/// ライトデータを追加する
		/// </summary>
		/// <param name="[in] lightData"> 追加するライトデータ </param>
		void AddLightData(const LightData& lightData);

		/// <summary>
		/// ライトデータを削除する
		/// </summary>
		/// <param name="[in] lightData"> 削除するライトデータ </param>
		void EraseLightData(const LightData& lightData);

	} // namespace LightData

} // namespace PokarinEngine

#endif // !POKARINENGINE_LIGHTMANAGER_H_INCLUDED
