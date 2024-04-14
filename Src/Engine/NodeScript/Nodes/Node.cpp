/**
* @file Node.cpp
*/
#include "Node.h"

#include "ImGui/imgui.h"
#include "ImGui/imnodes.h"

#include "../NodeEditor.h"

#include "../../Window.h"
#include "../../InputManager.h"

namespace PokarinEngine
{
#pragma region Create, Render

	/// <summary>
	/// �m�[�h�쐬���̏���
	/// </summary>
	/// <param name="nodeEditor"> ������ł���m�[�h�G�f�B�^ </param>
	void Node::CreateNode(NodeEditor* nodeEditor)
	{
		// ������ł���m�[�h�G�f�B�^��ݒ�
		owner = nodeEditor;

		// �}�E�X�J�[�\���̈ʒu�Ƀm�[�h��ݒu����
		ImVec2 mousePos = Input::Mouse::GetScreenPos(WindowID::NodeScript);
		ImNodes::SetNodeScreenSpacePos(id, mousePos);

		// ������
		Initialize();
	}

	/// <summary>
	/// �m�[�h��\������
	/// </summary>
	void Node::Render()
	{
		ImNodes::BeginNode(id);
		RenderInfo();
		ImNodes::EndNode();
	}

#pragma endregion

#pragma region ID

	/// <summary>
	/// ���ݒ�̏ꍇ�����m�[�h�̎��ʔԍ���ݒ肷��
	/// </summary>
	void Node::SetID_OnlyOnce(int setID)
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

	/// <summary>
	/// �d�����Ȃ����o�͗p�s���̎��ʔԍ����擾����
	/// </summary>
	/// <returns> �d�����Ȃ����ʔԍ� </returns>
	int Node::GetSinglePinID()
	{
		return owner->GetSinglePinID();
	}

#pragma endregion

#pragma region SetNode

	/// <summary>
	/// �m�[�h�̓��͗p�s����ݒ肷��
	/// </summary>
	/// <param name="pinID"> �ݒ肷��s���̎��ʔԍ� </param>
	/// <param name="pinName"> �ݒ肷��s���̖��O </param>
	void Node::SetInputPin(int pinID, const char* pinName)
	{
		ImNodes::BeginInputAttribute(pinID, ImNodesPinShape_CircleFilled);

		ImGui::Text(pinName);

		ImNodes::EndInputAttribute();
	}

	/// <summary>
	/// �m�[�h�̏o�͗p�s����ݒ肷��
	/// </summary>
	/// <param name="pinID"> �ݒ肷��s���̎��ʔԍ� </param>
	/// <param name="pinName"> �ݒ肷��s���̖��O </param>
	void Node::SetOutputPin(int pinID, const char* pinName)
	{
		ImNodes::BeginOutputAttribute(pinID, ImNodesPinShape_CircleFilled);

		ImGui::Text(pinName);

		ImNodes::EndOutputAttribute();
	}

	/// <summary>
	/// �m�[�h�̓��o�͗p�s����ݒ肷��
	/// </summary>
	/// <param name="inputID"> ���͗p�s���̎��ʔԍ� </param>
	/// <param name="outputID"> �o�͗p�s���̎��ʔԍ� </param>
	/// <param name="inputName"> ���͗p�s���̖��O </param>
	/// <param name="outputName"> �o�͗p�s���̖��O </param>
	void Node::SetInOutPin(int inputID, int outputID, const char* inputName, const char* outputName)
	{
		// ���͗p�s���̖��O�Əo�͗p�s���̖��O�̊Ԋu
		static const float spacing = 20;

		// ���o�͗p�s���𓯂��s�ɕ\������
		SetInputPin(inputID, inputName);
		ImGui::SameLine(0, spacing);
		SetOutputPin(outputID, outputName);
	}

#pragma endregion
}