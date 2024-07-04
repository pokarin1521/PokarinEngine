/**
* @file Node.cpp
*/
#include "Node.h"

#include "ImGui/imgui.h"
#include "ImGui/imnodes.h"

#include "../NodeEditor.h"
#include "../Pin/Pin.h"

#include "../../Window.h"
#include "../../InputManager.h"

#include "../../Color.h"

#include "../../GameObject.h"

namespace PokarinEngine
{
#pragma region Node

	/// <summary>
	/// �m�[�h�쐬���̏���
	/// </summary>
	/// <param name="[in] nodeEditor"> ������ł���m�[�h�G�f�B�^ </param>
	/// <param name="[in] nodeID"> �m�[�h�̎��ʔԍ� </param>
	/// <param name="[in] nodeTitle"> �m�[�h�̃^�C�g�� </param>
	void Node::CreateNode(NodeEditor& nodeEditor, int nodeID, const std::string& nodeTitle)
	{
		// ������ł���m�[�h�G�f�B�^��ݒ�
		ownerEditor = &nodeEditor;

		// �m�[�h�̎��ʔԍ���ݒ�
		id = nodeID;

		// �^�C�g����ݒ肷��
		title = nodeTitle;

		// �}�E�X�J�[�\���̈ʒu�Ƀm�[�h��ݒu
		Vector2 mousePos = Input::Mouse::GetScreenPos(WindowID::NodeScript);
		ImNodes::SetNodeScreenSpacePos(id, ImVec2(mousePos.x, mousePos.y));

		// ������
		Initialize();
	}

	/// <summary>
	/// ���s����
	/// </summary>
	void Node::Run()
	{
		// �m�[�h�ʂ̏��������s
		if (RunNode())
		{
			// ���̃m�[�h���ݒ肳��Ă���̂ŁA���s
			RunNextNode();
		}
	}

	/// <summary>
	/// �^�C�g����\������
	/// </summary>
	void Node::RenderTitle()
	{
		ImNodes::BeginNodeTitleBar();
		ImGui::Text(title.c_str());
		ImNodes::EndNodeTitleBar();
	}

	/// <summary>
	/// �m�[�h��\������
	/// </summary>
	void Node::Render()
	{
		ImNodes::BeginNode(id);
		RenderTitle();
		RenderPin();
		ImNodes::EndNode();
	}

#pragma endregion

#pragma region CreatePin

	/// <summary>
	/// �s�����쐬����
	/// </summary>
	/// <param name="[in] pinType"> �s���̎�� </param>
	/// <returns> �쐬�����s���̎��ʔԍ� </returns>
	int Node::CreatePin(PinType pinType)
	{
		return ownerEditor->CreatePin(id, pinType);
	}

#pragma endregion

#pragma region RenderPin

	/// <summary>
	/// �s���̌`
	/// </summary>
	enum class Node::PinShape
	{
		Triangle = ImNodesPinShape_TriangleFilled,
		Circle = ImNodesPinShape_CircleFilled,
	};

	/// <summary>
	/// �s���̕\�����J�n����
	/// </summary>
	/// <param name="[in] pinID"> �s���̎��ʔԍ� </param>
	/// <param name="[in] pinAttribute"> �s���̑��� </param>
	/// <param name="[in] pinShape"> �s���̌` </param>
	void Node::BeginPin(int pinID, PinAttribute pinAttribute, PinShape pinShape)
	{
		// ���͗p�s��
		if (pinAttribute == PinAttribute::Input)
		{
			ImNodes::BeginInputAttribute(pinID, ImNodesPinShape(pinShape));
		}
		// �o�͗p�s��
		else
		{
			ImNodes::BeginOutputAttribute(pinID, ImNodesPinShape(pinShape));
		}
	}

	/// <summary>
	/// �f�[�^�s���̕\�����J�n����
	/// </summary>
	/// <param name="[in] pinID"> �s���̎��ʔԍ� </param>
	/// <param name="[in] pinAttribute"> �s���̑��� </param>
	void Node::BeginDataPin(int pinID, PinAttribute pinAttribute)
	{
		BeginPin(pinID, pinAttribute, PinShape::Circle);
	}

	/// <summary>
	/// ���s�s���̕\�����J�n����
	/// </summary>
	/// <param name="[in] pinID"> �s���̎��ʔԍ� </param>
	/// <param name="[in] pinAttribute"> �s���̑��� </param>
	void Node::BeginRunPin(int pinID, PinAttribute pinAttribute)
	{
		BeginPin(pinID, pinAttribute, PinShape::Triangle);
	}

	/// <summary>
	/// �s���̕\�����I������
	/// </summary>
	/// <param name="[in] pinAttribute"> �s���̑��� </param>
	void Node::EndPin(PinAttribute pinAttribute)
	{
		// ���͗p�s��
		if (pinAttribute == PinAttribute::Input)
		{
			ImNodes::EndInputAttribute();
		}
		// �o�͗p�s��
		else
		{
			ImNodes::EndOutputAttribute();
		}
	}

	/// <summary>
	/// ���ɐݒ肷��s���𓯂��s�ɕ\������
	/// </summary>
	void Node::PinSameLin()
	{
		// �s�����m�̊Ԋu
		static const float spacing = 0;
		ImGui::SameLine(0, spacing);
	}

#pragma endregion

#pragma region GameObject

	/// <summary>
	/// �m�[�h�G�f�B�^�̎�����ł���Q�[���I�u�W�F�N�g���擾����
	/// </summary>
	/// <returns> ������ł���Q�[���I�u�W�F�N�g </returns>
	GameObject& Node::GetOnwerObject()
	{
		return ownerEditor->GetOwnerObject();
	}

#pragma endregion
}