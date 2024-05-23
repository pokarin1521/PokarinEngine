/**
* @file SphereCollider.cpp
*/
#include "SphereCollider.h"

namespace PokarinEngine
{
	/// <summary>
	/// 座標変換したコライダーを取得する
	/// </summary>
	/// <param name="transform"></param>
	/// <returns></returns>
	ColliderPtr SphereCollider::GetTransformedCollider(
		const Matrix4x4& transform) const
	{
		// -------------------------------
		// 中心座標を座標変換する
		// -------------------------------

		// 座標変換した球体コライダーを格納する変数
		auto p = std::make_shared<SphereCollider>();

		// 座標変換
		p->sphere.position = Vector3(
			transform * Vector4(sphere.position, 1));

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
		p->sphere.radius = sphere.radius * maxScale;

		// 座標変換した球体コライダー
		return p;
	}
}