/**
* @file Hierarchy.h
*/
#ifndef HIERARCHY_H_INCLUDED
#define HIERARCHY_H_INCLUDED

#include "../UsingNames/UsingGameObject.h"

#include <memory>
#include <vector>

namespace PokarinEngine
{
	// ---------------------
	// �O���錾
	// ---------------------

	class Engine;

	/// <summary>
	/// �q�G�����L�[(�V�[�����̃I�u�W�F�N�g�Ǘ��p�E�B���h�E)
	/// </summary>
	class Hierarchy
	{
	public: // ------------- �R���X�g���N�^�E�f�X�g���N�^ ----------------

		Hierarchy() = default;
		~Hierarchy() = default;

	public: // ------------------------- ���� ----------------------------

		/// <summary>
		/// ������
		/// </summary>
		void Initialize(Engine& e);

		/// <summary>
		/// �X�V
		/// </summary>
		void Update();

	public: // ---------------------- ���̎擾 -------------------------

		/// <summary>
		/// �I�𒆂̃I�u�W�F�N�g�̗v�f�ԍ����擾����
		/// </summary>
		/// <returns> �E�B���h�E���őI�𒆂̃I�u�W�F�N�g�̗v�f�ԍ� </returns>
		GameObjectPtr GetSelectObject()
		{
			return selectObject;
		}

	private: // ------------------ �E�B���h�E���̋@�\ --------------------

		/// <summary>
		/// �Q�[���I�u�W�F�N�g�\���p�c���[�m�[�h�̏���
		/// </summary>
		void ObjectTree();

		/// <summary>
		/// ���j���[�̏���
		/// </summary>
		void Mene();

		/// <summary>
		/// �I�𒆂̃I�u�W�F�N�g�ɑ΂��鐧����܂Ƃ߂��֐�
		/// </summary>
		void ScelectObjectControl();

	private: // --------------- �Q�[���I�u�W�F�N�g�쐬�p -----------------

		/// <summary>
		/// �Q�[���I�u�W�F�N�g�쐬�p�{�^���̏���
		/// </summary>
		/// <param name="kindName"> �쐬����Q�[���I�u�W�F�N�g�̎�� </param>
		/// <param name="staticMeshFile"> �X�^�e�B�b�N���b�V���̃t�@�C���� </param>
		void CreateObjectButton(const char* typeName, const char* staticMeshFile);

		/// <summary>
		/// �Q�[���I�u�W�F�N�g�쐬�p�|�b�v�A�b�v��W�J����
		/// </summary>
		void CreateObjectPopup_Open();

		/// <summary>
		/// �Q�[���I�u�W�F�N�g�쐬�p�|�b�v�A�b�v��W�J�������̏���
		/// </summary>
		void CreateObjectPopup_Run();

	private: // ------------------ �I�u�W�F�N�g�Ǘ��p --------------------

		// ��I�����̔ԍ�
		const int unselected = -1;

		// �I�𒆃I�u�W�F�N�g�̗v�f�ԍ�
		int selectObjectIndex = unselected;

		// �I�𒆂̃I�u�W�F�N�g
		GameObjectPtr selectObject;

	private: // ----------------------- �G���W�� -------------------------

		// �G���W���N���X�̃|�C���^
		Engine* engine = nullptr;
	};

} // namespace PokarinEngine

#endif // !HIERARCHY_H_INCLUDED
