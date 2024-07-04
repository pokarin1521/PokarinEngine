/**
* @file SphereCollider.h
*/
#ifndef SPHERECOLLIDER_H_INCLUDED
#define SPHERECOLLIDER_H_INCLUDED

#include "Collider.h"

namespace PokarinEngine
{
	/// <summary>
	/// ���̃R���C�_�[
	/// </summary>
	class SphereCollider : public Collider
	{
	public: // -------- �R���X�g���N�^�E�f�X�g���N�^ ---------

		SphereCollider() = default;
		~SphereCollider() = default;

	public: // -------------------- ���W ---------------------

		/// <summary>
		/// �R���C�_�[���ړ�������
		/// </summary>
		/// <param name="[in] translate"> �ړ��� </param>
		void AddPosition(const Vector3& translate) override
		{
			sphere.center += translate;
		}

		/// <summary>
		/// ���W�ϊ������R���C�_�[���擾����
		/// </summary>
		/// <param name="[in] transform"> ���W�ϊ��s�� </param>
		/// <returns> ���W�ϊ������R���C�_�[ </returns>
		ColliderPtr GetTransformedCollider(
			const Matrix4x4& transform) const override;

	public: // ----------------- ���̎擾 ------------------

		/// <summary>
		/// �}�`�̎�ނ��擾����
		/// </summary>
		/// <returns> �}�`�̎�� </returns>
		Type GetType() const override
		{
			return Type::Sphere;
		}

		/// <summary>
		/// �}�`���擾����
		/// </summary>
		/// <returns> ���� </returns>
		const Collision::Sphere& GetShape() const
		{
			return sphere;
		}

	public: // -------------------- ��� ---------------------

		// �}�`(����)
		Collision::Sphere sphere;

	private: // ----------------- �G�f�B�^�p -----------------

		/// <summary>
		/// ����ҏW�ł���悤�ɕ\������
		/// </summary>
		void InfoEditor() override;
	};

} // namespace PokarinEngine

#endif // !SPHERECOLLIDER_H_INCLUDED
