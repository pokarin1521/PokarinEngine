/**
* @file CollisionType.h
*/
#ifndef POKARINENGINE_COLLISIONTYPE_H_INCLUDED
#define POKARINENGINE_COLLISIONTYPE_H_INCLUDED

#include "../Math/Vector.h"

namespace PokarinEngine
{
	/// <summary>
	/// 衝突用
	/// </summary>
	namespace Collision
	{
		/// <summary>
		/// <para> 有向境界ボックス(OBB) </para>
		/// <para> (物体の回転に対応した直方体) </para>
		/// </summary>
		struct Box
		{
			// 中心座標
			Vector3 center = Vector3::zero;

			// 軸の向きベクトル
			Vector3 axis[Vector3::size] = {
				{ 1, 0, 0 },{ 0, 1, 0 }, { 0, 0, 1 } };

			// 大きさ
			Vector3 size = Vector3::one;
		};

		/// <summary>
		/// 球体
		/// </summary>
		struct Sphere
		{
			// 中心座標
			Vector3 center = Vector3(0);

			// 半径
			float radius = 0.5f;
		};

	} // namespace Collision

} // namespace PokarinEngine

#endif // !POKARINENGINE_COLLISIONTYPE_H_INCLUDED
