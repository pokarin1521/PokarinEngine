/**
* @file LightData.cpp
*/
#include "LightManager.h"

#include "Settings/ShaderSettings.h"

#include <algorithm>

namespace PokarinEngine
{
	namespace LightManager
	{
		// ------------------
		// 変数
		// ------------------

		// ライトデータ配列
		LightDataList lights;

		// 使用中ライトのインデックス配列
		std::vector<int> usedLights;

		// 未使用ライトのインデックス配列
		std::vector<int> freeLights;

		// 一度に増やすライト数
		static constexpr size_t lightResizeCount = 100;

		// ---------------
		// 関数
		// ---------------

		/// <summary>
		/// ライト配列を初期化
		/// </summary>
		void InitializeLight()
		{
			// -----------------------------------------------
			// 指定された数のライト分の容量・要素数を確保
			// -----------------------------------------------

			// ライトデータの要素数を確保
			lights.resize(lightResizeCount);

			// 使用中のライトの容量を確保
			usedLights.reserve(lightResizeCount);

			// --------------------------------------
			// 全てのライトを未使用ライト配列に
			//「逆順」で追加
			// --------------------------------------

			/* ライトを「逆順」で追加するのは、
			新しいライトを取得するとき、
			ライト配列の末尾から取得させるため */

			// 未使用ライトの要素数を確保
			freeLights.resize(lightResizeCount);

			// 逆順に格納(初期化)していく
			for (int i = 0; i < lightResizeCount; ++i)
			{
				freeLights[i] = static_cast<int>(lightResizeCount - i - 1);
			}
		}

		/// <summary>
		/// ライト情報の更新
		/// カメラに近いライトを選んでGPUメモリにコピーする
		/// </summary>
		/// <param name="progStandard"> シェーダプログラム(標準)の管理番号 </param>
		/// <param name="camera"> カメラ </param>
		/// <param name="lights"> ライトデータ配列 </param>
		/// <param name="usedLights"> 使用中ライト </param>
		void Update(GLuint progStandard, const GameObject& mainCamera)
		{
			// -------------------------
			// 使用中ライトを確認
			// -------------------------

			// 使用中のライトがなければコピーするライト数を0に設定
			if (usedLights.empty())
			{
				glProgramUniform1i(progStandard, LocationNum::lightCount, 0);

				return;
			}

			// ------------------------------------
			// 使用中ライトの配列から、
			// 未使用になったライトを除外する
			// ------------------------------------

			// ----------- 使用中でないライトを   ------------
			// ----------- 配列の後ろに集めて削除 ------------

			// 使用中でないライトの先頭を取得
			const auto itrUnused = std::remove_if(
				usedLights.begin(), usedLights.end(),
				[&](int i) { return !lights[i].used; });

			// 使用中でないライトを削除
			usedLights.erase(itrUnused, usedLights.end());

			// ----------- 重複する番号を除去 -------------

			// 使用中ライトのインデックスを整理
			std::sort(usedLights.begin(), usedLights.end());

			// unique関数で重複する番号を後ろに集め、
			// erase関数で削除する
			auto itr = std::unique(usedLights.begin(), usedLights.end());
			usedLights.erase(itr, usedLights.end());

			// -------------------------------
			// カメラの正面ベクトルを計算
			// -------------------------------

			// カメラの正面ベクトル
			const Vec3 front = {
				-sin(mainCamera.transform->rotation.y),
				0,
				-cos(mainCamera.transform->rotation.y) };

			// -------------------------------------
			// カメラからライトまでの距離を計算
			// -------------------------------------

			// -------------- 下準備 ----------------

			// カメラからライトまでの距離
			struct Distance
			{
				// カメラからライトの半径までの距離
				float distance;

				// ライトのアドレス
				const LightData* p;
			};

			// ライトまでの距離を格納する配列
			std::vector<Distance> distanceList;

			// ライトの数だけ容量を確保
			distanceList.reserve(lights.size());

			// ----------- ライトまでの距離を配列に格納する ------------

			// 使用中ライトのインデックス
			for (auto index : usedLights)
			{
				// 使用中のライト
				const auto& light = lights[index];

				// カメラからライトまでのベクトル
				const Vec3 v = light.position - mainCamera.transform->position;

				// カメラの後ろで、ライトの範囲外だった場合
				if (Vec3_Function::Dot(front, v) <= -light.radius)
				{
					// 計算の必要がないので
					// 次のライトへ
					continue;
				}

				// カメラからのライトの半径までの距離
				const float d = Vec3_Function::Length(v) - light.radius;

				// 配列に追加
				distanceList.push_back({ d, &light });

			} // for indexs

			// ----------- 画面に影響するライトがなければ -------------
			// ----------- ライト数を0に設定する		  -------------

			// 画面内にライトがない
			if (distanceList.empty())
			{
				glProgramUniform1i(progStandard, LocationNum::lightCount, 0);
				return;
			}

			// -----------------------------------
			// カメラに近いライトを優先する
			// -----------------------------------

			// 距離が近い順に並べる
			std::stable_sort(distanceList.begin(), distanceList.end(),
				[&](const auto& a, const auto& b) {
					return a.distance < b.distance; });

			// -------------------------------------
			// ライトデータをGPUメモリにコピー
			// -------------------------------------

			// ------------ 下準備 ---------------

			// 使用するライトの数
			const int lightCount = static_cast<int>(
				std::min(distanceList.size(), MaxShaderCount::light));

			// ライトの色と減衰開始角度
			std::vector<Vec4> colorAndFalloffAngle(lightCount);

			// ライトの座標と半径
			std::vector<Vec4> positionAndRadius(lightCount);

			// ライト方向と最大照射角度
			std::vector<Vec4> directionAndConeAngle(lightCount);

			// -------- GPUにコピーするライトデータを設定する ---------

			for (int i = 0; i < lightCount; ++i)
			{
				// ライト
				const LightData* p = distanceList[i].p;

				// 明るさを反映した色
				const Vec3 color = p->color * p->intensity;

				// 色と減衰開始角度を設定
				colorAndFalloffAngle[i] = {
					color.x, color.y, color.z, p->falloffAngle };

				// 座標と半径を設定
				positionAndRadius[i] = {
					p->position.x, p->position.y, p->position.z, p->radius };

				// ライトの方向と最大照射角度を設定
				directionAndConeAngle[i] = {
					p->directoin.x, p->directoin.y, p->directoin.z, p->coneAngle };
			}

			// ----------- GPUにライトデータをコピー ------------

			// 色と減衰開始角度
			glProgramUniform4fv(progStandard, LocationNum::lightColorAndFalloffAngle,
				lightCount, &colorAndFalloffAngle[0].x);

			// 座標と半径
			glProgramUniform4fv(progStandard, LocationNum::lightPositionAndRadius,
				lightCount, &positionAndRadius[0].x);

			// 方向と最大照射角度
			glProgramUniform4fv(progStandard, LocationNum::lightDirectionAndConeAngle,
				lightCount, &directionAndConeAngle[0].x);

			// 使用するライトの数
			glProgramUniform1i(progStandard, LocationNum::lightCount, lightCount);
		}

