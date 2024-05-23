/**
* @file Transform.h
*/
#ifndef TRANSFORM_H_INCLUDED
#define TRANSFORM_H_INCLUDED

#include "Component.h"
#include "../Math/Vector.h"
#include "../Math/Matrix.h"

#include <vector>
#include <string>
#include <cmath>

namespace PokarinEngine
{
	// -----------------
	// �O���錾
	// -----------------

	class Transform;
	class GameObject;

	// ------------------------
	// �^�̕ʖ����`
	// ------------------------

	using TransformPtr = std::shared_ptr<Transform>;

	/// <summary>
	/// �ʒu�A��]�A�g�嗦��ێ�����R���|�[�l���g
	/// </summary>
	class Transform : public Component
	{
		// �Q�[���I�u�W�F�N�g�ɏ������J
		friend GameObject;

	public: // ---------------- �R���X�g���N�^�E�f�X�g���N�^ -----------------

		Transform() = default;
		~Transform() = default;

	public: // --------------------- �R���|�[�l���g�̐��� --------------------

		/// <summary>
		/// �X�V
		/// </summary>
		void Update() override;

		/// <summary>
		/// �Q�[���I�u�W�F�N�g���폜���ꂽ���̏���
		/// </summary>
		void OnDestroy() override;

	public: // ------------------ �e�I�u�W�F�N�g�̎擾�E�ݒ� -----------------

		/// <summary>
		/// �e�I�u�W�F�N�g���擾
		/// </summary>
		/// <returns> �e�I�u�W�F�N�g�̃g�����X�t�H�[�� </returns>
		Transform* GetParent() const
		{
			return parent;
		}

		/// <summary>
		/// �e�I�u�W�F�N�g��ݒ�
		/// </summary>
		/// <param name="parent"> 
		/// <para> �e�ɂ���Q�[���I�u�W�F�N�g�̃g�����X�t�H�[�� </para>
		/// <para> nullptr���w�肷��Ɛe�q�֌W���������� </para>
		/// </param>
		void SetParent(Transform* parent);

		/// <summary>
		/// �e�I�u�W�F�N�g��ݒ�
		/// </summary>
		/// <param name="parent"> 
		/// <para> �e�ɂ���Q�[���I�u�W�F�N�g�̃g�����X�t�H�[�� </para>
		/// <para> nullptr���w�肷��Ɛe�q�֌W���������� </para>
		/// </param>
		void SetParent(const TransformPtr& parent);

	public: // ---------------------- �q�I�u�W�F�N�g�̎擾 ----------------------

		/// <summary>
		/// �q�I�u�W�F�N�g�̐����擾����
		/// </summary>
		/// <returns> �q�I�u�W�F�N�g�̐� </returns>
		size_t GetChildCount() const
		{
			return children.size();
		}

		/// <summary>
		/// �q�I�u�W�F�N�g���擾����
		/// </summary>
		/// <param name="index"> �q�I�u�W�F�N�g�̗v�f�ԍ� </param>
		/// <returns> index�Ԃ̎q�I�u�W�F�N�g�̃g�����X�t�H�[�� </returns>
		Transform* GetChild(size_t index) const
		{
			return children[index];
		}

	public: // --------------------- �ϊ��s��̎擾�E�ݒ� ----------------------

		/// <summary>
		/// ���W�ϊ��s����擾����
		/// </summary>
		/// <returns> ���W�ϊ��s�� </returns>
		const Matrix4x4& GetTransformMatrix() const
		{
			return transformMatrix;
		}

		/// <summary>
		/// �@���ϊ��s����擾����
		/// </summary>
		/// <returns> �@���ϊ��s�� </returns>
		const Matrix3x3& GetNormalMatrix() const
		{
			return normalMatrix;
		}

	public: // ---------------------- �����x�N�g�� ---------------------

		/// <summary>
		/// ���ʃx�N�g�����擾����
		/// </summary>
		/// <returns></returns>
		inline Vector3 Forward() const
		{
			// ��]�p�x��Sin
			Vector2 rotationSin = { std::sin(rotation.x), std::sin(rotation.y) };

			// ��]�p�x��Cos
			Vector2 rotationCos = { std::cos(rotation.x), std::cos(rotation.y) };

			return { -rotationSin.y, rotationSin.x, rotationCos.x * rotationCos.y };
		}

	public: // ----------------------- �G�f�B�^�p ----------------------

		/// <summary>
		/// ����ҏW�ł���悤�ɕ\������
		/// </summary>
		void InfoEditor() override;

	public: // ----------------------- ���O�̎擾 ----------------------

		/// <summary>
		/// �R���|�[�l���g�̖��O���擾����
		/// </summary>
		/// <returns> �R���|�[�l���g�̖��O </returns>
		const std::string& GetName() override
		{
			return name;
		}

	public: // -------------------- ���[���h���̏�� -------------------

		// ���[���h���̍��W
		Vector3 position = { 0, 0, 0 };

		// ���[���h���̉�]�p�x(�ʓx�@)
		Vector3 rotation = { 0, 0, 0 };

		// �g�嗦
		Vector3 scale = { 1, 1, 1 };

	public: // -------------------------- ���O ------------------------

		// �R���|�[�l���g�̖��O
		inline static const std::string name = "Transform";

	private: // ----------------------- �ϊ��s�� -----------------------

		// ���W�ϊ��s��
		Matrix4x4 transformMatrix = Matrix4x4(0);

		// �@���ϊ��s��
		Matrix3x3 normalMatrix = Matrix3x3(0);

	private: // ------------------------- �e�q -------------------------

		/* �z�Q�Ƃ�h�����߂ɐ��|�C���^�ŊǗ����� */

		// �e�I�u�W�F�N�g�̃g�����X�t�H�[��
		Transform* parent = nullptr;

		// �q�I�u�W�F�N�g�̃g�����X�t�H�[��
		std::vector<Transform*> children;
	};

} // namespace PokarinEngine

#endif // !TRANSFORM_H_INCLUDED
