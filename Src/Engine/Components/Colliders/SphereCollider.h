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
		/// <returns> ���W�ϊ������R���C�_�[ </returns>
		ColliderPtr GetTransformedCollider() const override;

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
		Collision::Sphere GetShape() const
		{
			return sphere;
		}

	public: // -------------------- Json ---------------------

		/// <summary>
		/// �R���|�[�l���g�̏���Json�^�Ɋi�[����
		/// </summary>
		/// <param name="[out] Json"> �����i�[����Json�^ </param>
		void ToJson(Json& data) const override {}

		/// <summary>
		/// �R���|�[�l���g�̏���Json�^����擾����
		/// </summary>
		/// <param name="[out] data"> �����i�[���Ă���Json�^ </param>
		void FromJson(const Json& data) override {}

	public: // -------------------- ��� ---------------------

		// �}�`(����)
		Collision::Sphere sphere;

	private: // ---------------- ���W�ϊ��s�� ----------------

		/// <summary>
		/// ���W�ϊ��s����擾����
		/// </summary>
		/// <returns> ���W�ϊ��s�� </returns>
		Matrix4x4 GetTransformMatrix() const;

	private: // ----------------- �G�f�B�^�p -----------------

		/// <summary>
		/// ����ҏW�ł���悤�ɕ\������
		/// </summary>
		void InfoEditor() override;
	};

} // namespace PokarinEngine

#endif // !SPHERECOLLIDER_H_INCLUDED