		/// <summary>
		/// 新しいライトを取得する
		/// </summary>
		/// <returns> ライトのインデックス </returns>
		int AllocateLight()
		{
			// --------------------------------
			// 未使用のライトがなければ
			// ライト配列を拡張する
			// --------------------------------

			// 未使用のライトがない
			if (freeLights.empty())
			{
				// 元のサイズ
				const size_t oldSize = lights.size();

				// 追加する分、要素数を増やす
				lights.resize(oldSize + lightResizeCount);

				// 拡張したライトを未使用ライト配列に「逆順」で追加
				for (size_t i = lights.size() - 1; i >= oldSize; --i)
				{
					freeLights.push_back(static_cast<int>(i));
				}
			}

			// ----------------------------------
			// 未使用ライト配列の末尾から
			// インデックスを取り出す
			// ----------------------------------

			// これから使用するライトのインデックス
			const int index = freeLights.back();

			// 取り出した分、削除
			freeLights.pop_back();

			// 取り出したインデックスを使用中ライト配列に追加
			usedLights.push_back(index);

			// ライトの状態を「使用中」にする
			lights[index].used = true;

			// 取り出したインデックスを返す
			return index;
		}

		/// <summary>
		/// ライトを解放する
		/// </summary>
		/// <param name="index"> ライトのインデックス </param>
		void DeallocateLight(int index)
		{
			// 有効なインデックスであれば解放処理
			if (index >= 0 && index < lights.size())
			{
				// インデックスを未使用ライト配列に追加
				freeLights.push_back(index);

				// ライトの状態を「未使用」にする
				lights[index].used = false;
			}
		}

		/// <summary>
		/// インデックスに対応するライトデータを取得する
		/// </summary>
		/// <param name="index"> ライトのインデックス </param>
		/// <returns>
		/// <para> nullptr以外 : indexに対応するライトデータのアドレス </para>
		/// <para> nullptr : 対応するライトデータが存在しない </para>
		/// </returns>
		LightData* GetLight(int index)
		{
			// 有効なインデックスで、使用中のライト
			if (index >= 0 && index < lights.size() && lights[index].used)
			{
				// ライトデータを返す
				return &lights[index];
			}

			// 無効なインデックス、または未使用のライト
			return nullptr;
		}

	} // namespace LightData

} // namespace PokarinEngine