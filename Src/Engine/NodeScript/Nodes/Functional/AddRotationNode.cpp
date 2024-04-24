/**
* @file AddRotationNode.cpp
*/
#include "AddRotationNode.h"

#include "ImGui/imgui.h"

#include "../../../GameObject.h"
#include "../../../Time.h"

#include <string>

namespace PokarinEngine
{
	/// <summary>
	/// ���s����
	/// </summary>
	void AddRotationNode::Run()
	{
		// �Q�[���I�u�W�F�N�g
		GameObject& gameObject = GetGameObject();

		gameObject.transform->rotation += addRotation * Time::DeltaTime();
	}

	/// �����ł����g��Ȃ��̂ŁAcpp�݂̂ɏ���
	/// <summary>
	/// �h���b�O����p�X���C�_�[�Œl��\������
	/// </summary>
	/// <param name="valueName"> �l�̖��O </param>
	/// <param name="value"> �l </param>
	void DragValue(const std::string& valueName, float* value)
	{
		// ���ʗp���x��
		std::string label = "##AddRotation_" + valueName;

		// �f�[�^���͗p�X���C�_�[�̕�
		static const float sliderWidth = 50.0f;

		// �h���b�O���x
		static const float dragSpeed = 1.0f;

		// �\�����錅�����w�肷��t�H�[�}�b�g
		// ����1���܂ŕ\������
		static const char* format = "%.1f";

		// �X���C�_�[�̕���ݒ�
		ImGui::PushItemWidth(sliderWidth);
		{
			// �f�[�^����\��
			ImGui::Text(valueName.c_str());

			// �f�[�^���Ɠ����s�ɃX���C�_�[�\��
			ImGui::SameLine();
			ImGui::DragFloat(label.c_str(), value, dragSpeed, 0, 0, format);

			ImGui::PopItemWidth();
		}
	}

	/// �����ł����g��Ȃ��̂ŁAcpp�݂̂ɏ���
	/// <summary>
	/// �h���b�O����p�X���C�_�[�ŉ�]�p�x��\������
	/// </summary>
	/// <param name="data"> �f�[�^ </param>
	void DragRotation(Vector3& rotation)
	{
		// X����]�p�x��\��
		DragValue("X", &rotation.x);

		// �����s��Y����]�p�x��\��
		ImGui::SameLine();
		DragValue("Y", &rotation.y);

		// �����s��Z����]�p�x��\��
		ImGui::SameLine();
		DragValue("Z", &rotation.z);
	}

	/// <summary>
	/// �f�[�^�s����\������
	/// </summary>
	void AddRotationNode::RenderDataPin()
	{
		// �f�[�^�s���̐ݒ���J�n
		BeginDataPin(inputRotationPin, PinAttribute::Input);

		// ��]�p�x�̉��Z�l��\��
		DragRotation(addRotation);

		// �f�[�^�s���̐ݒ���I��
		EndPin(PinAttribute::Input);
	}
}