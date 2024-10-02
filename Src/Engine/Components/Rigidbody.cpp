/**
* @file Rigidbody.cpp
*/
#include "Rigidbody.h"

#include "ImGui/imgui.h"

#include "Json/Json.h"

#include "../GameObject.h"
#include "../Scene.h"
#include "../Time.h"
#include "../ImGuiHelper.h"

namespace PokarinEngine
{
	/// <summary>
	/// ゲーム再生時の初期化処理
	/// </summary>
	void Rigidbody::Initialize_PlayGame()
	{
		// 速度を初期化
		velocity = Vector3::zero;
	}

	/// <summary>
	/// ゲーム再生中の更新
	/// </summary>
	void Rigidbody::Update_PlayGame()
	{
		// 現状、重力の処理しか行っていないので
		// 重力が無効なら何もしない
		if (!useGravity)
		{
			return;
		}

		// コンポーネントの持ち主
		const GameObject& owner = GetOwnerObject();

		// 前回の更新からの経過時間
		float deltaTime = Time::DeltaTime();

		// 接地している
		if (owner.isGrounded)
		{
			// 着地後は落下させる必要がないので、落下速度を0にする
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

	/// <summary>
	/// 削除時の処理
	/// </summary>
	void Rigidbody::OnDestroy()
	{
		GetOwnerObject().hasRigidbody = false;
	}

	/// <summary>
	/// 情報を編集できるように表示する
	/// </summary>
	void Rigidbody::InfoEditor()
	{
		// 重力の有無を表示する
		ImGuiHelper::CheckBox("Use Gravity", GetID_String(), useGravity);
	}

	/// <summary>
	/// コンポーネントの情報をJson型に格納する
	/// </summary>
	/// <param name="[out] json"> 情報を格納するJson型 </param>
	void Rigidbody::ToJson(Json& json) const
	{
		// 重力の有無を格納する
		json["UseGravity"] = useGravity;
	}

	/// <summary>
	/// コンポーネントの情報をJson型から取得する
	/// </summary>
	/// <param name="[in] json"> 情報を格納しているJson型 </param>
	void Rigidbody::FromJson(const Json& json)
	{
		// 重力の有無を取得する
		json["UseGravity"].get_to(useGravity);
	}

} // namespace PokarinEngine