/**
* @file AabbCollider.cpp
*/
#include "AabbCollider.h"

#include "ImGui/imgui.h"

namespace PokarinEngine
{
	/// <summary>
	/// <para> 座標変換したコライダーを取得する </para>
	/// <para> 回転角度は90度単位で指定すること </para>
	/// <para> それ以外の角度では正しい交差判定が行えない </para>
	/// </summary>
	/// <param name="[in] transform"> 座標変換行列 </param>
	/// <returns> 座標変換したコライダー </returns>
	ColliderPtr AabbCollider::GetTransformedCollider(
		const Matrix4x4& transform) const
	{
		// ------------------------------
		// 座標変換したコピーを作成
		// ------------------------------

		// 座標変換したAABBコライダーを格納する変数
		auto collider = std::make_shared<AabbCollider>();

		// 座標変換
		collider->aabb.min = Vector3(transform * Vector4(aabb.min, 1));
		collider->aabb.max = Vector3(transform * Vector4(aabb.max, 1));

		// ---------------------------------------
		// minの方が大きかったら入れ替える
		// ---------------------------------------

		for (int i = 0; i < Vector3::size; ++i)
		{
			if (collider->aabb.min[i] > collider->aabb.max[i])
			{
				const float tmp = collider->aabb.min[i];
				collider->aabb.min[i] = collider->aabb.max[i];
				collider->aabb.max[i] = tmp;
			}
		}

		// 座標変換したAABBコライダー
		return collider;
	}

	/// <summary>
	/// 情報を編集できるように表示する
	/// </summary>
	void AabbCollider::InfoEditor()
	{
		// -------------------------
		// 重複
		// -------------------------

		// 識別番号の文字列
		std::string id_string = GetID_String();

		//ImGui::Checkbox(IsTriggerTitle.c_str(), &isTrigger);

		// -------------------------
		// 中心座標
		// -------------------------

		aabb.center.RenderDrag("Center", id_string, sliderWidth);

		// 中心座標と大きさから
		// 最小座標と最大座標を求める
		aabb.min = aabb.center - aabb.size;
		aabb.max = aabb.center + aabb.size;

		// --------------------------
		// 大きさ
		// --------------------------

		aabb.size.RenderDrag("Size", id_string, sliderWidth);
	}
}