/**
* @file Intersect.h
*/
#ifndef INTERSECT_H_INCLUDED
#define INTERSECT_H_INCLUDED

#include "CollisionType.h"

namespace PokarinEngine
{
	/// <summary>
	/// Õ“Ë—p
	/// </summary>
	namespace Collision
	{
		/// <summary>
		/// AABB“¯m‚ÌŒğ·”»’è
		/// </summary>
		/// <param name="[in] a"> ”»’è‘ÎÛ‚ÌAABB(A) </param>
		/// <param name="[in] b"> ”»’è‘ÎÛ‚ÌAABB(B) </param>
		/// <param name="[out] penetration"> A‚ªB‚ÉŠÑ’Ê‚µ‚½‹——£ </param>
		/// <returns> 
		/// <para> true : Œğ·‚µ‚Ä‚¢‚é </para> 
		/// <para> false : Œğ·‚µ‚Ä‚¢‚È‚¢ </para> 
		/// </returns>
		bool Intersect(const AABB& a, const AABB& b, Vector3& penetration);

		/// <summary>
		/// ‹…‘Ì“¯m‚ÌŒğ·”»’è
		/// </summary>
		/// <param name="[in] a"> ”»’è‘ÎÛ‚Ì‹…‘ÌA </param>
		/// <param name="[in] b"> ”»’è‘ÎÛ‚Ì‹…‘ÌB </param>
		/// <param name="[out] penetration"> A‚ªB‚ÉŠÑ’Ê‚µ‚½‹——£ </param>
		/// <returns> 
		/// <para> true : Œğ·‚µ‚Ä‚¢‚é </para> 
		/// <para> false : Œğ·‚µ‚Ä‚¢‚È‚¢ </para> 
		/// </returns>
		bool Intersect(const Sphere& a, const Sphere& b, Vector3& penetration);

		/// <summary>
		/// AABB‚Æ‹…‘Ì‚ÌŒğ·”»’è
		/// </summary>
		/// <param name="[in] aabb"> ”»’è‘ÎÛ‚ÌAABB </param>
		/// <param name="[in] sphere"> ”»’è‘ÎÛ‚Ì‹…‘Ì </param>
		/// <param name="[out] penetration"> AABB‚ª‹…‘Ì‚ÉŠÑ’Ê‚µ‚½‹——£ </param>
		/// <returns> 
		/// <para> true : Œğ·‚µ‚Ä‚¢‚é </para> 
		/// <para> false : Œğ·‚µ‚Ä‚¢‚È‚¢ </para> 
		/// </returns>
		bool Intersect(const AABB& aabb, const Sphere& sphere, Vector3& penetration);
	}
}

#endif // !INTERSECT_H_INCLUDED
