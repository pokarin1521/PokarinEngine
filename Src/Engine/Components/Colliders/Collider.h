/**
* @file Collider.h
*/
#ifndef COLLIDER_H_INCLUDED
#define COLLIDER_H_INCLUDED

#include "../Component.h"

#include "../../Collision/CollisionType.h"

#include "../../Math/Vector.h"
#include "../../Math/Matrix.h"

#include "../../UsingNames/UsingCollider.h"

#include <memory>

namespace PokarinEngine
{
	/// <summary>
	/// �R���C�_�[�̊��N���X
	/// </summary>
	class Collider : public Component
	{
	public: // ------- �R���X�g���N�^�E�f�X�g���N�^ -------

		Collider() = default;
		virtual ~Collider() = default;

	public: // ------------------ ���� --------------------

		/// <summary>
		/// �X�V
		/// </summary>
		void Update() override;

	public: // --------------- �}�`�̎�� -----------------

		/// <summary>
		/// �}�`�̎��
		/// </summary>
		enum class Type
		{
			AABB,   // �����s���E�{�b�N�X
			Sphere, // ����
		};

	public: // ------------- �R���C�_�[�̋@�\ -------------

		/// <summary>
		/// �}�`�̎�ނ��擾����
		/// </summary>
		virtual Type GetType() const = 0;

		/// <summary>
		/// �R���C�_�[���ړ�������
		/// </summary>
		/// <param name="[in] translate"> �ړ��� </param> 
		virtual void AddPosition(const Vector3& translate) = 0;

		/// <summary>
		/// ���W�ϊ������R���C�_�[���擾����
		/// </summary>
		/// <param name="[in] transform"> ���W�ϊ��s�� </param>
		/// <returns> ���W�ϊ������R���C�_�[ </returns>
		virtual ColliderPtr GetTransformedCollider(
			const Matrix4x4& transform) const = 0;

	public: // ------------- �R���C�_�[�̏�� -------------

		// true = �d��������
		// false = �d�����֎~
		bool isTrigger = false;

		// true = �����Ȃ�����
		// false = ��������
		bool isStatic = false;

	protected: // ------------- �G�f�B�^�p ----------------

		// �h���b�O����p�X���C�_�[�̕�
		const float sliderWidth = 90.0f;

	private: // ------------------ �ۑ� -------------------

		/// <summary>
		/// �R���|�[�l���g�̏���ۑ�����
		/// </summary>
		/// <param name="[in] folderName"> �ۑ���̃t�H���_ </param>
		void SaveInfo(const std::string& folderName) const override {}
	};

} // namespace PokarinEngine

#endif // !COLLIDER_H_INCLUDED
