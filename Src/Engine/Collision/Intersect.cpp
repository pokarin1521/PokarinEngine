/**
* @file Intersect.cpp
*/
#include "Intersect.h"

#include <algorithm>

namespace PokarinEngine
{
	/// <summary>
	/// �Փ˗p
	/// </summary>
	namespace Collision
	{
		/// �����ł����g��Ȃ��̂ŁAcpp�݂̂ɏ���
		/// <summary>
		/// AABB����_�ւ̍ŋߐړ_�����߂�
		/// </summary>
		/// <param name="[in] aabb"> ����Ώۂ�AABB </param>
		/// <param name="[in] point"> ����Ώۂ̓_ </param>
		/// <returns> AABB�̒��ōł��_�ɋ߂����W </returns>
		Vector3 ClosestPoint(const AABB& aabb, const Vector3& point)
		{
			// AABB�̒��ōł��_�ɋ߂����W
			Vector3 closestPoint = Vector3(0);

			// X,Y,Z���W���ꂼ��ŋ��߂�
			for (int i = 0; i < Vector3::size; ++i)
			{
				// �͈͓��ɐ������邱�ƂŁA
				// AABB�̒��ōł��_�ɋ߂����W�����߂�
				closestPoint[i] = std::clamp(point[i], aabb.min[i], aabb.max[i]);
			}

			return closestPoint;
		}

		/// �����ł����g��Ȃ��̂ŁAcpp�݂̂ɏ���
		/// <summary>
		/// OBB����_�ւ̍ŋߐړ_�����߂�
		/// </summary>
		/// <param name="[in] box"> ����Ώۂ�OBB </param>
		/// <param name="[in] point"> ����Ώۂ̓_ </param>
		/// <returns> OBB�̒��ōł��_�ɋ߂����W </returns>
		Vector3 ClosestPoint(const Box& box, const Vector3& point)
		{
			// OBB�̒��S����_�Ɍ������x�N�g��
			const Vector3 vector = point - box.center;

			// OBB�̒��ōł��_�ɋ߂����W
			Vector3 closestPoint = box.center;

			// X,Y,Z�����ꂼ��ŋ��߂�
			for (int i = 0; i < Vector3::size; ++i)
			{
				/* OBB�̒��S����_�Ɍ������x�N�g����
				OBB��X,Y,Z���Ɏˉe���邱�ƂŁA
				���ꂼ��̎��̌����ɑΉ�����
				OBB�̒��S���狅�̂̒��S�܂ł̋��������߂�

				�ˉe�͓��ςŋ��߂� */

				// OBB��i���̌����ɑΉ�����OBB�̒��S����̋���
				float distance = Vector3::Dot(vector, box.axis[i]);

				// �ˉe�œ���ꂽ������OBB�͈͓̔��ɐ���
				distance = std::clamp(distance, -box.scale[i], box.scale[i]);

				// OBB�̒��S���W�Ƀx�N�g���𑫂��čŋߐړ_�����߂�
				closestPoint += distance * box.axis[i];
			}

			return closestPoint;
		}

