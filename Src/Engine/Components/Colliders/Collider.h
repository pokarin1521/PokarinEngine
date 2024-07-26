/**
* @file Collider.h
*/
#ifndef COLLIDER_H_INCLUDED
#define COLLIDER_H_INCLUDED

#include "../Component.h"

#include "../../UsingNames/UsingCollider.h"
#include "../../UsingNames/UsingMesh.h"

#include "../../Collision/CollisionType.h"

#include "../../Math/Vector.h"
#include "../../Math/Matrix.h"

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

		/// <summary>
		/// �`��
		/// </summary>
		void Draw();

	public: // --------------- �}�`�̎�� -----------------

		/// <summary>
		/// �}�`�̎��
		/// </summary>
		enum class Type
		{
			Box,	// �L�����E�{�b�N�X
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
		/// <returns> ���W�ϊ������R���C�_�[ </returns>
		virtual ColliderPtr GetTransformedCollider() const = 0;

	public: // ------------- �R���C�_�[�̏�� -------------

		// true = �d��������
		// false = �d�����֎~
		bool isTrigger = false;

		// true = �����Ȃ�����
		// false = ��������
		bool isStatic = false;

		// �X�^�e�B�b�N���b�V��
		StaticMeshPtr staticMesh;

	//protected: // ------------ ���W�ϊ��s�� ---------------

	//	// ���W�ϊ��s��
	//	Matrix4x4 transformMatrix = Matrix4x4();

	//	// ���W�ϊ������R���C�_�[
	//	ColliderPtr transformedCollider;

	protected: // ------------- �G�f�B�^�p ----------------

		// �h���b�O����p�X���C�_�[�̕�
		const float sliderWidth = 90.0f;

	private: // ----------------- ������ ------------------

		/// <summary>
		/// �Q�[���I�u�W�F�N�g�ɒǉ����ꂽ���̏�����
		/// </summary>
		void Awake() override;

	private: // -------------- ���W�ϊ��s�� ---------------

		/// <summary>
		/// ���W�ϊ��s����擾����
		/// </summary>
		/// <returns> ���W�ϊ��s�� </returns>
		virtual Matrix4x4 GetTransformMatrix() const = 0;
	};

} // namespace PokarinEngine

#endif // !COLLIDER_H_INCLUDED
