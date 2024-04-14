/**
* @file LightData.cpp
*/
#include "LightParameter.h"

#include "Settings/ShaderSettings.h"
#include "Shader/Shader.h"

#include <algorithm>
#include <set>

namespace PokarinEngine
{
	/// <summary>
	/// ライトのパラメータ管理用
	/// </summary>
	namespace LightParameter
	{
		// ---------------------------
		// 型の別名を定義
		// ---------------------------

		using LightDataList = std::set<LightData*>;

		// 昇順になるようにset型を使う
		using LightIndexList = std::set<int>;

		// ----------------------------
		// 変数
		// ----------------------------

		// ライトデータ配列
		LightDataList lightList;

		// 平行光源
		LightData* directionalLight = nullptr;

		// 環境光(青空をイメージ)
		// 方向を持たないので色だけ
		Color ambientLight = { 0.05f, 0.15f, 0.25f, 1.0f };

		// -----------------------------
		// 関数
		// -----------------------------

		/// <summary>
		/// ライト情報をGPUにコピーする
		/// </summary>
		/// <param name="mainCamera"> カメラ </param>
		void CopyGPU(const GameObject& mainCamera)
		{
			// ------------------------------------
			// 環境光をGPUメモリーにコピー
			// ------------------------------------

			// シェーダプログラム(標準)の管理番号
			const GLuint progStandard = Shader::GetProgram(Shader::ProgType::Standard);

			// -------------------------
			// 使用中ライトを確認
			// -------------------------

			// 使用中のライトがなければコピーするライト数を0に設定
			if (lightList.empty())
			{
				glProgramUniform1i(progStandard, UniformLocation::lightCount, 0);

				return;
			}

			// -------------------------------
			// カメラの正面ベクトルを計算
			// -------------------------------

			// カメラの正面ベクトル
			const Vector3 front = {
				-sin(mainCamera.transform->rotation.y),
				0,
				-cos(mainCamera.transform->rotation.y) };

			// -----------------------------------------
			// カメラからライトまでの距離を計算
			// -----------------------------------------

			// -------------- 下準備 ----------------

			// カメラからライトまでの距離を管理する構造体
			struct Distance
			{
				// カメラからライトまでの距離
				float distance = 0;

				// ライトデータ
				const LightData* lightData = nullptr;
			};

			// カメラからライトまでの距離を格納する配列
			std::vector<Distance> distanceList;

			// ライトの数だけ容量を確保する
			distanceList.reserve(lightList.size());

			// ----------- ライトまでの距離を配列に格納する ------------

			// 使用中ライトの要素番号
			for (auto& light : lightList)
			{
				// 平行光源は距離関係なく使うので別の変数で保持
				if (light->type == Type::directional)
				{
					directionalLight = light;
					continue;
				}

				// カメラからライトまでのベクトル
				const Vector3 v = light->position - mainCamera.transform->position;

				// カメラの後ろで、ライトの範囲外だった場合
				if (front.Dot(v) <= -light->range)
				{
					// 計算の必要がないので
					// 次のライトへ
					continue;
				}

				// カメラからのライトの半径までの距離
				const float distance = v.Length() - light->range;

				// 配列に追加
				distanceList.push_back({ distance, light });

			} // for indexs

			// ------------ 平行光源のデータをGPUにコピー -------------

			if (directionalLight)
			{
				// 色
				const Color color = directionalLight->color * directionalLight->intensity;

				// GPUにコピー
				glProgramUniform3fv(progStandard,
					UniformLocation::directionalLightColor, 1, &color.r);

				glProgramUniform3fv(progStandard,
					UniformLocation::directionalLightDirection, 1, &directionalLight->direction.x);
			}


			glProgramUniform3fv(progStandard,
				UniformLocation::ambientLight, 1, &ambientLight.r);

			// ----------- 画面に影響するライトがなければ -------------
			// ----------- ライト数を0に設定する		  -------------

			// 画面内にライトがない
			if (distanceList.empty())
			{
				glProgramUniform1i(progStandard, UniformLocation::lightCount, 0);
				return;
			}

			// -------------------------------------
			// カメラに近いライトを優先する
			// -------------------------------------

			// カメラからの距離が短い順にする
			std::stable_sort(distanceList.begin(), distanceList.end(),
				[](const auto& a, const auto& b) { return a.distance < b.distance; });

			// -------------------------------------
			// ライトデータをGPUメモリにコピー
			// -------------------------------------

			// ------------ 下準備 ---------------

			// 使用するライトの数
			const int lightCount = static_cast<int>(
				std::min(distanceList.size(), MaxShaderCount::light));

			// ライトの色と減衰開始角度
			std::vector<Vector4> colorAndFalloffAngle(lightCount);

			// ライトの座標と範囲(半径)
			std::vector<Vector4> positionAndRange(lightCount);

			// ライト方向と最大照射角度
			std::vector<Vector4> directionAndSpotAngle(lightCount);

			// -------- GPUにコピーするライトデータを設定する ---------

			for (int i = 0; i < lightCount; ++i)
			{
				// ライト
				const LightData* lightData = distanceList[i].lightData;

				// 明るさを反映した色
				const Color color = lightData->color * lightData->intensity;

				// 色と減衰開始角度を設定
				colorAndFalloffAngle[i] = {
					color.r, color.g, color.b, lightData->falloffAngle };

				// 座標と範囲(半径)を設定
				positionAndRange[i] = {
					lightData->position.x, lightData->position.y, lightData->position.z, lightData->range };

				// ライトの方向と最大照射角度を設定
				directionAndSpotAngle[i] = {
					lightData->direction.x, lightData->direction.y, lightData->direction.z, lightData->spotAngle };
			}

			// ----------- GPUにライトデータをコピー ------------

			// 色と減衰開始角度
			glProgramUniform4fv(progStandard, UniformLocation::lightColorAndFalloffAngle,
				lightCount, &colorAndFalloffAngle[0].x);

			// 座標と範囲(半径)
			glProgramUniform4fv(progStandard, UniformLocation::lightPositionAndRange,
				lightCount, &positionAndRange[0].x);

			// 方向と最大照射角度
			glProgramUniform4fv(progStandard, UniformLocation::lightDirectionAndSpotAngle,
				lightCount, &directionAndSpotAngle[0].x);

			// 使用するライトの数
			glProgramUniform1i(progStandard, UniformLocation::lightCount, lightCount);
		}

		/// <summary>
		/// ライトデータを追加する
		/// </summary>
		/// <param name="lightData"> 追加するライトデータ </param>
		void AddLightData(LightData& lightData)
		{
			lightList.emplace(&lightData);
		}

		/// <summary>
		/// ライトデータを削除する
		/// </summary>
		/// <param name="lightData"> 削除するライトデータ </param>
		void EraseLightData(LightData& lightData)
		{
			lightList.erase(&lightData);
		}

	} // namespace LightData

} // namespace PokarinEngine