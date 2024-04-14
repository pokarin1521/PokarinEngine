/**
* @file NodeEditor.h
*/
#ifndef NODEEDITOR_H_INCLUDED
#define NODEEDITOR_H_INCLUDED

#include "ImGui/imgui.h"
#include "ImGui/imgui_internal.h"

#include "ImGui/imnodes.h"
#include "ImGui/imnodes_internal.h"

#include "Nodes/Node.h"

#include "../UsingNames/UsingNode.h"
#include "../UsingNames/UsingNodeEditor.h"

#include <vector>
#include <unordered_set>
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

	public: // ------------------------- ���� --------------------------

		/// <summary>
		/// �X�V
		/// </summary>
		/// <returns>
		/// <para> true : �m�[�h�G�f�B�^�̃E�B���h�E���I������Ă��� </para>
		/// <para> false : �m�[�h�G�f�B�^�̃E�B���h�E���I������Ă��Ȃ� </para>
		/// </returns>
		bool Update();

	public: // ----------------- �m�[�h�̓��o�͗p�s�� ------------------

		/// <summary>
		/// �m�[�h�̓��o�͗p�s���̎��ʔԍ����擾����
		/// </summary>
		/// <returns> �d�����Ȃ����ʔԍ� </returns>
		int GetSinglePinID();

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

	private: // ----------------------- �^�̕ʖ����` -------------------------

		// <�m�[�hA�̎��ʔԍ�, �m�[�hB�̎��ʔԍ�>
		using LinkPair = std::pair<int, int>;
		using LinkPairList = std::vector<LinkPair>;

		using IDList = std::unordered_set<int>;

	private: // ------------------------- �m�[�h�쐬�p -------------------------

		/// <summary>
		/// �m�[�h�쐬�p�{�^���̏���
		/// </summary>
		/// <typeparam name="T"> �m�[�h�N���X </typeparam>
		template <class T>
		void CreateNodeButton()
		{
			// �{�^������������m�[�h�쐬
			if(ImGui::Button(T::name))
			{
				CreateNode(std::make_shared<T>());
			}
		}

		/// <summary>
		/// �m�[�h�쐬���̏���
		/// </summary>
		/// <param name="node"> �쐬�����m�[�h </param>
		void CreateNode(NodePtr node);

		/// <summary>
		/// �m�[�h�쐬�p�|�b�v�A�b�v�̏���
		/// </summary>
		void CreateNodePopup();

	private: // --------------------------- ���ʔԍ� ---------------------------

		/// <summary>
		/// �d�����Ȃ����ʔԍ���z��ɒǉ�����
		/// </summary>
		/// <param name="idList"> ���ʔԍ���ǉ�����z�� </param>
		/// <returns> �d�����Ȃ����ʔԍ� </returns>
		int AddSingleID(IDList& idList);

	private: // ------------------------- �m�[�h�Ǘ��p -------------------------

		// �m�[�h�Ǘ��p�z��
		NodeList nodeList;

		// �m�[�h�̃����N��Ԃ��Ǘ�����z��
		LinkPairList linkPairList;

		// �m�[�h�̎��ʔԍ����Ǘ�����z��
		IDList nodeIDList;

		// �m�[�h�̓��o�͗p�s���̎��ʔԍ����Ǘ�����z��
		IDList nodePinIDList;

	private: // --------------------- �m�[�h�G�f�B�^�̏�� ---------------------

		// �m�[�h�G�f�B�^�p�R���e�L�X�g
		ImNodesEditorContext* nodeEditorContext = nullptr;

		// ImGui�E�B���h�E
		ImGuiWindow* imGuiWindow = nullptr;

		// �m�[�h�G�f�B�^�̎�����
		GameObject* owner = nullptr;

		// �m�[�h�G�f�B�^�̖��O
		// ImGui�E�B���h�E�쐬���̃^�C�g���ɂ���
		std::string name = "";

		// �m�[�h�G�f�B�^���J���Ă���Ȃ�true
		bool isOpen = false;

	private: // ------------------------- �m�[�h�쐬�p -------------------------

		// �m�[�h�쐬�p�|�b�v�A�b�v�̖��O
		const char* createNodePopup = "CreateNode";
	};

} // namespace PokarinEngine

#endif // !NODEEDITOR_H_INCLUDED

