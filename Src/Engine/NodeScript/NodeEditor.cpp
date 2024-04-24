/**
* @file NodeEditor.cpp
*/
#include "NodeEditor.h"

#include "Nodes/Event/EventUpdateNode.h"
#include "Nodes/Functional/AddRotationNode.h"

#include "Nodes/Node.h"
#include "Pin/Pin.h"

#include "../GameObject.h"
#include "../Random.h"
#include "../InputManager.h"

namespace PokarinEngine
{
#pragma region NodeEditor

	/// <summary>
	/// �R���X�g���N�^
	/// </summary>
	/// <param name="object"> ������ɂȂ�Q�[���I�u�W�F�N�g </param>
	NodeEditor::NodeEditor(GameObject& ownerObject)
		: owner(&ownerObject), name(owner->GetName())
	{
		// �m�[�h�G�f�B�^�̖��O��ݒ�
		// ������₷���悤�Ɂu������̖��O + #������̎��ʔԍ��v�ɂ���
		name += " #" + std::to_string(owner->GetID());

		nodeEditorContext = ImNodes::EditorContextCreate();
	}

	/// <summary>
	/// �m�[�h�̏��������s����
	/// </summary>
	void NodeEditor::Run()
	{
		for (auto& eventNode : eventNodeList)
		{
			eventNode->Run();
		}
	}

	/// <summary>
	/// �X�V
	/// </summary>
	/// <returns>
	/// <para> true : �m�[�h�G�f�B�^�̃E�B���h�E���I������Ă��� </para>
	/// <para> false : �m�[�h�G�f�B�^�̃E�B���h�E���I������Ă��Ȃ� </para>
	/// </returns>
	bool NodeEditor::Update()
	{
		// �E�B���h�E���J���ĂȂ��Ȃ牽�����Ȃ�
		if (!isOpen)
		{
			return false;
		}

		// ImGui�E�B���h�E���쐬
		// �E�B���h�E���I�����ꂽ���ɏ�������
		if (ImGui::Begin(name.c_str(), &isOpen))
		{
			// ImGui�E�B���h�E��o�^
			if (!imGuiWindow)
			{
				imGuiWindow = ImGui::GetCurrentWindow();
			}

			// �m�[�h�G�f�B�^�p�R���e�L�X�g�̎g�p���J�n
			ImNodes::EditorContextSet(nodeEditorContext);

			// �E�N���b�N�Ńm�[�h�쐬�p�|�b�v�A�b�v��W�J����
			if (Input::GetKeyUp(KeyCode::MouseRight))
			{
				ImGui::OpenPopup(createNodePopup);
			}

			// �|�b�v�A�b�v�W�J���ɏ��������s����
			if (ImGui::BeginPopup(createNodePopup))
			{
				CreateNodePopup();

				ImGui::EndPopup();
			}

			// ���o�̓s���̎��ʔԍ�
			// �s���̃����N����p
			static int inputPinID = 0, outputPinID = 0;

			// �m�[�h�G�f�B�^���쐬
			ImNodes::BeginNodeEditor();
			{
				// �쐬�ς݂̃m�[�h��\������
				for (auto& [nodeID, node] : nodeList)
				{
					node->Render();
				}

				// �s���̃����N��Ԃ�\������
				RenderLink();

				ImNodes::EndNodeEditor();
			}

			// �s���̃����N��Ԃ��X�V����
			UpdateLink();

			ImGui::End();

			// �E�B���h�E���I������Ă���̂�true��Ԃ�
			return true;
		}

		// �I����ԂɊւ�炸Begin�֐����Ă΂�Ă���̂�
		// End�֐����Ă�
		ImGui::End();

		// �E�B���h�E���I������ĂȂ��̂�false��Ԃ�
		return false;
	}

#pragma endregion

#pragma region CreateNode

	/// <summary>
	/// �m�[�h�쐬���̏���
	/// </summary>
	/// <param name="node"> �쐬�����m�[�h </param>
	/// <param name="nodeTitle"> �쐬�����m�[�h�̃^�C�g�� </param>
	void NodeEditor::CreateNode(NodePtr node, const char* nodeTitle)
	{
		// �d�����Ȃ��m�[�h�̎��ʔԍ�
		int nodeID = GetSingleNodeID(node);

		// �쐬���̏��������s
		node->CreateNode(*this, nodeID, nodeTitle);
	}

	/// <summary>
	/// �m�[�h�쐬�p�|�b�v�A�b�v�̏���
	/// </summary>
	void NodeEditor::CreateNodePopup()
	{
		CreateNodeButton<AddRotationNode>("AddRotation");
		CreateNodeButton<EventUpdateNode>("EventUpdate");
	}

#pragma endregion

#pragma region CreatePin

	/// <summary>
	/// �m�[�h�̃s�����쐬����
	/// </summary>
	/// <param name="pinID"> �s���̎�����ɂȂ�m�[�h�̎��ʔԍ� </param>
	/// <param name="pinType"> �쐬����s���̎�� </param>
	/// <returns> �쐬�����s���̎��ʔԍ� </returns>
	int NodeEditor::CreatePin(int nodeID, PinType pinType)
	{
		// ���ʔԍ�
		int singleID = Random::Range(INT_MIN, INT_MAX);

		// ���ʔԍ���ǉ�����
		// �d�����Ă���ꍇ�͒ǉ��ł��Ȃ��̂ōēx�ԍ����擾����
		while (!pinList.emplace(singleID, nullptr).second)
		{
			singleID = Random::Range(INT_MIN, INT_MAX);
		}

		// �s�����쐬���ēo�^����
		pinList[singleID] = std::make_shared<Pin>(nodeID, singleID, pinType);

		// �쐬�����s���̎��ʔԍ���Ԃ�
		return singleID;
	}

#pragma endregion

#pragma region ID

