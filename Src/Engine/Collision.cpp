/**
* @file Collision.cpp
*/
#include "Collision.h"

#include <algorithm>

namespace PokarinEngine
{
	/// �����ł����g��Ȃ��̂ŁAcpp�݂̂ɏ���
	/// <summary>
	/// �X���u(���鎲�ɐ����ȕ���)�ƌ����̌�������
	/// </summary>
	/// <param name="[in] min"> �X���u�̊J�n�ʒu </param>
	/// <param name="[in] max"> �X���u�̏I���ʒu </param>
	/// <param name="[in] start"> �����̔��˓_ </param>
	/// <param name="[in] direction"> �����̌��� </param>
	/// <param name="[out] tmin"> AABB�ƌ����̌����J�n���� </param>
	/// <param name="[out] tmax"> AABB�ƌ����̌����I������ </param>
	/// <returns>
	/// <para> true : �������Ă��� </para>
	/// <para> false : �������Ă��Ȃ� </para>
	/// </returns>
	bool IntersectSlab(float min, float max, float start, float direction,
		float& tmin, float& tmax)
	{
		// ----------------------------
		// �����ƃX���u�����s���m�F
		// ----------------------------

		/* �����ƃX���u�����s�ȏꍇ�A
		��������0�ɂȂ邽�ߎ��Ԃ����߂邱�Ƃ��ł��Ȃ�
		���̏ꍇ�́u���˓_���X���u���ɂ��邩�ǂ����v�Ŕ��肷��*/

		// �������X���u�ƕ��s�ȏꍇ
		// ���˓_���X���u���ɂ���Ό������Ă���A�O�ɂ���Ό������Ă��Ȃ�
		if (abs(direction) < 0.0001f)
		{
			return (start >= min) && (start <= max);
		}

		// ----------------------------------------------------
		// �����ƃX���u����������J�n���ԂƏI�����Ԃ����߂�
		// ----------------------------------------------------

		/* �����̌����x�N�g����
		�u������X,Y,Z�̊e�������ɐi�ޑ��x�v�ƍl������

		���̂��߁A���鎲�ɂ�����u���˓_����X���u�܂ł̋����v��
		�����x�N�g���̓����������Ŋ��邱�ƂŁA
		�u�����J�n����(�������o�Ă���������n�߂�܂ł̎���)�v��
		�u�����I������(�������o�Ă���������I���܂ł̎���)�v�����߂��� */

		// �����ƃX���u����������J�n����
		// (�������o�Ă���������n�߂�܂ł̎���)
		float t0 = (min - start) / direction;

		// �����ƃX���u����������I������
		// (�������o�Ă���������I���܂ł̎���)
		float t1 = (max - start) / direction;

		// ���Ԃ̑��������J�n���ԂƂ���
		if (t0 > t1)
		{
			// t0��t1�����ւ�
			std::swap(t0, t1);
		}

		// -------------------------
		// ���ʂ̌����͈͂����߂�
		// -------------------------

		/* ������X�X���u�ƌ������Ă���Œ��ɁA
		Y�X���u�ƌ��������ꍇ�A

		X : �����J�n���� <------------------> �����I������
		Y :          �����J�n���� <=================> �����I������

		�ƂȂ邩

		X : �����J�n���� <---------------------------------> �����I������
		Y :          �����J�n���� <=================> �����I������

		�ƂȂ�

		���̂Ƃ��A���ʂ̌����͈͂�
		�u�x�����̌����J�n���� �` �������̌����I�����ԁv�ƂȂ� */

		// �ȑO�̌����J�n���Ԃƍ���̌����J�n���Ԃ��r���A
		// �x������I��
		if (t0 > tmin)
		{
			tmin = t0;
		}

		// �ȑO�̌����I�����Ԃƍ���̌����I�����Ԃ��r���A
		// ��������I��
		if (t1 < tmax)
		{
			tmax = t1;
		}

		// ------------
		// ��������
		// ------------

		// �u�����J�n���� <= �����I�����ԁv�̏ꍇ�͌������Ă���
		// ����ȊO�́u���ʂ̌����͈́v���Ȃ����ƂɂȂ�
		return tmin <= tmax;
	}

