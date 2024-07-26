/**
* @file SphereCollider.cpp
*/
#include "SphereCollider.h"

#include "ImGui/imgui.h"

#include "../../GameObject.h"

namespace PokarinEngine
{
	/// <summary>
	/// 座標変換したコライダーを取得する
	/// </summary>
	/// <returns> 座標変換したコライダー </returns>
	ColliderPtr SphereCollider::GetTransformedCollider() const
	{
		// 自身の座標変換行列
		Matrix4x4 transformMatrix = GetTransformMatrix();

		// 座標変換したコライダー
		auto collider = std::make_shared<SphereCollider>();

		// 中心座標を取得する
		collider->sphere.center = Matrix4x4::Translate(transformMatrix);

		// 半径を取得する
		// 座標変換行列は直径で作成されているので、半分にする
		collider->sphere.radius = Matrix4x4::Scale(transformMatrix).x / 2.0f;

		return collider;
	}

	/// <summary>
	/// 座標変換行列を取得する
	/// </summary>
	/// <returns> 座標変換行列 </returns>
	Matrix4x4 SphereCollider::GetTransformMatrix() const
	{
		// --------------------------------------
		// 持ち主の座標変換行列を分解する
		// --------------------------------------
		
		// 持ち主であるゲームオブジェクトの座標・回転・拡大率
		TransformPtr transform = GetOwnerObject().transform;

		// 持ち主であるゲームオブジェクトの座標変換行列
		Matrix4x4 transformMatrix = transform->GetTransformMatrix();

		// -------------------------------
		// 中心座標を座標変換する
		// -------------------------------

		// 座標変換した中心座標
		Vector3 center = Vector3(
			transformMatrix * Vector4(sphere.center, 1));

		// ----------------------------------
		// 球体の半径に拡大率を反映する
		// ----------------------------------

		/* 球体の拡大率は、
		XYZが等しくなくてはならない(楕円形になるから)

		オブジェクトの拡大率は個別に指定できてしまうので
		「座標変換行列に含まれるXYZの拡大率のうち最大の値」を
		球体の拡大率とする(unityの球体コライダーと同じ挙動) */

		// 拡大率を取得
		const Vector3 scale = Matrix4x4::Scale(transformMatrix);

		// X,Y,Zの拡大率で最大の値
		const float maxScale = std::max({ scale.x, scale.y, scale.z });

		// 拡大率を反映した半径
		const float radius = sphere.radius * maxScale;

		// ----------------------------------------
		// 座標変換行列を作成して返す
		// ----------------------------------------

		// 球体の直径
		const float diameter = radius * 2.0f;

		// 座標変換行列を作成して返す
		// オブジェクトの大きさとしては直径の値が正しいので、直径を設定する
		return Matrix4x4::CreateTransformMatrix(
			center, transform->rotation, Vector3(diameter));
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

		sphere.center.RenderDrag("Center", GetID_String(), sliderWidth);

		// ---------------------
		// 大きさ
		// ---------------------

		ImGui::DragFloat("Radius##SphereCollider", &sphere.radius);
	}
}