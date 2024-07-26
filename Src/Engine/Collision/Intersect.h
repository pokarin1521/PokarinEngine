/**
* @file Intersect.h
*/
#ifndef INTERSECT_H_INCLUDED
#define INTERSECT_H_INCLUDED

#include "CollisionType.h"

namespace PokarinEngine
{
	/// <summary>
	/// �Փ˗p
	/// </summary>
	namespace Collision
	{
		/// <summary>
		/// ���̓��m�̌�������
		/// </summary>
		/// <param name="[in] a"> ����Ώۂ̋���A </param>
		/// <param name="[in] b"> ����Ώۂ̋���B </param>
		/// <param name="[out] penetration"> A��B�Ɋђʂ������� </param>
		/// <returns> 
		/// <para> true : �������Ă��� </para> 
		/// <para> false : �������Ă��Ȃ� </para> 
		/// </returns>
		bool Intersect(const Sphere& a, const Sphere& b, Vector3& penetration);

		/// <summary>
		/// OBB�Ƌ��̂̌�������
		/// </summary>
		/// <param name="[in] box"> ����Ώۂ�OBB </param>
		/// <param name="[in] sphere"> ����Ώۂ̋��� </param>
		/// <param name="[out] penetration"> OBB�̋��̂ɑ΂���ђʃx�N�g�� </param>
		/// <returns> 
		/// <para> true : �������Ă��� </para> 
		/// <para> false : �������Ă��Ȃ� </para> 
		/// </returns>
		bool Intersect(const Box& box, const Sphere& sphere, Vector3& penetration);
	}
}

#endif // !INTERSECT_H_INCLUDED
