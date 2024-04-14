/**
* @file NodeEditor.cpp
*/
#include "NodeEditor.h"

#include "Nodes/TestNode.h"

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
	NodeEditor::NodeEditor(GameObject* ownerObject)
	{
		// �������ݒ�
		owner = ownerObject;

		// �m�[�h�G�f�B�^�̖��O��ݒ�
		// ������₷���悤�Ɂu������̖��O + #������̎��ʔԍ��v�ɂ���
		name = owner->GetName();
		name += " #" + std::to_string(owner->GetID());

		nodeEditorContext = ImNodes::EditorContextCreate();
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

			// �m�[�h�G�f�B�^���쐬
			ImNodes::BeginNodeEditor();
			{
				// �쐬�ς݂̃m�[�h��\������
				for (auto& node : nodeList)
				{
					node->Render();
				}

				// �����N�ς݂̃s�����m����Ōq����
				for (int i = 0; i < linkPairList.size(); ++i)
				{
					int _s = linkPairList[i].first;
					int _e = linkPairList[i].second;
					ImNodes::Link(i, _s, _e);
				}

				ImNodes::EndNodeEditor();
			}

			// �s�����m�������N������z��ɒǉ�����
			int s = 0, e = 0;
			if (ImNodes::IsLinkCreated(&s, &e))
			{
				linkPairList.push_back(LinkPair(s, e));
			}

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
	void NodeEditor::CreateNode(NodePtr node)
	{
		// �m�[�h�z��ɒǉ�
		nodeList.emplace(node);

		// �d�����Ȃ��m�[�h�̎��ʔԍ�
		int nodeID = AddSingleID(nodeIDList);

		// �m�[�h�̎��ʔԍ���ݒ�
		node->SetID_OnlyOnce(nodeID);

		// �m�[�h�쐬���̏��������s
		node->CreateNode(this);
	}

	/// <summary>
	/// �m�[�h�쐬�p�|�b�v�A�b�v�̏���
	/// </summary>
	void NodeEditor::CreateNodePopup()
	{
		CreateNodeButton<TestNode>();
	}

#pragma endregion

#pragma region SingleID

	/// <summary>
	/// �m�[�h�̓��o�͗p�s���̎��ʔԍ����擾����
	/// </summary>
	/// <returns> �d�����Ȃ����ʔԍ� </returns>
	int NodeEditor::GetSinglePinID()
	{
		return AddSingleID(nodePinIDList);
	}

	/// <summary>
	/// �d�����Ȃ����ʔԍ���z��ɒǉ�����
	/// </summary>
	/// <param name="idList"> ���ʔԍ���ǉ�����z�� </param>
	/// <returns> �d�����Ȃ����ʔԍ� </returns>
	int NodeEditor::AddSingleID(IDList& idList)
	{
		// ���ʔԍ�
		int singleID = Random::Range(INT_MIN, INT_MAX);

		// ���ʔԍ���ǉ�����
		// �d�����Ă���ꍇ�͍ēx�ԍ����擾����
		while (!idList.emplace(singleID).second)
		{
			singleID = Random::Range(INT_MIN, INT_MAX);
		}

		return singleID;
	}

#pragma endregion

} // namespace PokarinEngine