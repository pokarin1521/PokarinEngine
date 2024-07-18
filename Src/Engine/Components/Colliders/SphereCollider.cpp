/**
* @file SphereCollider.cpp
*/
#include "SphereCollider.h"

#include "ImGui/imgui.h"

namespace PokarinEngine
{
	/// <summary>
	/// 座標変換したコライダーを取得する
	/// </summary>
	/// <param name="[in] transform"> 座標変換行列 </param>
	/// <returns> 座標変換したコライダー </returns>
	ColliderPtr SphereCollider::GetTransformedCollider(
		const Matrix4x4& transform) const
	{
		// -------------------------------
		// 中心座標を座標変換する
		// -------------------------------

		// 座標変換した球体コライダーを格納する変数
		auto collider = std::make_shared<SphereCollider>();

		// 座標変換
		collider->sphere.center = Vector3(
			transform * Vector4(sphere.center, 1));

		// -------------------------------
		// 球体の拡大率を反映
		// -------------------------------

		/* 球体の拡大率は、
		XYZが等しくなくてはならない(楕円形になるから)

		オブジェクトの拡大率は個別に指定できてしまうので
		「座標変換行列に含まれるXYZの拡大率のうち最大の値」を
		球体の拡大率とする(unityの球体コライダーと同じ挙動) */

		// 拡大率を取得
		const Vector3 scale = Matrix4x4::Scale(transform);

		// X,Y,Zの拡大率で最大の値
		const float maxScale = std::max({ scale.x, scale.y, scale.z });

		// 拡大率を反映
		collider->sphere.radius = sphere.radius * maxScale;

		// 座標変換した球体コライダー
		return collider;
	}

	/// <summary>
	/// 情報を編集できるように表示する
	/// </summary>
	void SphereCollider::InfoEditor()
	{
		// ---------------------
		// 重複
		// ---------------------

		ImGui::Checkbox("Is Trigger", &isTrigger);

		// ---------------------
		// 中心座標
		// ---------------------

		sphere.center.RenderDrag("Center", "SphereCollider", sliderWidth);		
		
		// ---------------------
		// 大きさ
		// ---------------------

		ImGui::DragFloat("Center##SphereCollider", &sphere.radius);
	}
}