/**
* @file Light.cpp
*/
#include "Light.h"

#include "ImGui/imgui.h"

#include "../Engine.h"

namespace PokarinEngine
{
	/// <summary>
	/// ライトを解放するデストラクタ
	/// </summary>
	Light::~Light()
	{
		LightParameter::EraseLightData(lightData);
	}

	/// <summary>
	/// <para> コンポーネントを初期化 </para> 
	/// <para> ゲームオブジェクトに追加したときに実行 </para>
	/// </summary>
	void Light::Awake()
	{
		LightParameter::AddLightData(lightData);
	}

	/// <summary>
	/// コンポーネントを更新
	/// </summary>
	/// <param name="deltaTime"> 前回の更新からの経過時間(秒) </param>
	void Light::Update(float deltaTime)
	{
		// --------------------------------
		// ライトの座標を設定
		// --------------------------------

		// コンポーネントの持ち主
		const GameObject& owner = GetOwner();

		// ライトの座標を設定
		lightData.position = owner.transform->position;

		// ポイントライトなら向きは不要なので向きの計算はしない
		if (lightData.type == LightParameter::Type::pointLight)
		{
			return;
		}

		// -------------------------------------------
		// 向きが必要なので計算する
		// -------------------------------------------

		// ライトの向き
		// 回転なしの向きベクトルとして初期化
		Vector3 direction = { 0, 0, -1 };

		// ------ X軸回転を反映した向きベクトルを計算 ------

		// 持ち主のX回転角度(Sin)
		const float sinX = sin(owner.transform->rotation.x);

		// 持ち主のX回転角度(Cos)
		const float cosX = cos(owner.transform->rotation.x);

		// 回転を反映した向きベクトルを計算
		direction = {
			direction.x,
			direction.y * cosX - direction.z * sinX,
			direction.y * sinX + direction.z * cosX };

		// ------ Y軸回転を反映した向きベクトルを計算 ------

		// 持ち主のY回転角度(Sin)
		const float sinY = sin(owner.transform->rotation.y);

		// 持ち主のY回転角度(Cos)
		const float cosY = cos(owner.transform->rotation.y);

		// 回転を反映した向きベクトルを計算
		direction = {
			direction.x * cosY + direction.z * sinY,
			direction.y,
			-direction.x * sinY + direction.z * cosY };

		// ---------------------------------- 
		// ライトの向きを設定 
		// ----------------------------------

		lightData.direction = direction;
	}

	/// <summary>
	/// エディタに情報を描画する
	/// </summary>
	void Light::RenderInfo()
	{
		if (ImGui::CollapsingHeader("Light"))
		{
			ImGui::Text("Light Information");
		}
	}

} // namespace PokarinEngine