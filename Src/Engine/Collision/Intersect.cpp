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

			// ���S����̑傫��
			const Vector3 centerSize = box.size / 2.0f;

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
				distance = std::clamp(distance, -centerSize[i], centerSize[i]);

				// OBB�̒��S���W�Ƀx�N�g���𑫂��čŋߐړ_�����߂�
				closestPoint += distance * box.axis[i];
			}

			return closestPoint;
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

			// ����A�̒��S���狅��B�̒��S�܂ł̃x�N�g��
			const Vector3 vector = b.center - a.center;

			// ����A�̒��S���狅��B�̒��S�܂ł̋�����2��
			// ������������邽�߁A2��̒l���g��
			const float sprDistance = vector.SqrMagnitude();

			//---------------------------------------
			// ���������a�̍��v��蒷���ꍇ��
			// �������Ă��Ȃ�
			// --------------------------------------

			// ����A�Ƌ���B�̔��a�̍��v
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

			// ����A�̒��S���狅��B�̒��S�܂ł̋���
			const float distance = sqrt(sprDistance);

			// ����A�̒��S���狅��B�̒��S�܂ł̒P�ʃx�N�g��(�����x�N�g��)
			const Vector3 normalize = vector / distance;

			// �ђʋ���
			const float penetrationDistance = totalRadius - distance;

			// �����x�N�g���Ɋђʋ����𔽉f�����āA�ђʃx�N�g�������߂�
			penetration = normalize * penetrationDistance;

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
				/* �ђʋ�����P�ʃx�N�g��(�����x�N�g��)�ɔ��f���邱�Ƃ�
				�ђʃx�N�g�������߂� */

				const float distance = sqrt(sqrDistance);

				// �ђʋ���
				const float penetrationDistance = (sphere.radius - distance);

				// �ŋߐړ_���狅�̂̒��S�ւ̒P�ʃx�N�g��(�����x�N�g��)
				const Vector3 normalize = vector / distance;

				// �ђʃx�N�g�������߂�
				penetration = normalize * penetrationDistance;
			}
			// ������0�̏ꍇ�A���̂̒��S��OBB�̓����ɂ���
			// ���̏ꍇ�A�ђʋ������ł��Z���ʂ���Փ˂����Ƃ݂Ȃ�
			else
			{
				/* �ђʋ������ł��Z���ʂ����߂�
				�����x�N�g���ɔ��f�����邱�ƂŁA�ђʃx�N�g�������߂�*/

				// OBB�̒��S���狅�̂̒��S�ւ̃x�N�g��
				const Vector3 obbVector = sphere.center - box.center;

				// OBB�̒��S����̑傫��
				const Vector3 centerSize = box.size / 2.0f;

				// �ђʕ���(��)�������C���f�b�N�X
				int faceIndex = 0;

				// ���̂̒��S��OBB�Ɋђʂ�������
				float distance = FLT_MAX;

				// �ђʃx�N�g���̕���
				float sign = 1;

				// �����ƂɁu���̂̒��S����OBB�̖ʂ܂ł̋����v�𒲂ׂ�
				// �Z�����̂��u���̂̒��S��OBB�Ɋђʂ��������v�Ƃ��č̗p����
				for (int i = 0; i < Vector3::size; ++i)
				{
					// OBB��i���̌����ɑΉ�����OBB�̒��S����̋���
					const float obbDistance = Vector3::Dot(obbVector, box.axis[i]);

					// �}�C�i�X�����̋��̂̒��S����OBB�̖ʂ܂ł̋���
					const float minus = obbDistance - (-centerSize[i]);

					// �������Z����΃}�C�i�X�����Ō��������Ɣ��f����
					if (minus < distance)
					{
						faceIndex = i;
						distance = minus;
						sign = -1;
					}

					// �v���X�����̋��̂̒��S����OBB�̖ʂ܂ł̋���
					const float plus = centerSize[i] - obbDistance;

					// �������Z����΃v���X�����Ō��������Ɣ��f����
					if (plus < distance)
					{
						faceIndex = i;
						distance = plus;
						sign = 1;
					}
				}

				// ���̂�OBB�Ɋђʂ�������
				// ������������悤�ɕ�����t����
				const float penetrationDistance = (distance * sphere.radius) * sign;

				// �����t���̊ђʋ����������x�N�g���ɔ��f�����邱�ƂŁA
				// �ђʃx�N�g�������߂�
				penetration = box.axis[faceIndex] * penetrationDistance;
			}

			return true;
		}
	}
}