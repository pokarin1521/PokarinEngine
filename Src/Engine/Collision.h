/**
* @file Collision.h
*/
#ifndef COLLISION_H_INCLUDED
#define COLLISION_H_INCLUDED

#include "VecMath.h"

namespace PokarinEngine
{
	/// <summary>
	/// <para> �����s���E�{�b�N�X(Axis Aligned Bounding Box) </para>
	/// <para> (���[���h���W�n��xyz���ɉ�����������) </para>
	/// </summary>
	struct AABB
	{
		Vec3 min; // AABB�̍ŏ����W
		Vec3 max; // AABB�̍ő���W
	};

	/// <summary>
	/// ����
	/// </summary>
	struct Sphere
	{
		Vec3 position; // ���S���W
		float radius;  // ���a
	};

	/// <summary>
	/// ����
	/// </summary>
	struct Ray
	{
		Vec3 start;		// �����̎n�_
		Vec3 direction; // �����̌���
	};

	/// <summary>
	/// AABB���m�̌�������
	/// </summary>
	/// <param name="[in] a"> ����Ώۂ�AABB(�Փ˂��鑤) </param>
	/// <param name="[in] b"> ����Ώۂ�AABB(�Փ˂���鑤) </param>
	/// <param name="[out] penetration"> A��B�Ɋђʂ������� </param>
	/// <returns> 
	/// <para> true : �������Ă��� </para> 
	/// <para> false : �������Ă��Ȃ� </para> 
	/// </returns>
	bool Intersect(const AABB& a, const AABB& b, Vec3& penetration);

	/// <summary>
	/// ���̓��m�̌�������
	/// </summary>
	/// <param name="[in] a"> ����Ώۂ̋���(�Փ˂��鑤) </param>
	/// <param name="[in] b"> ����Ώۂ̋���(�Փ˂���鑤) </param>
	/// <param name="[out] penetration"> A��B�Ɋђʂ������� </param>
	/// <returns> 
	/// <para> true : �������Ă��� </para> 
	/// <para> false : �������Ă��Ȃ� </para> 
	/// </returns>
	bool Intersect(const Sphere& a, const Sphere& b, Vec3& penetration);

	/// <summary>
	/// AABB�Ƌ��̂̌�������
	/// </summary>
	/// <param name="[in] aabb"> ����Ώۂ�AABB(�Փ˂��鑤) </param>
	/// <param name="[in] sphere"> ����Ώۂ̋���(�Փ˂���鑤) </param>
	/// <param name="[out] penetration"> AABB�����̂Ɋђʂ������� </param>
	/// <returns> 
	/// <para> true : �������Ă��� </para> 
	/// <para> false : �������Ă��Ȃ� </para> 
	/// </returns>
	bool Intersect(const AABB& aabb, const Sphere& sphere, Vec3& penetration);

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

} // namespace PokarinEngine

#endif // !COLLISION_H_INCLUDED
