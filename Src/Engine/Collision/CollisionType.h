/**
* @file CollisionType.h
*/
#ifndef COLLISIONTYPE_H_INCLUDED
#define COLLISIONTYPE_H_INCLUDED

#include "../Math/Vector.h"

namespace PokarinEngine
{
	/// <summary>
	/// 衝突用
	/// </summary>
	namespace Collision
	{
		/// <summary>
		/// <para> 軸平行境界ボックス(AABB) </para>
		/// <para> (ワールド座標系のxyz軸に沿った直方体) </para>
		/// </summary>
		struct AABB
		{
			// AABBの最小座標
			Vector3 min = Vector3(-1);

			// AABBの最大座標
			Vector3 max = Vector3(1);

			// 中心座標
			Vector3 center = Vector3(0);

			// 中心からの大きさ
			Vector3 size = Vector3(1);
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

		/// <summary>
		/// <para> 有向境界ボックス(OBB) </para>
		/// <para> (物体の回転に対応した直方体) </para>
		/// </summary>
		struct Box
		{
			// 中心座標
			Vector3 center = Vector3(0);

			// 軸の向きベクトル
			Vector3 axis[Vector3::size] = { Vector3(0) };

			// 軸方向の大きさ
			Vector3 scale = Vector3(0);
		};

	} // namespace Collision

} // namespace PokarinEngine

#endif // !COLLISIONTYPE_H_INCLUDED
