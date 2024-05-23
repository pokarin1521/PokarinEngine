/**
* @file Collision.h
*/
#ifndef COLLISION_H_INCLUDED
#define COLLISION_H_INCLUDED

#include "Math/Vector.h"

namespace PokarinEngine
{
	/// <summary>
	/// �Փ˗p
	/// </summary>
	namespace Collision
	{
		/// <summary>
		/// <para> �����s���E�{�b�N�X(AABB) </para>
		/// <para> (���[���h���W�n��xyz���ɉ�����������) </para>
		/// </summary>
		struct AABB
		{
			Vector3 min; // AABB�̍ŏ����W
			Vector3 max; // AABB�̍ő���W
		};

		/// <summary>
		/// ����
		/// </summary>
		struct Sphere
		{
			Vector3 position; // ���S���W
			float radius;  // ���a
		};

		/// <summary>
		/// <para> �L�����E�{�b�N�X(OBB) </para>
		/// <para> (���̂̉�]�ɑΉ�����������) </para>
		/// </summary>
		struct Box
		{
			// ���S���W
			Vector3 position;

			// ���̌����x�N�g��
			Vector3 axis[Vector3::size];

			// �������̑傫��
			Vector3 scale;
		};

		/// <summary>
		/// ����
		/// </summary>
		struct Ray
		{
			Vector3 start;	   // �����̎n�_
			Vector3 direction; // �����̌���
		};

		/// <summary>
		/// AABB���m�̌�������
		/// </summary>
		/// <param name="[in] a"> ����Ώۂ�AABB(A) </param>
		/// <param name="[in] b"> ����Ώۂ�AABB(B) </param>
		/// <param name="[out] penetration"> A��B�Ɋђʂ������� </param>
		/// <returns> 
		/// <para> true : �������Ă��� </para> 
		/// <para> false : �������Ă��Ȃ� </para> 
		/// </returns>
		bool Intersect(const AABB& a, const AABB& b, Vector3& penetration);

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
		/// AABB�Ƌ��̂̌�������
		/// </summary>
		/// <param name="[in] aabb"> ����Ώۂ�AABB </param>
		/// <param name="[in] sphere"> ����Ώۂ̋��� </param>
		/// <param name="[out] penetration"> AABB�����̂Ɋђʂ������� </param>
		/// <returns> 
		/// <para> true : �������Ă��� </para> 
		/// <para> false : �������Ă��Ȃ� </para> 
		/// </returns>
		bool Intersect(const AABB& aabb, const Sphere& sphere, Vector3& penetration);

		/// <summary>
		/// OBB�Ƌ��̂̌�������
		/// </summary>
		/// <param name="[in] aabb"> ����Ώۂ�OBB </param>
		/// <param name="[in] sphere"> ����Ώۂ̋��� </param>
		/// <param name="[out] penetration"> OBB�����̂Ɋђʂ������� </param>
		/// <returns> 
		/// <para> true : �������Ă��� </para> 
		/// <para> false : �������Ă��Ȃ� </para> 
		/// </returns>
		bool Intersect(const Box& box, const Sphere& sphere, Vector3& penetration);

		/// <summary>
		/// AABB�ƌ����̌�������
		/// </summary>
		/// <param name="[in] aabb"> ����Ώۂ�AABB </param>
		/// <param name="[in] ray"> ����Ώۂ̌��� </param>
		/// <param name="[out] distance"> ������AABB�ƍŏ��Ɍ�������܂ł̋��� </param>
		/// <returns>
		/// <para> true : �������Ă��� </para>
		/// <para> false : �������Ă��Ȃ� </para>
		/// </returns>
		bool Intersect(const AABB& aabb, const Ray& ray, float& distance);

		/// <summary>
		/// ���̂ƌ����̌�������
		/// </summary>
		/// <param name="[in] sphere"> ����Ώۂ̋��� </param>
		/// <param name="[in] ray"> ����Ώۂ̌��� </param>
		/// <param name="[out] distance"> ������AABB�ƍŏ��Ɍ�������܂ł̋��� </param>
		/// <returns>
		/// <para> true : �������Ă��� </para>
		/// <para> false : �������Ă��Ȃ� </para>
		/// </returns>
		bool Intersect(const Sphere& sphere, const Ray& ray, float& distance);

	} // namespace Collision

} // namespace PokarinEngine

#endif // !COLLISION_H_INCLUDED
