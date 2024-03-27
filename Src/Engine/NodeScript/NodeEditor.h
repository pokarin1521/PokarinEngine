/**
* @file NodeEditor.h
*/
#ifndef NODEEDITOR_H_INCLUDED
#define NODEEDITOR_H_INCLUDED

#include "ImGui/imgui_internal.h"

#include "ImGui/imnodes.h"
#include "ImGui/imnodes_internal.h"

#include "Nodes/Node.h"

#include "../UsingNames/UsingNodeEditor.h"

#include <vector>
#include <memory>

namespace PokarinEngine
{
	// -----------------------
	// �O���錾
	// -----------------------

	class GameObject;

	/// <summary>
	/// �m�[�h�ŃI�u�W�F�N�g�ɑ΂��鏈����ҏW������
	/// </summary>
	class NodeEditor
	{
	public: // -------------------------- �쐬 --------------------------

		/// <summary>
		/// �m�[�h�G�f�B�^���쐬����
		/// </summary>
		/// <returns> �쐬�����m�[�h�G�f�B�^ </returns>
		static NodeEditorPtr Create(GameObject* ownerObject)
		{
			return std::make_shared<NodeEditor>(ownerObject);
		}

	public: // -------------- �R���X�g���N�^�E�f�X�g���N�^ --------------

		NodeEditor(GameObject* ownerObject);

		~NodeEditor()
		{
			ImNodes::EditorContextFree(nodeEditorContext);
		}

	public: // -------------------- �^�̕ʖ����` ---------------------

		// <�m�[�hA�̎��ʔԍ�, �m�[�hB�̎��ʔԍ�>
		using LinkPair = std::pair<int, int>;
		using LinkPairList = std::vector<LinkPair>;

	public: // ------------------------- ���� --------------------------

		/// <summary>
		/// �X�V
		/// </summary>
		/// <returns>
		/// <para> true : �m�[�h�G�f�B�^�̃E�B���h�E���I������Ă��� </para>
		/// <para> false : �m�[�h�G�f�B�^�̃E�B���h�E���I������Ă��Ȃ� </para>
		/// </returns>
		bool Update();

	public: // --------------------- �m�[�h�̒ǉ� ----------------------

		void AddNode(NodePtr node)
		{
			nodeList.emplace(node);
		}

	public: // ---------------------- ���̎擾 -----------------------

		/// <summary>
		/// ImGui�E�B���h�E���擾����
		/// </summary>
		/// <returns> ImGui�E�B���h�E�̎Q�� </returns>
		ImGuiWindow& GetImGuiWindow() const
		{
			return *imGuiWindow;
		}

		/// <summary>
		/// ���O���擾����
		/// </summary>
		/// <returns> �m�[�h�G�f�B�^�̖��O </returns>
		const char* GetName() const
		{
			return name.c_str();
		}

	public: // ---------------------- ��Ԃ̐��� -----------------------

		/// <summary>
		/// �m�[�h�G�f�B�^���J���Ă��邩�擾����
		/// </summary>
		/// <returns>
		/// <para> true : �J���Ă��� </para>
		/// <para> false : �J���Ă��Ȃ� </para>
		/// </returns>
		bool IsOpen() const
		{
			return isOpen;
		}

		/// <summary>
		/// �m�[�h�G�f�B�^���J��
		/// </summary>
		void OpenEditor()
		{
			isOpen = true;
		}

	private: // ------------------------- ��� -------------------------

		// �m�[�h�Ǘ��p�z��
		NodeList nodeList;

		// �m�[�h�̃����N��Ԃ��Ǘ�����z��
		LinkPairList linkPairList;

		// �m�[�h�G�f�B�^�̎�����
		GameObject* owner = nullptr;

		// ImGui�E�B���h�E
		ImGuiWindow* imGuiWindow = nullptr;

		// �m�[�h�G�f�B�^�̖��O
		// ImGui�E�B���h�E�쐬���̃^�C�g���ɂ���
		std::string name = "";

		// �m�[�h�G�f�B�^���J���Ă���Ȃ�true
		bool isOpen = false;

		// �m�[�h�G�f�B�^�p�R���e�L�X�g
		ImNodesEditorContext* nodeEditorContext = nullptr;
	};

} // namespace PokarinEngine

#endif // !NODEEDITOR_H_INCLUDED

