/**
* @file SphereCollider.h
*/
#ifndef SPHERECOLLIDER_H_INCLUDED
#define SPHERECOLLIDER_H_INCLUDED

#include "Collider.h"
#include "../../Collision.h"

namespace PokarinEngine
{
	/// <summary>
	/// ���̃R���C�_�[
	/// </summary>
	class SphereCollider : public Collider
	{
	public: // -------- �R���X�g���N�^�E�f�X�g���N�^ ---------

		SphereCollider() = default;
		virtual ~SphereCollider() = default;

	public: // -------------- �R���C�_�[�̋@�\ ---------------

		/// <summary>
		/// �}�`�̎�ނ��擾����
		/// </summary>
		/// <returns> ���̂�\��Type </returns>
		Type GetType() const override
		{
			return Type::Sphere;
		}

		/// <summary>
		/// �R���C�_�[�̍��W��ύX����
		/// </summary>
		/// <param name="translate"> �ړ��� </param>
		void AddPosition(const Vec3& translate) override
		{
			sphere.position += translate;
		}

		/// <summary>
		/// ���W�ϊ������R���C�_�[���擾����
		/// </summary>
		/// <param name="transform"></param>
		/// <returns></returns>
		ColliderPtr GetTransformedCollider(
			const Mat4& transform) const override
		{
			// -------------------------------
			// ���S���W�����W�ϊ�����
			// -------------------------------

			// ���W�ϊ��������̃R���C�_�[���i�[����ϐ�
			auto p = std::make_shared<SphereCollider>();

			// ���W�ϊ�
			p->sphere.position = Vec3(
				transform * Vec4(sphere.position, 1));

			// -------------------------------
			// ���̂̊g�嗦�𔽉f
			// -------------------------------

			/* ���̂̊g�嗦�́A
			XYZ���������Ȃ��Ă͂Ȃ�Ȃ�(�ȉ~�`�ɂȂ邩��)

			�I�u�W�F�N�g�̊g�嗦�͌ʂɎw��ł��Ă��܂��̂�
			�u���W�ϊ��s��Ɋ܂܂��XYZ�̊g�嗦�̂����ő�̒l�v��
			���̂̊g�嗦�Ƃ���(unity�̋��̃R���C�_�[�Ɠ�������) */

			// �g�嗦���擾
			const Vec3 scale = Mat4_Function::ExtractScale(transform);

			// X,Y,Z�̊g�嗦�ōő�̒l
			const float maxScale = std::max({ scale.x, scale.y, scale.z });

			// �g�嗦�𔽉f
			p->sphere.radius = sphere.radius * maxScale;

			// ���W�ϊ��������̃R���C�_�[
			return p;
		}

		/// <summary>
		/// �}�`���擾����
		/// </summary>
		/// <returns> ���� </returns>
		const Sphere& GetShape() const
		{
			return sphere;
		}

	public: // -------------- �R���C�_�[�̏�� ---------------

		// �}�`(����)
		Sphere sphere = { Vec3(0), 1 };
	};

} // namespace PokarinEngine

#endif // !SPHERECOLLIDER_H_INCLUDED
