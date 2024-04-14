/**
* @file Collider.h
*/
#ifndef COLLIDER_H_INCLUDED
#define COLLIDER_H_INCLUDED

#include "../Component.h"
#include "../../Math/Vector.h"
#include "../../Math/Matrix.h"

#include <memory>

namespace PokarinEngine
{
	// ---------------
	// �O���錾
	// ---------------

	class Collider;
	using ColliderPtr = std::shared_ptr<Collider>;

	/// <summary>
	/// �R���C�_�[�̊��N���X
	/// </summary>
	class Collider : public Component
	{
	public: // ------- �R���X�g���N�^�E�f�X�g���N�^ -------

		Collider() = default;
		virtual ~Collider() = default;

	public: // --------------- �}�`�̎�� -----------------

		/// <summary>
		/// �}�`�̎��
		/// </summary>
		enum class Type
		{
			AABB,   // �����s���E�{�b�N�X
			Sphere, // ����
		};

	public: // ---------- �R���C�_�[�̋@�\ ----------

		/// <summary>
		/// �}�`�̎�ނ��擾
		/// </summary>
		virtual Type GetType() const = 0;

		/// <summary>
		/// �R���C�_�[�̍��W��ύX����
		/// </summary>
		virtual void AddPosition(const Vector3& translate) = 0;

		/// <summary>
		/// ���W�ϊ������R���C�_�[���擾����
		/// </summary>
		virtual ColliderPtr GetTransformedCollider(
			const Matrix4x4& transform) const = 0;

		/// <summary>
		/// �G�f�B�^�`��p
		/// </summary>
		virtual void RenderInfo() override {}

	public: // ----------- �R���C�_�[�̏�� -----------

		// true = �d��������
		// false = �d�����֎~
		bool isTrigger = false;

		// true = �����Ȃ�����
		// false = ��������
		bool isStatic = false;
	};

} // namespace PokarinEngine

#endif // !COLLIDER_H_INCLUDED
