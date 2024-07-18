/**
* @file AabbCollider.h
*/
#ifndef AABBCOLLIDER_H_INCLUDED
#define AABBCOLLIDER_H_INCLUDED

#include "Collider.h"

#include "../../Debug.h"

namespace PokarinEngine
{
	// -----------------
	// �O���錾
	// -----------------

	class AabbCollider;

	// -----------------------
	// �^�̕ʖ����`
	// -----------------------

	using AabbColliderPtr = std::shared_ptr<AabbCollider>;

	/// <summary>
	/// AABB�R���C�_�[
	/// </summary>
	class AabbCollider : public Collider
	{
	public: // -------- �R���X�g���N�^�E�f�X�g���N�^ --------

		AabbCollider() = default;
		~AabbCollider() = default;

	public: // ------------------- ���W ---------------------

		/// <summary>
		/// �R���C�_�[���ړ�������
		/// </summary>
		/// <param name="[in] translate"> �ړ��� </param>
		void AddPosition(const Vector3& translate) override
		{
			aabb.min += translate;
			aabb.max += translate;
		}

		/// <summary>
		/// <para> ���W�ϊ������R���C�_�[���擾���� </para>
		/// <para> ��]�p�x��90�x�P�ʂŎw�肷�邱�� </para>
		/// <para> ����ȊO�̊p�x�ł͐������������肪�s���Ȃ� </para>
		/// </summary>
		/// <param name="[in] transform"> ���W�ϊ��s�� </param>
		/// <returns> ���W�ϊ������R���C�_�[ </returns>
		ColliderPtr GetTransformedCollider(
			const Matrix4x4& transform) const override;

	public: // ---------------- ���̎擾 ------------------

		/// <summary>
		/// �}�`�̎�ނ��擾����
		/// </summary>
		/// <returns> �}�`�̎�� </returns>
		Type GetType() const override
		{
			return Type::AABB;
		}

		/// <summary>
		/// �}�`���擾����
		/// </summary>
		/// <returns> �����s���E�{�b�N�X </returns>
		const Collision::AABB& GetShape() const
		{
			return aabb;
		}

	public: // ------------------- Json ---------------------

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

	public: // ------------------- ��� ---------------------

		// �}�`(�����s���E�{�b�N�X)
		Collision::AABB aabb;

	private: // ---------------- �G�f�B�^�p -----------------

		/// <summary>
		/// ����ҏW�ł���悤�ɕ\������
		/// </summary>
		void InfoEditor() override;
	};

} // namespace PokarinEngine

#endif // !AABBCOLLIDER_H_INCLUDED
