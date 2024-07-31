/**
* @file CollisionType.h
*/
#ifndef POKARINENGINE_COLLISIONTYPE_H_INCLUDED
#define POKARINENGINE_COLLISIONTYPE_H_INCLUDED

#include "../Math/Vector.h"

namespace PokarinEngine
{
	/// <summary>
	/// �Փ˗p
	/// </summary>
	namespace Collision
	{
		/// <summary>
		/// <para> �L�����E�{�b�N�X(OBB) </para>
		/// <para> (���̂̉�]�ɑΉ�����������) </para>
		/// </summary>
		struct Box
		{
			// ���S���W
			Vector3 center = Vector3::zero;

			// ���̌����x�N�g��
			Vector3 axis[Vector3::size] = {
				{ 1, 0, 0 },{ 0, 1, 0 }, { 0, 0, 1 } };

			// �傫��
			Vector3 size = Vector3::one;
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

	} // namespace Collision

} // namespace PokarinEngine

#endif // !POKARINENGINE_COLLISIONTYPE_H_INCLUDED
