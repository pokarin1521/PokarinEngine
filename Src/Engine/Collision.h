/**
* @file Collision.h
*/
#ifndef COLLISION_H_INCLUDED
#define COLLISION_H_INCLUDED

#include "VecMath.h"

namespace PokarinEngine
{
	/// <summary>
	/// <para> 軸平行境界ボックス(Axis Aligned Bounding Box) </para>
	/// <para> (ワールド座標系のxyz軸に沿った直方体) </para>
	/// </summary>
	struct AABB
	{
		Vec3 min; // AABBの最小座標
		Vec3 max; // AABBの最大座標
	};

	/// <summary>
	/// 球体
	/// </summary>
	struct Sphere
	{
		Vec3 position; // 中心座標
		float radius;  // 半径
	};

	/// <summary>
	/// 光線
	/// </summary>
	struct Ray
	{
		Vec3 start;		// 光線の始点
		Vec3 direction; // 光線の向き
	};

	/// <summary>
	/// AABB同士の交差判定
	/// </summary>
	/// <param name="[in] a"> 判定対象のAABB(衝突する側) </param>
	/// <param name="[in] b"> 判定対象のAABB(衝突される側) </param>
	/// <param name="[out] penetration"> AがBに貫通した距離 </param>
	/// <returns> 
	/// <para> true : 交差している </para> 
	/// <para> false : 交差していない </para> 
	/// </returns>
	bool Intersect(const AABB& a, const AABB& b, Vec3& penetration);

	/// <summary>
	/// 球体同士の交差判定
	/// </summary>
	/// <param name="[in] a"> 判定対象の球体(衝突する側) </param>
	/// <param name="[in] b"> 判定対象の球体(衝突される側) </param>
	/// <param name="[out] penetration"> AがBに貫通した距離 </param>
	/// <returns> 
	/// <para> true : 交差している </para> 
	/// <para> false : 交差していない </para> 
	/// </returns>
	bool Intersect(const Sphere& a, const Sphere& b, Vec3& penetration);

	/// <summary>
	/// AABBと球体の交差判定
	/// </summary>
	/// <param name="[in] aabb"> 判定対象のAABB(衝突する側) </param>
	/// <param name="[in] sphere"> 判定対象の球体(衝突される側) </param>
	/// <param name="[out] penetration"> AABBが球体に貫通した距離 </param>
	/// <returns> 
	/// <para> true : 交差している </para> 
	/// <para> false : 交差していない </para> 
	/// </returns>
	bool Intersect(const AABB& aabb, const Sphere& sphere, Vec3& penetration);

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

} // namespace PokarinEngine

#endif // !COLLISION_H_INCLUDED