	/// �����ł����g��Ȃ��̂ŁAcpp�݂̂ɏ���
	/// <summary>
	/// AABB����_�ւ̍ŋߐړ_
	/// </summary>
	/// <param name="aabb"> ����Ώۂ�AABB </param>
	/// <param name="point"> ����Ώۂ̓_ </param>
	/// <returns> AABB�̒��ōł��_�ɋ߂����W </returns>
	Vec3 ClosestPoint(const AABB& aabb, const Vec3& point)
	{
		Vec3 result = Vec3(0);

		// �͈͓��ɐ������邱�ƂŁA
		// AABB�̒��ōł��_�ɋ߂����W�����߂�
		for (int i = 0; i < 3; ++i)
		{
			result[i] = std::clamp(point[i], aabb.min[i], aabb.max[i]);
		}

		return result;
	}

	/// <summary>
	/// AABB���m�̌�������
	/// </summary>
	/// <param name="[in] a"> ����Ώۂ�AABB(�Փ˂��鑤) </param>
	/// <param name="[in] b"> ����Ώۂ�AABB(�Փ˂���鑤) </param>
	/// <param name="[out] penetration"> A��B�ɑ΂���ђʃx�N�g�� </param>
	/// <returns> 
	/// <para> true : �������Ă��� </para> 
	/// <para> false : �������Ă��Ȃ� </para> 
	/// </returns>
	bool Intersect(const AABB& a, const AABB& b, Vec3& penetration)
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

