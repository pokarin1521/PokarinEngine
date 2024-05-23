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
		~SphereCollider() = default;

	public: // -------------------- ���W ---------------------

		/// <summary>
		/// �R���C�_�[�̍��W��ύX����
		/// </summary>
		/// <param name="translate"> �ړ��� </param>
		void AddPosition(const Vector3& translate) override
		{
			sphere.position += translate;
		}

		/// <summary>
		/// ���W�ϊ������R���C�_�[���擾����
		/// </summary>
		/// <param name="transform"></param>
		/// <returns></returns>
		ColliderPtr GetTransformedCollider(
			const Matrix4x4& transform) const override;

	public: // ----------------- ���̎擾 ------------------

		/// <summary>
		/// �}�`�̎�ނ��擾����
		/// </summary>
		/// <returns> ���̂�\��Type </returns>
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

		/// <summary>
		/// �R���|�[�l���g�̖��O���擾����
		/// </summary>
		/// <returns> �R���|�[�l���g�̖��O </returns>
		const std::string& GetName() override
		{
			return name;
		}

	public: // -------------------- ��� ---------------------
	
		// �}�`(����)
		Collision::Sphere sphere = { Vector3(0), 1 };

		// �R���|�[�l���g�̖��O
		inline static const std::string name = "Sphere Collider";

	private: // ---------------- �G�f�B�^�p ------------------

		/// <summary>
		/// ����ҏW�ł���悤�ɕ\������
		/// </summary>
		void InfoEditor() override {}
	};

} // namespace PokarinEngine

#endif // !SPHERECOLLIDER_H_INCLUDED