		/// <summary>
		/// AABB���m�̌�������
		/// </summary>
		/// <param name="[in] a"> ����Ώۂ�AABB(A) </param>
		/// <param name="[in] b"> ����Ώۂ�AABB(B) </param>
		/// <param name="[out] penetration"> A��B�ɑ΂���ђʃx�N�g�� </param>
		/// <returns> 
		/// <para> true : �������Ă��� </para> 
		/// <para> false : �������Ă��Ȃ� </para> 
		/// </returns>
		bool Intersect(const AABB& a, const AABB& b, Vector3& penetration)
		{
			// �ђʋ������~�����̂ŁA2�̒l�̍����Ō������Ă��邩���肷��
			// �ђʋ����͕ǂɂ߂荞�񂾂Ƃ��̏C���Ɏg��

			// a�̍����ʂ�b�̉E���ʂ��E�ɂ���Ȃ�A�������Ă��Ȃ�
			const float dx0 = b.max.x - a.min.x; // a�̍����ʂ���̊ђʋ���

			// �ђʋ�����0�����Ȃ�������Ă��Ȃ�
			if (dx0 <= 0)
			{
				return false;
			}

			// a�̉E���ʂ�b�̍����ʂ�荶�ɂ���Ȃ�A�������Ă��Ȃ�
			const float dx1 = a.max.x - b.min.x; // a�̉E���ʂ���̊ђʋ���

			// �ђʋ�����0�����Ȃ�������Ă��Ȃ�
			if (dx1 <= 0)
			{
				return false;
			}

			// a�̉��ʂ�b�̏�ʂ���ɂ���Ȃ�A�������Ă��Ȃ�
			const float dy0 = b.max.y - a.min.y; // a�̉��ʂ���̊ђʋ���

			// �ђʋ�����0�����Ȃ�������Ă��Ȃ�
			if (dy0 <= 0)
			{
				return false;
			}

			// a�̏�ʂ�b�̉��ʂ�艺�ɂ���Ȃ�A�������Ă��Ȃ�
			const float dy1 = a.max.y - b.min.y; // a�̏�ʂ���̊ђʋ���

			// �ђʋ�����0�����Ȃ�������Ă��Ȃ�
			if (dy1 <= 0)
			{
				return false;
			}

			// a�̉����ʂ�b�̎�O���ʂ���O�ɂ���Ȃ�A�������Ă��Ȃ�
			const float dz0 = b.max.z - a.min.z; // a�̉����ʂ���̊ђʋ���

			// �ђʋ�����0�����Ȃ�������Ă��Ȃ�
			if (dz0 <= 0)
			{
				return false;
			}

			// a�̎�O���ʂ�b�̉����ʂ�艜�ɂ���Ȃ�A�������Ă��Ȃ�
			const float dz1 = a.max.z - b.min.z; // a�̎�O���ʂ���̊ђʋ���

			// �ђʋ�����0�����Ȃ�������Ă��Ȃ�
			if (dz1 <= 0)
			{
				return false;
			}

			// ���̎��_�Ō������m��

			// �ǂɂ߂荞�񂾕��C�����邽�߂ɁA�ђʋ����Ɗђʕ��������߂�
			// �ђʋ����͌�������ŋ��߂����̂��g��
			// �ђʕ����́A�ђʋ������ł��Z��������I�юg�p����

			Vector3 length = { dx1,dy1,dz1 }; // �ђʋ����̐�Βl
			Vector3 signedLength = length; // �����t���̊ђʋ���

			// XYZ�̊e���ɂ��āA�ђʋ������Z��������I��

			// ������̊ђʋ����ƁA�E����̊ђʋ����ŒZ��������I��
			if (dx0 < dx1)
			{
				length.x = dx0;

				// ������\�����߂ɕ���������
				signedLength.x = -dx0;
			}

			// ������̊ђʋ����ƁA�ォ��̊ђʋ����ŒZ��������I��
			if (dy0 < dy1)
			{
				length.y = dy0;

				// ������\�����߂ɕ���������
				signedLength.y = -dy0;
			}

			// ������̊ђʋ����ƁA��O����̊ђʋ����ŒZ��������I��
			if (dz0 < dz1)
			{
				length.z = dz0;

				// ������\�����߂ɕ���������
				signedLength.z = -dz0;
			}

			// XYZ�̂����ł��Z��������I��

			// X���������ł��Z���ꍇ
			if (length.x < length.y)
			{
				if (length.x < length.z)
				{
					// �ђʃx�N�g����X����I��
					penetration = { signedLength.x, 0, 0 };
					return true;
				}
			}

			// Y���������ł��Z���ꍇ
			else if (length.y < length.z)
			{
				// �ђʃx�N�g����Y����I��
				penetration = { 0, signedLength.y, 0 };
				return true;
			}

			// Z���������ł��Z���ꍇ
			// �ђʃx�N�g����Z����I��
			penetration = { 0, 0, signedLength.z };
			return true;
		}

