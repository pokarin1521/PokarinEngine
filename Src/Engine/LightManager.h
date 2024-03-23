/**
* @file LightData.h
*/
#ifndef LIGHTDATA_H_INCLUDED
#define LIGHTDATA_H_INCLUDED

#include "VecMath.h"
#include "GameObject.h"

#include <GLFW/glfw3.h>
#include <vector>

namespace PokarinEngine
{
	namespace LightManager
	{
		// ----------------------
		// 前方宣言
		// ----------------------

		struct LightData;

		// ---------------------------
		// 型の別名を定義
		// ---------------------------

		// ライトデータ配列
		using LightDataList = std::vector<LightData>;

		// ----------------
		// 構造体
		// ----------------

		/// <summary>
		/// ライトの情報
		/// </summary>
		struct LightData
		{
			// ----------------- 
			// ライト全般 
			// -----------------

			Vec3 color = { 0,0,0 };		// 色
			float intensity = 0;		// 明るさ
			Vec3 position = { 0,0,0 };	// 位置
			float radius = 0;			// ライトが届く最大半径
			bool used = false;			// 使用中かどうか

			// --------------------- 
			// スポットライト 
			// ---------------------

			// ライトの向き
			Vec3 directoin = { 0,0,0 };

			// スポットライトの最大照射角度
			// 0以下なら点光源として扱う
			float coneAngle = 0;

			// スポットライトの減衰開始角度
			float falloffAngle = 0;
		};

		// ------------------
		// 関数
		// ------------------

		/// <summary>
		/// ライト配列を初期化
		/// </summary>
		void InitializeLight();

		/// <summary>
		/// ライト情報の更新
		/// カメラに近いライトを選んでGPUメモリにコピーする
		/// </summary>
		/// <param name="progStandard"> シェーダプログラム(標準)の管理番号 </param>
		/// <param name="camera"> カメラ </param>
		/// <param name="lights"> ライトデータ配列 </param>
		/// <param name="usedLights"> 使用中ライト </param>
		void Update(GLuint progStandard, const GameObject& mainCamera);

		/// <summary>
		/// 新しいライトを取得する
		/// </summary>
		/// <returns> ライトのインデックス </returns>
		int AllocateLight();

		/// <summary>
		/// ライトを解放する
		/// </summary>
		/// <param name="index"> ライトのインデックス </param>
		void DeallocateLight(int index);

		/// <summary>
		/// インデックスに対応するライトデータを取得する
		/// </summary>
		/// <param name="index"> ライトのインデックス </param>
		/// <returns>
		/// <para> nullptr以外 : indexに対応するライトデータのアドレス </para>
		/// <para> nullptr : 対応するライトデータが存在しない </para>
		/// </returns>
		LightData* GetLight(int index);

	} // namespace LightData

} // namespace PokarinEngine

#endif // !LIGHTDATA_H_INCLUDED
