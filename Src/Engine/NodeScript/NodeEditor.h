/**
* @file NodeEditor.h
*/
#ifndef NODEEDITOR_H_INCLUDED
#define NODEEDITOR_H_INCLUDED

#include "ImGui/imgui.h"
#include "ImGui/imgui_internal.h"

#include "ImGui/imnodes.h"
#include "ImGui/imnodes_internal.h"

#include "../UsingNames/UsingNodeEditor.h"

#include "Pin/PinType.h"

#include <string>
#include <unordered_map>
#include <memory>
#include <cassert>

namespace PokarinEngine
{
	// -----------------------
	// �O���錾
	// -----------------------

	class GameObject;
	class Node;
	class EventNode;
	class Pin;

	/// <summary>
	/// �m�[�h�ŃI�u�W�F�N�g�ɑ΂��鏈����ҏW������
	/// </summary>
	class NodeEditor
	{
	public: // -------------- �R���X�g���N�^�E�f�X�g���N�^ --------------

		NodeEditor(GameObject& gameObject);

		~NodeEditor()
		{
			Finalize();
			ImNodes::EditorContextFree(nodeEditorContext);
		}

	public: // ------------------------- ���� --------------------------

		/// <summary>
		/// �m�[�h�̏��������s����
		/// </summary>
		void Run();

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
		/// �m�[�h�̃s�����쐬����
		/// </summary>
		/// <param name="pinID"> �s���̎�����ɂȂ�m�[�h�̎��ʔԍ� </param>
		/// <param name="pinType"> �쐬����s���̎�� </param>
		/// <returns> �쐬�����s���̎��ʔԍ� </returns>
		int CreatePin(int nodeID, PinType pinType);

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

		/// <summary>
		/// ������ł���Q�[���I�u�W�F�N�g���擾����
		/// </summary>
		/// <returns> ������ł���Q�[���I�u�W�F�N�g </returns>
		GameObject& GetOwnerObject()
		{
			return *ownerObject;
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

		// <���͗p�s���̎��ʔԍ�, �o�͗p�s���̎��ʔԍ�>
		using LinkPair = std::pair<int, int>;

		// <�����N���ʔԍ�, �����N�̑g>
		using LinkPairList = std::unordered_map<int, LinkPair>;

		using NodePtr = std::shared_ptr<Node>;
		using EventNodePtr = std::shared_ptr<EventNode>;
		using PinPtr = std::shared_ptr<Pin>;

		// <���ʔԍ�, �m�[�h>
		using NodeList = std::unordered_map<int, NodePtr>;

		// <���ʔԍ�, �s��>
		using PinList = std::unordered_map<int, PinPtr>;

		// <���ʔԍ�, �C�x���g�m�[�h>
		using EventNodeList = std::unordered_map<int, EventNodePtr>;

	private: // ------------------------- �m�[�h�쐬�p -------------------------

		/// <summary>
		/// �m�[�h�쐬�p�{�^���̏���
		/// </summary>
		/// <typeparam name="T"> �m�[�h�N���X </typeparam>
		/// <param name="nodeTitle"> �m�[�h�̃^�C�g�� </param>
		template <class T>
		void CreateNodeButton(const std::string& nodeTitle)
		{
			// �^�C�g�����ݒ肳��Ă邩�m�F
			assert(!nodeTitle.empty());

			// �m�[�h�쐬�p�{�^��
			if (ImGui::Button(nodeTitle.c_str()))
			{
				// �����ꂽ��m�[�h�쐬
				CreateNode<T>(nodeTitle);
			}
		}

		/// <summary>
		/// �m�[�h�쐬�p�{�^���̏���
		/// </summary>
		/// <typeparam name="T"> �m�[�h�N���X </typeparam>
		/// <param name="nodeTitle"> �m�[�h�̃^�C�g�� </param>
		template <class T>
		void CreateNode(const std::string& nodeTitle)
		{
			// �m�[�h
			auto node = std::make_shared<T>();

			// �d�����Ȃ��m�[�h�̎��ʔԍ�
			int nodeID = AddNode(node);

			// �C�x���g�m�[�h�Ȃ�z��ɒǉ�����
			if constexpr (std::is_base_of_v<EventNode, T>)
			{
				eventNodeList.emplace(nodeID, node);
			}

			// �쐬���̏��������s
			node->CreateNode(*this, nodeID, nodeTitle);
		}

		/// <summary>
		/// �m�[�h�쐬�p�|�b�v�A�b�v�̏���
		/// </summary>
		void CreateNodePopup();

	private: // ------------------------- �m�[�h�̒ǉ� -------------------------

		/// <summary>
		/// �m�[�h��ǉ�����
		/// </summary>
		/// <param name="node"> �ǉ�����m�[�h </param>
		/// <returns> �ǉ������m�[�h�̎��ʔԍ� </returns>
		int AddNode(NodePtr node);

	private: // ------------------------- �m�[�h�폜�p -------------------------

		/// <summary>
		/// �m�[�h���폜����
		/// </summary>
		/// <param name="nodeID"> �폜����m�[�h�̎��ʔԍ� </param>
		void DestroyNode(int nodeID);

	private: // ------------------------- �m�[�h����p -------------------------

		/// <summary>
		/// �G�f�B�^���̃m�[�h�̏�Ԃ��X�V����
		/// </summary>
		void UpdateNode();

	private: // ----------------------- �s�����m�̃����N -----------------------

		/// <summary>
		/// �s�����m�̃����N��Ԃ��X�V����
		/// </summary>
		void UpdateLink();

		/// <summary>
		/// �s�����m�̃����N��Ԃ�\������
		/// </summary>
		void RenderLink();

		/// <summary>
		/// �����N����g��ǉ�����
		/// </summary>
		/// <param name="linkPair"> �ǉ�����g </param>
		void AddLinkPair(const LinkPair& linkPair);

		/// <summary>
		/// �w�肵���g�̃����N���폜����
		/// </summary>
		/// <param name="linkPairID"> �폜���郊���N�̎��ʔԍ� </param>
		void DestroyLink(int linkPairID);

	private: // --------------------------- �I������ ---------------------------

		/// <summary>
		/// �I������
		/// </summary>
		void Finalize();

	private: // ------------------------- �m�[�h�Ǘ��p -------------------------

		// �m�[�h�Ǘ��p�z��
		NodeList nodeList;

		// �C�x���g�m�[�h�Ǘ��p�z��
		NodeList eventNodeList;

		// �s���Ǘ��p�z��
		PinList pinList;

		// �����N���Ă���s���̎��ʔԍ����Ǘ�����z��
		LinkPairList linkPairList;

	private: // --------------------- �m�[�h�G�f�B�^�̏�� ---------------------

		// �m�[�h�G�f�B�^�p�R���e�L�X�g
		ImNodesEditorContext* nodeEditorContext = nullptr;

		// ImGui�E�B���h�E
		ImGuiWindow* imGuiWindow = nullptr;

		// ������ł���Q�[���I�u�W�F�N�g
		GameObject* ownerObject = nullptr;

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