		/// <summary>
		/// ���̓��m�̌�������
		/// </summary>
		/// <param name="[in] a"> ����Ώۂ̋���A </param>
		/// <param name="[in] b"> ����Ώۂ̋���B </param>
		/// <param name="[out] penetration"> A��B�ɑ΂���ђʃx�N�g�� </param>
		/// <returns> 
		/// <para> true : �������Ă��� </para> 
		/// <para> false : �������Ă��Ȃ� </para> 
		/// </returns>
		bool Intersect(const Sphere& a, const Sphere& b, Vector3& penetration)
		{
			// ---------------------------------
			// ���S�̊Ԃ̋�����2����v�Z
			// ---------------------------------

			// a�̒��S����b�̒��S�܂ł̃x�N�g��
			const Vector3 vector = b.center - a.center;

			// a�`b�܂ł̋�����2��
			// (������������邽�߁A2�擯�m�̔�r�Ɏg��)
			const float sprDistance = vector.SqrMagnitude();

			//-------------------------------------
			// ����(2��)�����a�̍��v��蒷���ꍇ��
			// �������Ă��Ȃ�
			// ------------------------------------

			// a��b�̔��a�̍��v
			const float totalRadius = a.radius + b.radius;

			// ������������邽�߁A2�擯�m�Ŕ�r����
			if (sprDistance > totalRadius * totalRadius)
			{
				return false;
			}

			// ------------------------------
			// �������Ă���̂�
			// �ђʃx�N�g�������߂�
			// ------------------------------

			// a�`b�܂ł̋���
			const float distance = sqrt(sprDistance);

			// �u���a�̍��v - a�`b�܂ł̋����v�́u�����ɑ΂���䗦�v�����߂�
			const float ratio = (totalRadius - distance) / distance;

			// �ђʃx�N�g�����v�Z
			penetration = vector * ratio;

			return true;
		}

