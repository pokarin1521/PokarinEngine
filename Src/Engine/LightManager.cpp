/**
* @file LightManager.cpp
*/
#include "LightManager.h"

#include "Components/Camera.h"

#include "Shader/Shader.h"

#include <algorithm>
#include <set>
#include <unordered_set>

namespace PokarinEngine
{
	/// <summary>
	/// ライト管理用
	/// </summary>
	namespace LightManager
	{
		// ---------------------------
		// 型の別名を定義
		// ---------------------------

		using LightDataList = std::unordered_set<const LightData*>;

		// 昇順になるようにset型を使う
		using LightIndexList = std::set<int>;

		// ----------------------------
		// 変数
		// ----------------------------

		// ライトデータ配列
		LightDataList lightList;

		// 平行光源
		const LightData* directionalLight = nullptr;

		// 環境光(青空をイメージ)
		// 方向を持たないので色だけ
		const Color ambientLight = { 0.05f, 0.15f, 0.25f, 1.0f };

		// 標準シェーダの識別番号
		GLuint progStandard = 0;

		// 計算で使用するライトの最大数
		const size_t lightMax = 16;

		// -----------------------------
		// 関数
		// -----------------------------

		/// ここでしか使わないので、cppのみに書く
		/// <summary>
		/// 平行光源のライトデータをシェーダに設定する
		/// </summary>
		void SetToShader_DirectionalLight()
		{
			// 標準シェーダ
			static const Shader::ProgType progStandard = Shader::ProgType::Standard;

			// 平行光源がない場合は
			// 黒色の光源としてシェーダに設定する
			if (!directionalLight)
			{
				Shader::SetVector3(progStandard, UniformVector3::directionalLight_Color, Color::black);

				return;
			}

			// 色
			const Color color = directionalLight->color * directionalLight->intensity;

			// 色を設定
			Shader::SetVector3(progStandard, UniformVector3::directionalLight_Color, color);

			// 向きを設定
			Shader::SetVector3(progStandard, UniformVector3::directionalLight_Direction,
				directionalLight->direction);
		}

		/// <summary>
		/// ライト情報をシェーダに設定する
		/// </summary>
		/// <param name="[in] camera"> 使用するカメラ </param>
		void SetToShader(const Camera& camera)
		{
			// -----------------------------------------------
			// 平行光源のライトデータをシェーダに設定する
			// -----------------------------------------------

			SetToShader_DirectionalLight();

			// -------------------------------------- 
			// 環境光のデータをシェーダに設定する
			// --------------------------------------

			// 標準シェーダ
			static const Shader::ProgType progStandard = Shader::ProgType::Standard;

			// 環境光を設定
			Shader::SetVector3(progStandard, UniformVector3::ambientLight, ambientLight);

			// -------------------------
			// 使用中ライトを確認
			// -------------------------

			// 使用中のライトがなければコピーするライト数を0に設定
			if (lightList.empty())
			{
				Shader::SetInt(progStandard, UniformInt::lightCount, 0);

				return;
			}

			// -----------------------------------------
			// カメラからライトまでの距離を計算
			// -----------------------------------------

			// -------------- 下準備 ----------------

			// カメラの正面ベクトル
			const Vector3 front = camera.transform->Front();

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
				// 平行光源は距離関係なく使うので
				// 別で処理ができるように変数に保持して次のライトへ
				if (light->type == LightType::directional)
				{
					directionalLight = light;
					continue;
				}

				// カメラからライトまでのベクトル
				const Vector3 v = light->position - camera.transform->position;

				// カメラの後ろで、ライトの範囲外だった場合
				if (Vector3::Dot(front, v) <= -light->range)
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

			// ----------- 画面に影響するライトがなければ -------------
			// ----------- ライト数を0に設定する		  -------------

			// 画面内にライトがない
			if (distanceList.empty())
			{
				Shader::SetInt(progStandard, UniformInt::lightCount, 0);
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
				std::min(distanceList.size(), Shader::lightMax));

			// ライトの色と減衰開始角度
			std::vector<Vector4> colorAndFalloffAngle(lightCount);

			// ライトの座標と範囲(半径)
			std::vector<Vector4> positionAndRange(lightCount);

			// ライト方向と最大照射角度
			std::vector<Vector4> directionAndSpotAngle(lightCount);

			// -------- シェーダに設定するライトデータを設定する ---------

			for (int i = 0; i < lightCount; ++i)
			{
				// ライト
				const LightData* lightData = distanceList[i].lightData;

				// 明るさを反映した色
				const Color color = lightData->color * lightData->intensity;

				// 色と減衰開始角度を設定
				colorAndFalloffAngle[i] = {
					color.r, color.g, color.b, lightData->falloffAngle };

				// 位置と範囲(半径)を設定
				positionAndRange[i] = {
					lightData->position.x, lightData->position.y, lightData->position.z, lightData->range };

				// ライトの向きと最大照射角度を設定
				directionAndSpotAngle[i] = {
					lightData->direction.x, lightData->direction.y, lightData->direction.z, lightData->spotAngle };
			}

			// ----------- ライトデータをシェーダに設定する ------------

			// 色と減衰開始角度
			Shader::SetVector4List(progStandard, UniformVector4List::lightColorAndFalloffAngle,
				colorAndFalloffAngle);

			// 位置と範囲(半径)
			Shader::SetVector4List(progStandard, UniformVector4List::lightPositionAndRange,
				positionAndRange);

			// 向きと最大照射角度
			Shader::SetVector4List(progStandard, UniformVector4List::lightDirectionAndSpotAngle,
				directionAndSpotAngle);

			// 使用するライトの数
			Shader::SetInt(progStandard, UniformInt::lightCount, lightCount);
		}

		/// <summary>
		/// ライトデータを追加する
		/// </summary>
		/// <param name="[in] lightData"> 追加するライトデータ </param>
		void AddLightData(const LightData& lightData)
		{
			lightList.emplace(&lightData);
		}

		/// <summary>
		/// ライトデータを削除する
		/// </summary>
		/// <param name="[in] lightData"> 削除するライトデータ </param>
		void EraseLightData(const LightData& lightData)
		{
			// 平行光源のライトデータなら
			// 使用中の平行光源を削除する
			if (directionalLight == &lightData)
			{
				directionalLight = nullptr;
			}

			// ライトデータを削除する
			lightList.erase(&lightData);
		}

	} // namespace LightData

} // namespace PokarinEngine