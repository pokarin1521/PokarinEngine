/**
* @file NodeEditor.cpp
*/
#include "NodeEditor.h"

#include "Nodes/Event/EventNode.h"

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
	NodeEditor::NodeEditor(GameObject& gameObject)
		: ownerObject(&gameObject), name(gameObject.GetName())
	{
		// �m�[�h�G�f�B�^�̖��O��ݒ�
		// ������₷���悤�Ɂu������̖��O + #������̎��ʔԍ��v�ɂ���
		name += " #" + std::to_string(gameObject.GetID());

		// �R���e�L�X�g�쐬
		nodeEditorContext = ImNodes::EditorContextCreate();
	}

	/// <summary>
	/// �m�[�h�̏��������s����
	/// </summary>
	void NodeEditor::Run()
	{
		for (auto& [nodeID, eventNode] : eventNodeList)
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
		// �E�B���h�E���I������Ă��Ȃ��̂ŉ������Ȃ�
		if (ImGui::Begin(name.c_str(), &isOpen, ImGuiWindowFlags_NoSavedSettings))
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

			// �G�f�B�^���̃m�[�h�̏�Ԃ��X�V����
			UpdateNode();

			// �s���̃����N��Ԃ��X�V����
			UpdateLink();

			// ImGui�E�B���h�E���I��
			ImGui::End();

			// �E�B���h�E���I������Ă���̂�true��Ԃ�
			return true;
		}

		// �I������Ă��Ȃ��Ă�Begin�֐����Ă΂�Ă���̂ŁA
		// ImGui�E�B���h�E���I������
		ImGui::End();

		// �E�B���h�E���I������Ă��Ȃ��̂�false��Ԃ�
		return false;
	}

	/// <summary>
	/// �I������
	/// </summary>
	void NodeEditor::Finalize()
	{
		// �m�[�h��S�폜
		nodeList.clear();
		eventNodeList.clear();

		// �s����S�폜
		pinList.clear();

		// �����N��S�폜
		linkPairList.clear();
	}

#pragma endregion

#pragma region Node

	/// <summary>
	/// �m�[�h��ǉ�����
	/// </summary>
	/// <param name="node"> �ǉ�����m�[�h </param>
	/// <returns> �ǉ������m�[�h�̎��ʔԍ� </returns>
	int NodeEditor::AddNode(NodePtr node)
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

	/// <summary>
	/// �m�[�h���폜����
	/// </summary>
	/// <param name="nodeID"> �폜����m�[�h�̎��ʔԍ� </param>
	void NodeEditor::DestroyNode(int nodeID)
	{
		// �o�^����Ă��Ȃ����ʔԍ��Ȃ牽�����Ȃ�
		if (nodeList.find(nodeID) == nodeList.end())
		{
			return;
		}

		eventNodeList.erase(nodeID);
		nodeList.erase(nodeID);
	}

	/// <summary>
	/// �G�f�B�^���̃m�[�h�̏�Ԃ��X�V����
	/// </summary>
	void NodeEditor::UpdateNode()
	{
		// �m�[�h�G�f�B�^���쐬
		ImNodes::BeginNodeEditor();
		{
			// �폜����m�[�h�̎��ʔԍ��̊Ǘ��p�z��
			std::vector<int> destroyNodeIDList;
			destroyNodeIDList.reserve(nodeList.size());

			// [�m�[�h���ʔԍ�, �m�[�h]
			for (auto& [nodeID, node] : nodeList)
			{
				// �m�[�h��\������
				node->Render();

				// �m�[�h��I�����������ŁADelete�L�[����������
				// �폜�p�z��Ɏ��ʔԍ���ǉ�
				if (ImNodes::IsNodeSelected(nodeID) &&
					Input::GetKeyDown(KeyCode::Delete))
				{
					destroyNodeIDList.push_back(nodeID);
				}
			}

			// �z��ɒǉ�����Ă��鎯�ʔԍ��̃m�[�h���폜����
			for (auto destroyNodeID : destroyNodeIDList)
			{
				DestroyNode(destroyNodeID);
			}

			// �s���̃����N��Ԃ�\������
			RenderLink();

			ImNodes::EndNodeEditor();
		}
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