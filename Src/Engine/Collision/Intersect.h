/**
* @file Intersect.h
*/
#ifndef INTERSECT_H_INCLUDED
#define INTERSECT_H_INCLUDED

#include "CollisionType.h"

namespace PokarinEngine
{
	/// <summary>
	/// 衝突用
	/// </summary>
	namespace Collision
	{
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
		/// OBBと球体の交差判定
		/// </summary>
		/// <param name="[in] box"> 判定対象のOBB </param>
		/// <param name="[in] sphere"> 判定対象の球体 </param>
		/// <param name="[out] penetration"> OBBの球体に対する貫通ベクトル </param>
		/// <returns> 
		/// <para> true : 交差している </para> 
		/// <para> false : 交差していない </para> 
		/// </returns>
		bool Intersect(const Box& box, const Sphere& sphere, Vector3& penetration);
	}
}

#endif // !INTERSECT_H_INCLUDED
