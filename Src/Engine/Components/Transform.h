/**
* @file Transform.h
*/
#ifndef TRANSFORM_H_INCLUDED
#define TRANSFORM_H_INCLUDED

#include "Component.h"
#include "../VecMath.h"

#include <vector>
#include <string>

namespace PokarinEngine
{
	// -----------------
	// �O���錾
	// -----------------

	class Transform;

	// ------------------------
	// �^�̕ʖ����`
	// ------------------------

	using TransformPtr = std::shared_ptr<Transform>;

	/// <summary>
	/// �ʒu�A��]�A�g�嗦��ێ�����R���|�[�l���g
	/// </summary>
	class Transform : public Component
	{
	public: // ------- �R���X�g���N�^�E�f�X�g���N�^ --------

		Transform() = default;

		virtual ~Transform() = default;

	public: // ----------------- �֎~���� ------------------

		// �R�s�[�̋֎~
		Transform(Transform&) = delete;

		// ����̋֎~
		Transform& operator=(Transform&) = delete;

	public: // ----------- �R���|�[�l���g�̐��� ------------

		/// <summary>
		/// �Q�[���I�u�W�F�N�g���폜���ꂽ���̏���
		/// </summary>
		void OnDestroy() override;

	public: // ------------------- ��] --------------------

		/// <summary>
		/// point�𒆐S��Y����]
		/// </summary>
		/// <param name="point"> ���S�̈ʒu </param>
		/// <param name="rotY"> Y����]������p�x(�ʓx�@) </param>
		/// <param name="radius"> ���S����̋��� </param>
		void RotateAroundY(const Vec3& point, float rotY, float distance);

		/// <summary>
		/// �^�[�Q�b�g�̕��������悤��Y����]
		/// </summary>
		/// <param name="target"> �^�[�Q�b�g�̍��W </param>
		void LookAtY(const Vec3& target);

	public: // ---------- �e�I�u�W�F�N�g�̎擾�E�ݒ� ----------

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

	public: // ------------ �q�I�u�W�F�N�g�̎擾 ------------

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

	public: // ------------- �ϊ��s��̎擾�E�ݒ� -------------

		/// <summary>
		/// ���W�ϊ��s����擾����
		/// </summary>
		/// <returns> ���W�ϊ��s�� </returns>
		const Mat4& GetTransformMatrix() const
		{
			return transformMatrix;
		}

		/// <summary>
		/// �@���ϊ��s����擾����
		/// </summary>
		/// <returns> �@���ϊ��s�� </returns>
		const Mat3& GetNormalMatrix() const
		{
			return normalMatrix;
		}

		/// <summary>
		/// ���W�ϊ��s���ݒ肷��
		/// </summary>
		/// <param name="mat"> ���W�ϊ��s�� </param>
		void SetTransformMatrix(const Mat4& mat)
		{
			transformMatrix = mat;
		}

		/// <summary>
		/// �@���ϊ��s���ݒ肷��
		/// </summary>
		/// <param name="mat"> �@���ϊ��s�� </param>
		void SetNormalMatrix(const Mat3& mat)
		{
			normalMatrix = mat;
		}

	public: // ----------------- �G�f�B�^�p -----------------

		/// <summary>
		/// �G�f�B�^�ɏ���\������
		/// </summary>
		void RenderEditor() override;

	public: // -------------------- ��� --------------------

		// �ʒu
		Vec3 position = { 0, 0, 0 };

		// ��](�ʓx�@)
		Vec3 rotation = { 0, 0, 0 };

		// �g�嗦
		Vec3 scale = { 1, 1, 1 };

	private: // ---------------- �G�f�B�^�p ----------------

		// ��](�x���@)
		// �X���C�_�[�\���Ɏg��
		Vec3 rotationDeg = { 0, 0, 0 };

		bool isDrag = false;

	private: // ----------------- �ϊ��s�� -----------------

		// ���W�ϊ��s��
		Mat4 transformMatrix = Mat4(0);

		// �@���ϊ��s��
		Mat3 normalMatrix = Mat3(0);

	private: // ------------------- �e�q --------------------

		/* �z�Q�Ƃ�h�����߂ɐ��|�C���^�ŊǗ����� */

		// �e�I�u�W�F�N�g�̃g�����X�t�H�[��
		Transform* parent = nullptr;

		// �q�I�u�W�F�N�g�̃g�����X�t�H�[��
		std::vector<Transform*> children;
	};

} // namespace PokarinEngine

#endif // !TRANSFORM_H_INCLUDED
