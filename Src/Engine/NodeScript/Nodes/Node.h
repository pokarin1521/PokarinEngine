/**
* @file Node.h
*/
#ifndef NODE_H_INCLUDED
#define NODE_H_INCLUDED

#include "ImGui/imgui.h"
#include "ImGui/imnodes.h"

#include "../../Window.h"
#include "../../InputManager.h"

#include <string>
#include <vector>
#include <unordered_set>
#include <memory>

namespace PokarinEngine
{
	// -----------------------
	// �O���錾
	// -----------------------

	class Node;
	class NodeEditor;

	// -----------------------------
	// �^�̕ʖ����`
	// -----------------------------

	using NodePtr = std::shared_ptr<Node>;
	using NodeList = std::unordered_set<NodePtr>;

	/// <summary>
	/// �m�[�h�̊��N���X
	/// </summary>
	class Node
	{
	public: // ------------------ �R���X�g���N�^�E�f�X�g���N�^ -------------------

		Node() = default;
		virtual ~Node() = default;

	public: // ---------------------------- �m�[�h���� ---------------------------

		/// <summary>
		/// ������
		/// </summary>
		virtual void Initialize() {}

		/// <summary>
		/// �m�[�h��\������
		/// </summary>
		virtual void RenderNode() = 0;

		/// <summary>
		/// �m�[�h�ǉ����̏���
		/// </summary>
		/// <param name="nodeEditor"> ������ł���m�[�h�G�f�B�^ </param>
		void AddNode(NodeEditor* nodeEditor)
		{
			// ������ł���m�[�h�G�f�B�^��ݒ�
			owner = nodeEditor;

			// �}�E�X�J�[�\���̈ʒu�Ƀm�[�h��ݒu����
			ImVec2 mousePos = Input::Mouse::GetScreenPos(WindowID::NodeScript);
			ImNodes::SetNodeScreenSpacePos(id, mousePos);

			// ������
			Initialize();
		}
	
	public: // -------------------------- ���ʔԍ��̐ݒ� --------------------------

		/// <summary>
		/// ���ݒ�̏ꍇ�����m�[�h�̎��ʔԍ���ݒ肷��
		/// </summary>
		void SetID_OnlyOnce(int setID)
		{
			// �ݒ�ς݂Ȃ牽�����Ȃ�
			if (isSetID)
			{
				return;
			}

			// ���ʔԍ���ݒ�
			id = setID;

			// �ݒ�ς݂ɂ���
			isSetID = true;
		}

	protected: // ------------------------- ���̎擾 ---------------------------

		/// <summary>
		/// �m�[�h�̎��ʔԍ����擾����
		/// </summary>
		/// <returns> �m�[�h�̎��ʔԍ� </returns>
		int GetID() const
		{
			return id;
		}

		/// <summary>
		/// ������ł���m�[�h�G�f�B�^���擾����
		/// </summary>
		/// <returns> ������ł���m�[�h�G�f�B�^ </returns>
		NodeEditor& GetOwner()
		{
			return *owner;
		}

	protected: // ------------------------ �m�[�h�쐬�p --------------------------

		/// <summary>
		/// �m�[�h�̃^�C�g����ݒ肷��
		/// </summary>
		/// <param name="title"> �^�C�g�� </param>
		void SetTitle(const char* title)
		{
			ImNodes::BeginNodeTitleBar();
			ImGui::Text(title);
			ImNodes::EndNodeTitleBar();
		}

	private: // ------------------------------ ��� ------------------------------

		// �m�[�h�̎��ʔԍ����ݒ�ς݂Ȃ�true
		bool isSetID = false;

		// �m�[�h�̎��ʔԍ�
		int id = 0;

		// ������ł���m�[�h�G�f�B�^
		NodeEditor* owner = nullptr;
	};

} // namespace PokarinEngine

#endif // !NODE_H_INCLUDED