		/// <summary>
		/// AABB�Ƌ��̂̌�������
		/// </summary>
		/// <param name="[in] aabb"> ����Ώۂ�AABB </param>
		/// <param name="[in] sphere"> ����Ώۂ̋��� </param>
		/// <param name="[out] penetration"> AABB�̋��̂ɑ΂���ђʃx�N�g�� </param>
		/// <returns> 
		/// <para> true : �������Ă��� </para> 
		/// <para> false : �������Ă��Ȃ� </para> 
		/// </returns>
		bool Intersect(const AABB& aabb, const Sphere& sphere, Vector3& penetration)
		{
			// --------------------------------------------------
			// �ŋߐړ_�܂ł̋��������̂̔��a��蒷����΁A
			// �������Ă��Ȃ�
			// --------------------------------------------------

			// AABB���狅�̂̒��S�ւ̍ŋߐړ_
			const Vector3 point = ClosestPoint(aabb, sphere.center);

			// �ŋߐړ_���狅�̂̒��S�܂ł̃x�N�g��
			const Vector3 vector = sphere.center - point;

			// �ŋߐړ_���狅�̂̒��S�܂ł̋�����2��
			const float sqrDistance = vector.SqrMagnitude();

			// ������������邽�߁A2�擯�m�Ŕ�r
			if (sqrDistance > sphere.radius * sphere.radius)
			{
				return false;
			}

			// ----------------------------------------------
			// �������Ă���̂ŁA�ђʃx�N�g�������߂�
			// ----------------------------------------------

			// ���̂̒��S��AABB�̊O���ɂ���̂�
			// ���̂̒��S����ŋߐړ_�֌�������������Փ˂����Ƃ݂Ȃ�
			if (sqrDistance > 0)
			{
				// �ŋߐړ_���狅�̂̒��S�܂ŋ���
				const float distance = sqrt(sqrDistance);

				// �u���a - �ړ_���璆�S�܂ł̋����v�́u�����ɑ΂���䗦�v�����߂�
				const float ratio = (sphere.radius - distance) / distance;

				// �ړ_���璆�S�܂ł̃x�N�g���ɔ䗦�𔽉f����
				// �ђʃx�N�g�������߂�
				penetration = vector * ratio;
			}
			// ���̂̒��S��AABB�̓����ɂ���̂�
			// �ђʋ������ł��Z���ʂ���Փ˂����Ƃ݂Ȃ�
			else
			{
				// AABB�����̂Ɋђʂ��������������C���f�b�N�X
				// ����v���X�����A�������}�C�i�X����
				int faceIndex = 0;

				// AABB�����̂Ɋђʂ�������
				float penetrationDistance = FLT_MAX;

				// X,Y,Z���ꂼ��Ŕ�r���A
				// �ђʋ�������ԒZ�������𒲂ׂăC���f�b�N�X��ݒ肷��
				for (int i = 0; i < Vector3::size; ++i)
				{
					// AABB�̍ŏ����W���狅�̂̒��S�܂ł̋���
					float distance_sphere = point[i] - aabb.min[i];

					// �Z�������ђʋ����Ƃ��Đݒ肷��
					if (distance_sphere < penetrationDistance)
					{
						// �}�C�i�X�����Ɋђʂ��Ă���̂�
						// ������ݒ肷��
						faceIndex = i * 2;

						// �ђʋ������X�V
						penetrationDistance = distance_sphere;
					}

					// ���̂̒��S����AABB�̍ő���W�܂ł̋���
					float distance_aabb = aabb.max[i] - point[i];

					// �Z�������ђʋ����Ƃ��Đݒ肷��
					if (distance_aabb < penetrationDistance)
					{
						// �v���X�����Ɋђʂ��Ă���
						faceIndex = i * 2 + 1;

						// �ђʋ������X�V
						penetrationDistance = distance_aabb;
					}
				}

				// �uAABB�����̂ɑ΂��Ăǂꂾ���ђʂ��Ă��邩�v�������x�N�g��
				// �ʂ̊O�����̃x�N�g�����g��
				static const Vector3 faceNormals[] = {
					{ -1, 0, 0 }, { 1, 0, 0 }, // -X, +X
					{  0,-1, 0 }, { 0, 1, 0 }, // -Y, +Y
					{  0, 0,-1 }, { 0, 0, 1 }, // -Z, +Z
				};

				// �ђʋ�������ԒZ�������̊ђʃx�N�g�������߂�
				penetration = faceNormals[faceIndex] * penetrationDistance;
			}

			return true;
		}

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
		bool Intersect(const Box& box, const Sphere& sphere, Vector3& penetration)
		{
			// OBB���狅�̂̒��S�ւ̍ŋߐړ_
			const Vector3 point = ClosestPoint(box, sphere.center);

			// �ŋߐړ_���狅�̂̒��S�܂ł̃x�N�g��
			const Vector3 vector = sphere.center - point;

			// �ŋߐړ_���狅�̂̒��S�܂ł̋�����2��
			// ������������邽�߂�2��̒l���g��
			const float sqrDistance = vector.SqrMagnitude();

			// �ŋߐړ_���狅�̂̒��S�܂ł̋�����
			// ���̂̔��a���傫���ꍇ�́A�������Ă��Ȃ�
			if (sqrDistance > sphere.radius * sphere.radius)
			{
				return false;
			}

			// ������0���傫���ꍇ�A���̂̒��S��OBB�̊O���ɂ���
			// ���̏ꍇ�A�ŋߐړ_���狅�̂̒��S�֌�������������Փ˂����Ƃ݂Ȃ�
			if (sqrDistance > 0)
			{
				// �ŋߐړ_���狅�̂̒��S�܂ŋ���
				const float distance = sqrt(sqrDistance);

				// �ђʋ���
				const float penetrationDistance = (sphere.radius - distance);

				// �ŋߐړ_���狅�̂̒��S�ւ̒P�ʃx�N�g��
				const Vector3 normalize = vector / distance;

				// �ŋߐړ_���狅�̂̒��S�֌�������������Փ˂����Ƃ݂Ȃ��̂ŁA
				// �P�ʃx�N�g���ɋ����𔽉f���邱�ƂŁA�ђʃx�N�g�������߂�
				penetration = normalize * penetrationDistance;
			}
			else
			{
				// ������0�̏ꍇ�A���̂̒��S��OBB�̓����ɂ���
				// ���̏ꍇ�A�ђʋ������ł��Z���ʂ���Փ˂����Ƃ݂Ȃ�

				// OBB�̒��S���狅�̂̒��S�ւ̃x�N�g��
				const Vector3 obbVector = sphere.center - box.center;

				// �ђʕ����������C���f�b�N�X
				int faceIndex = 0;


			}

			return true;
		}
	}
}