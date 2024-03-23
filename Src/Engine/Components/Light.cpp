/**
* @file Light.cpp
*/
#include "Light.h"
#include "../Engine.h"
#include "../LightManager.h"

namespace PokarinEngine
{
	/// <summary>
	/// ライトを解放するデストラクタ
	/// </summary>
	Light::~Light()
	{
		// ライトを解放する
		LightManager::DeallocateLight(lightIndex);
	}

	/// <summary>
	/// コンポーネントを初期化
	/// ゲームオブジェクトに追加したときに実行
	/// </summary>
	void Light::Awake()
	{
		// ライトがなければ取得する
		if (lightIndex < 0)
		{
			lightIndex = LightManager::AllocateLight();
		}
	}

	/// <summary>
	/// コンポーネントを更新
	/// </summary>
	/// <param name="deltaTime"> 前回の更新からの経過時間(秒) </param>
	void Light::Update(float deltaTime)
	{
		// -------------------
		// ライトを取得
		// -------------------

		// コンポーネントの持ち主
		const GameObject& owner = GetOwner();

		// ライト(なければnullptrが入る)
		auto light = LightManager::GetLight(lightIndex);

		// -----------------------------------------------
		// コンポーネントの値を取得したライトに反映する
		// -----------------------------------------------

		// ライトがあるので、値を反映する
		if (light)
		{
			// -------------------- 
			// ライト全般 
			// --------------------

			// 座標
			light->position = owner.transform->position;

			// 明るさ
			light->intensity = intensity;

			// 色
			light->color = color;

			// 半径
			light->radius = radius;

			// 最大照射角度
			// 最初はポイントライトとして扱う
			light->coneAngle = 0;

			// ---------------------- 
			// スポットライト 
			// ----------------------

			// 種類がスポットライトなら
			// スポットライトの値を反映する
			if (type == Type::spotLight)
			{
				// ライトの向き
				// 回転なしの向きベクトルとして初期化
				Vec3 direction = { 0, 0, -1 };

				// ------ X軸回転を反映した向きベクトルを計算 ------

				// sin, cos
				const float sinX = sin(owner.transform->rotation.x);
				const float cosX = cos(owner.transform->rotation.x);

				// 回転を反映した向きベクトルを計算
				direction = {
					direction.x,
					direction.y * cosX - direction.z * sinX,
					direction.y * sinX + direction.z * cosX };

				// ------ Y軸回転を反映した向きベクトルを計算 ------

				// sin, cos
				const float sinY = sin(owner.transform->rotation.y);
				const float cosY = cos(owner.transform->rotation.y);

				// 回転を反映した向きベクトルを計算
				direction = {
					direction.x * cosY + direction.z * sinY,
					direction.y,
					-direction.x * sinY + direction.z * cosY };

				// ------------- パラメータを反映 -------------

				light->directoin = direction;
				light->coneAngle = coneAngle;
				light->falloffAngle = falloffAngle;
			}

		} // if light
	}

} // namespace PokarinEngine