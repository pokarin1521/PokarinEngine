/**
* @file AabbCollider.h
*/
#ifndef AABBCOLLIDER_H_INCLUDED
#define AABBCOLLIDER_H_INCLUDED

#include "Collider.h"
#include "../../Collision.h"

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
		virtual ~AabbCollider() = default;

	public: // ------------- �R���C�_�[�̋@�\ ---------------

		/// <summary>
		/// �}�`�̎�ނ��擾����
		/// </summary>
		/// <returns> �����s���E�{�b�N�X��\��Type </returns>
		Type GetType() const override
		{
			return Type::AABB;
		}

		/// <summary>
		/// �R���C�_�[�̍��W��ύX����
		/// </summary>
		/// <param name="translate"> �ړ��� </param>
		void AddPosition(const Vec3& translate) override
		{
			aabb.min += translate;
			aabb.max += translate;
		}

		/// <summary>
		/// <para> ���W�ϊ������R���C�_�[���擾���� </para>
		/// <para> ��]�p�x��90�x�P�ʂŎw�肷�邱�� </para>
		/// <para> ����ȊO�̊p�x�ł͐������������肪�s���Ȃ� </para>
		/// </summary>
		/// <param name="transform"> ���W�ϊ��s�� </param>
		/// <returns> ���W�ϊ������R���C�_�[ </returns>
		ColliderPtr GetTransformedCollider(
			const Mat4& transform) const override
		{
			// ------------------------------
			// ���W�ϊ������R�s�[���쐬
			// ------------------------------

			// ���W�ϊ�����AABB�R���C�_�[���i�[����ϐ�
			auto p = std::make_shared<AabbCollider>();

			// ���W�ϊ�
			p->aabb.min = Vec3(transform * Vec4(aabb.min, 1));
			p->aabb.max = Vec3(transform * Vec4(aabb.max, 1));

			// ---------------------------------------
			// min�̕����傫�����������ւ���
			// ---------------------------------------

			for (int i = 0; i < Info_Vec3::count; ++i)
			{
				if (p->aabb.min[i] > p->aabb.max[i])
				{
					const float tmp = p->aabb.min[i];
					p->aabb.min[i] = p->aabb.max[i];
					p->aabb.max[i] = tmp;
				}
			}

			// ���W�ϊ�����AABB�R���C�_�[
			return p;
		}

	public: // --------------- AABB�̎擾 -------------------

		/// <summary>
		/// �}�`���擾����
		/// </summary>
		/// <returns> �����s���E�{�b�N�X </returns>
		const AABB& GetShape() const
		{
			return aabb;
		}

	public: // ------------- �R���C�_�[�̏�� ---------------

		// �}�`(�����s���E�{�b�N�X)
		AABB aabb = { Vec3(-1), Vec3(1) };
	};

} // namespace PokarinEngine

#endif // !AABBCOLLIDER_H_INCLUDED
