/**
* @file AabbCollider.cpp
*/
#include "AabbCollider.h"

namespace PokarinEngine
{
	/// <summary>
	/// <para> 座標変換したコライダーを取得する </para>
	/// <para> 回転角度は90度単位で指定すること </para>
	/// <para> それ以外の角度では正しい交差判定が行えない </para>
	/// </summary>
	/// <param name="transform"> 座標変換行列 </param>
	/// <returns> 座標変換したコライダー </returns>
	ColliderPtr AabbCollider::GetTransformedCollider(
		const Matrix4x4& transform) const
	{
		// ------------------------------
		// 座標変換したコピーを作成
		// ------------------------------

		// 座標変換したAABBコライダーを格納する変数
		auto p = std::make_shared<AabbCollider>();

		// 座標変換
		p->aabb.min = Vector3(transform * Vector4(aabb.min, 1));
		p->aabb.max = Vector3(transform * Vector4(aabb.max, 1));

		// ---------------------------------------
		// minの方が大きかったら入れ替える
		// ---------------------------------------

		for (int i = 0; i < Vector_Size::vector3; ++i)
		{
			if (p->aabb.min[i] > p->aabb.max[i])
			{
				const float tmp = p->aabb.min[i];
				p->aabb.min[i] = p->aabb.max[i];
				p->aabb.max[i] = tmp;
			}
		}

		// 座標変換したAABBコライダー
		return p;
	}
}