/**
* @file CollisionType.h
*/
#ifndef COLLISIONTYPE_H_INCLUDED
#define COLLISIONTYPE_H_INCLUDED

#include "../Math/Vector.h"

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
			// AABB�̍ŏ����W
			Vector3 min = Vector3(-1);

			// AABB�̍ő���W
			Vector3 max = Vector3(1);

			// ���S���W
			Vector3 center = Vector3(0);

			// ���S����̑傫��
			Vector3 size = Vector3(1);
		};

		/// <summary>
		/// ����
		/// </summary>
		struct Sphere
		{
			// ���S���W
			Vector3 center = Vector3(0);

			// ���a
			float radius = 0.5f;
		};

		/// <summary>
		/// <para> �L�����E�{�b�N�X(OBB) </para>
		/// <para> (���̂̉�]�ɑΉ�����������) </para>
		/// </summary>
		struct Box
		{
			// ���S���W
			Vector3 center = Vector3(0);

			// ���̌����x�N�g��
			Vector3 axis[Vector3::size] = { Vector3(0) };

			// �������̑傫��
			Vector3 scale = Vector3(0);
		};

	} // namespace Collision

} // namespace PokarinEngine

#endif // !COLLISIONTYPE_H_INCLUDED