	/// <summary>
	/// �d�����Ȃ��m�[�h���ʔԍ����擾����
	/// </summary>
	/// <param name="node"> �m�[�h </param>
	/// <returns> �d�����Ȃ����ʔԍ� </returns>
	int NodeEditor::GetSingleNodeID(NodePtr node)
	{
		// ���ʔԍ�
		int singleID = Random::Range(INT_MIN, INT_MAX);

		// ���ʔԍ���ǉ�����
		// �d�����Ă���ꍇ�͒ǉ��ł��Ȃ��̂ōēx�ԍ����擾����
		while (!nodeList.emplace(singleID, node).second)
		{
			singleID = Random::Range(INT_MIN, INT_MAX);
		}

		return singleID;
	}

#pragma endregion

#pragma region Link

	/// <summary>
	/// �s�����m�̃����N��Ԃ��X�V����
	/// </summary>
	void NodeEditor::UpdateLink()
	{
		// �s���̎��ʔԍ�
		static int inputPinID = 0, outputPinID = 0;

		// �s�����m�������N������z��ɒǉ�����
		if (ImNodes::IsLinkCreated(&inputPinID, &outputPinID))
		{
			// ���͗p�s��
			const Pin& inputPin = *pinList[inputPinID];

			// �o�͗p�s��
			const Pin& outputPin = *pinList[outputPinID];

			// �s���̎�ނ������ꍇ�̂݃����N��������
			if (inputPin.GetType() == outputPin.GetType())
			{
				// �����N����s���̑g��ǉ�
				AddLinkPair(LinkPair(inputPinID, outputPinID));
			}
		}

		// �}�E�X�J�[�\���Əd�Ȃ��Ă��郊���N���̎��ʔԍ�
		static int hoveredLinkID = 0;

		// Alt�L�[����͂��Ȃ��烊���N�������N���b�N���邱�Ƃ�
		// �����N����������
		if (ImNodes::IsLinkHovered(&hoveredLinkID))
		{
			if (Input::GetKey(KeyCode::LeftAlt) &&
				Input::GetKeyDown(KeyCode::MouseLeft))
			{
				DestroyLink(hoveredLinkID);
			}
		}
	}

	/// <summary>
	/// �s�����m�̃����N��Ԃ�\������
	/// </summary>
	void NodeEditor::RenderLink()
	{
		// �s���̎��ʔԍ�
		static int inputPinID = 0, outputPinID = 0;

		// �����N�ς݂̃s�����m����Ōq����
		for (auto& [linkID, linkPair] : linkPairList)
		{
			// �s���̎��ʔԍ����擾
			inputPinID = linkPair.first;
			outputPinID = linkPair.second;

			// �����N��Ԃ�����
			ImNodes::Link(linkID, inputPinID, outputPinID);
		}
	}

	/// <summary>
	/// �����N����g��ǉ�����
	/// </summary>
	/// <param name="linkPair"> �ǉ�����g </param>
	void NodeEditor::AddLinkPair(const LinkPair& linkPair)
	{
		// ���ʔԍ�
		int singleID = Random::Range(INT_MIN, INT_MAX);

		// ���ʔԍ���ǉ�����
		// �d�����Ă���ꍇ�͒ǉ��ł��Ȃ��̂ōēx�ԍ����擾����
		while (!linkPairList.emplace(singleID, linkPair).second)
		{
			singleID = Random::Range(INT_MIN, INT_MAX);
		}

		// ���͗p�s��
		const Pin& inputPin = *pinList[linkPair.first];

		// �o�͗p�s��
		const Pin& outputPin = *pinList[linkPair.second];

		// �ǉ�����g�����s�s�����m
		if (inputPin.GetType() == PinType::Run)
		{
			// ���͑��̃m�[�h
			Node& inputNode = *nodeList[inputPin.GetNodeID()];

			// �o�͑��̃m�[�h
			Node* outputNode = nodeList[outputPin.GetNodeID()].get();

			// ���͑��̎��Ɏ��s����m�[�h�Ƃ���
			// �o�͑��̃m�[�h��o�^����
			inputNode.SetNextNode(outputNode);
		}
	}

	/// <summary>
	/// �w�肵���g�̃����N���폜����
	/// </summary>
	/// <param name="linkPairID"> �폜���郊���N�̎��ʔԍ� </param>
	void NodeEditor::DestroyLink(int linkPairID)
	{
		// �s���̎��ʔԍ�
		int pinID = linkPairList[linkPairID].first;

		// �s��
		const Pin& pin = *pinList[pinID];

		// �폜���郊���N�����s�s�����m�̂��̂Ȃ�
		// ���Ɏ��s����m�[�h�̐ݒ����������
		if (pin.GetType() == PinType::Run)
		{
			Node& node = *nodeList[pin.GetNodeID()];
			node.SetNextNode(nullptr);
		}

		// �����N���폜
		linkPairList.erase(linkPairID);
	}

#pragma endregion

} // namespace PokarinEngine