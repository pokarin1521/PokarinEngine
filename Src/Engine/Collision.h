/**
* @file Collision.h
*/
#ifndef COLLISION_H_INCLUDED
#define COLLISION_H_INCLUDED

#include "Math/Vector.h"

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
			Vector3 min; // AABBの最小座標
			Vector3 max; // AABBの最大座標
		};

		/// <summary>
		/// 球体
		/// </summary>
		struct Sphere
		{
			Vector3 position; // 中心座標
			float radius;  // 半径
		};

		/// <summary>
		/// <para> 有向境界ボックス(OBB) </para>
		/// <para> (物体の回転に対応した直方体) </para>
		/// </summary>
		struct Box
		{
			// 中心座標
			Vector3 position;

			// 軸の向きベクトル
			Vector3 axis[Vector3::size];

			// 軸方向の大きさ
			Vector3 scale;
		};

		/// <summary>
		/// 光線
		/// </summary>
		struct Ray
		{
			Vector3 start;	   // 光線の始点
			Vector3 direction; // 光線の向き
		};

		/// <summary>
		/// AABB同士の交差判定
		/// </summary>
		/// <param name="[in] a"> 判定対象のAABB(A) </param>
		/// <param name="[in] b"> 判定対象のAABB(B) </param>
		/// <param name="[out] penetration"> AがBに貫通した距離 </param>
		/// <returns> 
		/// <para> true : 交差している </para> 
		/// <para> false : 交差していない </para> 
		/// </returns>
		bool Intersect(const AABB& a, const AABB& b, Vector3& penetration);

		/// <summary>
		/// 球体同士の交差判定
		/// </summary>
		/// <param name="[in] a"> 判定対象の球体A </param>
		/// <param name="[in] b"> 判定対象の球体B </param>
		/// <param name="[out] penetration"> AがBに貫通した距離 </param>
		/// <returns> 
		/// <para> true : 交差している </para> 
		/// <para> false : 交差していない </para> 
		/// </returns>
		bool Intersect(const Sphere& a, const Sphere& b, Vector3& penetration);

		/// <summary>
		/// AABBと球体の交差判定
		/// </summary>
		/// <param name="[in] aabb"> 判定対象のAABB </param>
		/// <param name="[in] sphere"> 判定対象の球体 </param>
		/// <param name="[out] penetration"> AABBが球体に貫通した距離 </param>
		/// <returns> 
		/// <para> true : 交差している </para> 
		/// <para> false : 交差していない </para> 
		/// </returns>
		bool Intersect(const AABB& aabb, const Sphere& sphere, Vector3& penetration);

		/// <summary>
		/// OBBと球体の交差判定
		/// </summary>
		/// <param name="[in] aabb"> 判定対象のOBB </param>
		/// <param name="[in] sphere"> 判定対象の球体 </param>
		/// <param name="[out] penetration"> OBBが球体に貫通した距離 </param>
		/// <returns> 
		/// <para> true : 交差している </para> 
		/// <para> false : 交差していない </para> 
		/// </returns>
		bool Intersect(const Box& box, const Sphere& sphere, Vector3& penetration);

		/// <summary>
		/// AABBと光線の交差判定
		/// </summary>
		/// <param name="[in] aabb"> 判定対象のAABB </param>
		/// <param name="[in] ray"> 判定対象の光線 </param>
		/// <param name="[out] distance"> 光線がAABBと最初に交差するまでの距離 </param>
		/// <returns>
		/// <para> true : 交差している </para>
		/// <para> false : 交差していない </para>
		/// </returns>
		bool Intersect(const AABB& aabb, const Ray& ray, float& distance);

		/// <summary>
		/// 球体と光線の交差判定
		/// </summary>
		/// <param name="[in] sphere"> 判定対象の球体 </param>
		/// <param name="[in] ray"> 判定対象の光線 </param>
		/// <param name="[out] distance"> 光線がAABBと最初に交差するまでの距離 </param>
		/// <returns>
		/// <para> true : 交差している </para>
		/// <para> false : 交差していない </para>
		/// </returns>
		bool Intersect(const Sphere& sphere, const Ray& ray, float& distance);

	} // namespace Collision

} // namespace PokarinEngine

#endif // !COLLISION_H_INCLUDED