		Vec3 length = { dx1,dy1,dz1 }; // �ђʋ����̐�Βl
		Vec3 signedLength = length; // �����t���̊ђʋ���

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
	bool Intersect(const Sphere& a, const Sphere& b, Vec3& penetration)
	{
		// ---------------------------------
		// ���S�̊Ԃ̋�����2����v�Z
		// ---------------------------------

		// a�̒��S����b�̒��S�܂ł̃x�N�g��
		const Vec3 vector = b.position - a.position;

		// a�`b�܂ł̋�����2��
		// (������������邽�߁A2�擯�m�̔�r�Ɏg��)
		const float distance2 = Vec3_Function::Dot(vector, vector);

		//-------------------------------------
		// d2�����a�̍��v��蒷���ꍇ��
		// �������Ă��Ȃ�
		// ------------------------------------

		// a��b�̔��a�̍��v
		const float totalRadius = a.radius + b.radius;

		// ������������邽�߁A2�擯�m�Ŕ�r����
		if (distance2 > totalRadius * totalRadius)
		{
			return false;
		}

		// ------------------------------
		// �������Ă���̂�
		// �ђʃx�N�g�������߂�
		// ------------------------------

		// a�`b�܂ł̋���
		const float distance = sqrt(distance2);

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
	bool Intersect(const AABB& aabb, const Sphere& sphere, Vec3& penetration)
	{
		// --------------------------------------------------
		// �ŋߐړ_�܂ł̋��������̂̔��a��蒷����΁A
		// �������Ă��Ȃ�
		// --------------------------------------------------

		// ���̂̒��S����̍ŋߐړ_
		const Vec3 point = ClosestPoint(aabb, sphere.position);

		// �ŋߐړ_���狅�̂̒��S�܂ł̃x�N�g��
		const Vec3 vector = sphere.position - point;

		// �ŋߐړ_���狅�̂̒��S�܂ł̋�����2��
		const float distance2 = Vec3_Function::Dot(vector, vector);

		// ������������邽�߁A2�擯�m�Ŕ�r
		if (distance2 > sphere.radius * sphere.radius)
		{
			return false;
		}

		// ----------------------------------------------
		// �������Ă���̂ŁA�ђʃx�N�g�������߂�
		// ----------------------------------------------

		// ���̂̒��S��AABB�̊O���ɂ���
		// ���̂̒��S����ŋߐړ_�֌�������������Փ˂����Ƃ݂Ȃ�
		if (distance2 > 0)
		{
			// �ŋߐړ_���狅�̂̒��S�܂ŋ���
			const float distance = sqrt(distance2);

			// �ђʃx�N�g�������߂�
			penetration = vector * ((sphere.radius - distance) / distance);
		}
		// ���̂̒��S��AABB�̓����ɂ���
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
			for (int i = 0; i < Info_Vec3::count; ++i)
			{
				// AABB�̍ŏ����W���狅�̂̒��S�܂ł̋���
				float d0 = point[i] - aabb.min[i];

				// �Z�������ђʋ����Ƃ��Đݒ肷��
				if (d0 < penetrationDistance)
				{
					// �}�C�i�X�����Ɋђʂ��Ă���
					faceIndex = i * 2;

					// �ђʋ������X�V
					penetrationDistance = d0;
				}

				// AABB�̍ő���W���狅�̂̒��S�܂ł̋���
				float d1 = aabb.max[i] - point[i];

				// �Z�������ђʋ����Ƃ��Đݒ肷��
				if (d1 < penetrationDistance)
				{
					// �v���X�����Ɋђʂ��Ă���
					faceIndex = i * 2 + 1;

					// �ђʋ������X�V
					penetrationDistance = d1;
				}
			}

			// �uAABB�����̂ɑ΂��Ăǂꂾ���ђʂ��Ă��邩�v�������x�N�g��
			// �ʂ̊O�����̃x�N�g�����g��
			static const Vec3 faceNormals[] = {
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
	/// AABB�ƌ����̌�������
	/// </summary>
	/// <param name="[in] aabb"> ����Ώۂ�AABB </param>
	/// <param name="[in] ray"> ����Ώۂ̌��� </param>
	/// <param name="[out] distance"> ������AABB�ƍŏ��Ɍ�������܂ł̋��� </param>
	/// <returns>
	/// <para> true : �������Ă��� </para>
	/// <para> false : �������Ă��Ȃ� </para>
	/// </returns>
	bool Intersect(const AABB& aabb, const Ray& ray, float& distance)
	{
		// ------------------------------
		// ���ʂ̌����͈�
		// �ŏ���0 �` �ő�l�ɂ��Ă���
		// ------------------------------

		// �����J�n����
		float tmin = 0;

		// �����I������
		float tmax = FLT_MAX;

		// --------------------------------
		// �e���̃X���u�ƌ����̌�������
		// --------------------------------

		/* ������AABB�ƌ�������Ƃ����̂́A
		�������e���̃X���u�Ɠ����Ɍ�������Ƃ�������

		IntersectSlab�֐��Łu���ʂ̌����͈́v������
		�����Ɍ������Ă��邩�m�F���� */

		// X�X���u�ƌ����̌�������
		if (!IntersectSlab(aabb.min.x, aabb.max.x,
			ray.start.x, ray.direction.x, tmin, tmax))
		{
			// �������Ă��Ȃ�
			return false;
		}

		// Y�X���u�ƌ����̌�������
		if (!IntersectSlab(aabb.min.y, aabb.max.y,
			ray.start.y, ray.direction.y, tmin, tmax))
		{
			// �������Ă��Ȃ�
			return false;
		}

		// Z�X���u�ƌ����̌�������
		if (!IntersectSlab(aabb.min.z, aabb.max.z,
			ray.start.z, ray.direction.z, tmin, tmax))
		{
			// �������Ă��Ȃ�
			return false;
		}

		// --------------------------------
		// ��_�܂ł̋�����ݒ肵�A
		// �������Ă��邱�Ƃ��m�肳����
		// --------------------------------

		// �������n�߂�܂ł̎��Ԃ��A
		// ���̂܂܌�_�܂ł̋����ɂȂ�

		// ��_�܂ł̋�����ݒ�
		// �����ƃR���C�_�[�̌����J�n���Ԃ�
		// ���̂܂܌�_�܂ł̋����ɂȂ�
		distance = tmin;

		// �������Ă���
		return true;
	}

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
	bool Intersect(const Sphere& sphere, const Ray& ray, float& distance)
	{
		/* ����(����)�̎�R�́A
		�u�n�_P�A����D�A������̔C�ӂ̈ʒu��\���ϐ�t�v�Œ�`�ł���

		R(t) = P + t * D

		���̂̎��́A
		�u���̂̒��SC�A���ar�A���̕\�ʂ̔C�ӂ̍��WX�v�Œ�`�ł���

		(X - C)�E(X - C) = r^2		(�E�͓���)

		���̂ƌ���������������W�́A
		X�Ɍ����̎�R(t)�������A���̂̎��𖞂����ϐ�t�����߂邱�ƂŌ�����

		m = P - C �Ƃ���
		(t * D + m)�E(t * D + m) = r^2

		�W�J����(D�͌����x�N�g���Ȃ̂Œ�����1�AD�ED = 1)

		t^2 + 2(m�ED)t + m�Em = r^2

		t�ȊO�̋L���͒萔�Ȃ̂ŁAt��2���������ɂȂ�
		���̌������g����t�����߂� (b = m�ED, c = m�Em - r^2)
		�������A�����ɂ͎n�_������̂ŁA
		t���n�_����O���w���ꍇ�����O���Ȃ��Ă͂Ȃ�Ȃ� */

		// ---------------------------------------------
		// �u���̂ƌ���������������Wt�v��
		// ���߂�̂ɕK�v�ȁum, b, c�v�����߂�
		// ---------------------------------------------

		// ���̂̒��S��������̎n�_�܂ł̃x�N�g��
		const Vec3 m = ray.start - sphere.position;

		// ���̂ɑ΂�������̕���(���ςŔ��f����)
		// �v���X   : ���̂��痣��Ă�������
		// �}�C�i�X : ���̂Ɍ������Ă�������
		const float b = Vec3_Function::Dot(m, ray.direction);

		//�u���̂̒��S��������̎n�_�܂ł̋����v�Ɓu���̂̔��a�v�̍�
		// �v���X   : �����̎n�_�����̂̊O�ɂ���
		// �}�C�i�X : �����̎n�_�����̂̒��ɂ���
		const float c = Vec3_Function::Dot(m, m) - sphere.radius * sphere.radius;

		// ------------------------------------------
		// ���̂ƌ�������������̂����肷��
		// ------------------------------------------

		// �����̎n�_�����̂̊O�ɂ���A
		// ���������̂��痣��Ă��������ɔ��˂��ꂽ�ꍇ�A
		if (c > 0 && b > 0)
		{
			// �������Ȃ�
			return false;
		}

		// --------------------------------------
		// �u���̂ƌ���������������Wt�v��
		// ���߂���̂����肷��
		// --------------------------------------

		// ���ʎ�
		const float d = b * b - c;

		// ���ʎ������̏ꍇ��
		// ���Ȃ�
		if (d < 0)
		{
			return false;
		}

		// ---------------------------------
		// �ŏ��Ɍ���������W���v�Z
		// ---------------------------------

		distance = -b - sqrt(d);

		// ---------------------------------------------
		// �����̎n�_����둤�Ō������Ă���ꍇ
		// 0�ɂ��āu�n�_ = ��_�v�Ƃ���
		// ---------------------------------------------

		// ���߂���_�����̏ꍇ�A�����̎n�_����O���w��
		// ���������̓����甭�˂��ꂽ���Ƃ��Ӗ�����
		// (�n�_�����̊O�ɂ���󋵂́A�������邩�̔���ŏ��O�ς�)
		if (distance < 0)
		{
			// �n�_ = ��_�Ƃ���
			distance = 0;
		}

		return true;
	}

} // namespace PokarinEngine