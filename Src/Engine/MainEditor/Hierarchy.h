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
	public: // --------------------------- ���� ----------------------------

		/// <summary>
		/// ������
		/// </summary>
		/// <param name="[in] engine"> �G���W���N���X�̎Q�� </param>
		void Initialize(Engine& e);

		/// <summary>
		/// �X�V
		/// </summary>
		void Update();

	public: // -------------------- �I�u�W�F�N�g�̎擾 ---------------------

		/// <summary>
		/// �I�𒆂̃I�u�W�F�N�g���擾����
		/// </summary>
		/// <returns> �E�B���h�E���őI�𒆂̃I�u�W�F�N�g </returns>
		GameObjectPtr& GetSelectObject();

	private: // -------------------- �E�B���h�E���̏��� --------------------

		/// <summary>
		/// �q�G�����L�[�E�B���h�E���ł̑���
		/// </summary>
		void HierarchyControl();

		/// <summary>
		/// ���j���[�̏���
		/// </summary>
		void Mene();

		/// <summary>
		/// �Q�[���I�u�W�F�N�g�\���p�c���[�m�[�h�̏���
		/// </summary>
		void ObjectTree();

		/// <summary>
		/// �I�𒆂̃I�u�W�F�N�g�ɑ΂��鑀��
		/// </summary>
		void ScelectObjectControl();

	private: // -------------------- �I�u�W�F�N�g�쐬�p --------------------

		/// <summary>
		/// �Q�[���I�u�W�F�N�g�쐬�p�|�b�v�A�b�v�̏���
		/// </summary>
		void CreateObjectPopup();

		/// <summary>
		/// �Q�[���I�u�W�F�N�g�쐬�p�{�^���̏���
		/// </summary>
		/// <param name="[in] kindName"> �쐬����Q�[���I�u�W�F�N�g�̎�� </param>
		/// <param name="[in] staticMeshFile"> �X�^�e�B�b�N���b�V���̃t�@�C���� </param>
		void CreateObjectButton(const char* typeName, const char* staticMeshFile);

	private: // --------------------------- ��� ---------------------------

		// ��I�����̗v�f�ԍ�
		const int unselected = -1;

		// �I�𒆃I�u�W�F�N�g�̗v�f�ԍ�
		int selectObjectIndex = unselected;

		// �I�𒆂̃I�u�W�F�N�g
		GameObjectPtr selectObject;

		// �G���W���N���X�̃|�C���^
		Engine* engine = nullptr;

		// �I�u�W�F�N�g�쐬�p�|�b�v�A�b�v�̖��O
		const char* createObjectPopupName = "CreateObject";
	};

} // namespace PokarinEngine

#endif // !HIERARCHY_H_INCLUDED
