/**
* @file Rigidbody.cpp
*/
#include "Rigidbody.h"

#include "../Time.h"
#include "../Scene.h"

namespace PokarinEngine
{
	/// <summary>
	/// ゲーム再生中の更新
	/// </summary>
	void Rigidbody::Update_PlayGame()
	{
		// コンポーネントの持ち主
		const GameObject& owner = GetOwnerObject();
		
		// 前回の更新からの経過時間
		float deltaTime = Time::DeltaTime();

		// 接地している
		if (owner.isGrounded)
		{
			// 着地後は落下させる必要がないので、下方向への速度を0にする
			velocity.y = std::max(velocity.y, 0.0f);
		}
		// 接地していない
		else
		{
			// 重力加速度によって速度を更新する
			velocity.y -= gravity * gravityScale * deltaTime;
		}

		// 速度を座標に反映する
		owner.transform->position += velocity * deltaTime;
	}

} // namespace PokarinEngine