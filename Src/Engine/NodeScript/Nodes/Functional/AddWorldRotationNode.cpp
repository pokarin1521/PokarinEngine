/**
* @file AddWorldRotationNode.cpp
*/
#include "AddWorldRotationNode.h"

#include "ImGui/imgui.h"

#include "../../../GameObject.h"
#include "../../../Math/Angle.h"
#include "../../../Time.h"

#include <string>

namespace PokarinEngine
{
	/// <summary>
	/// �m�[�h�ʂ̎��s����
	/// </summary>
	/// <returns>
	/// <para> true : ���̃m�[�h�����s���� </para>
	/// <para> false : ���̃m�[�h�����s���Ȃ� </para>
	/// </returns>
	bool AddWorldRotationNode::RunNode()
	{
		// �Q�[���I�u�W�F�N�g
		GameObject& gameObject = GetOnwerObject();

		// ��]�ʂ��ʓx�@�ɕϊ������Z����
		gameObject.transform->rotation += Radians(rotationDeg);

		// ���̃m�[�h�����s����
		return true;
	}

	/// <summary>
	/// �f�[�^�s����\������
	/// </summary>
	void AddWorldRotationNode::RenderDataPin()
	{
		// �f�[�^�s���̕\�����J�n
		BeginDataPin(inputRotationPin, PinAttribute::Input);

		// �X���C�_�[�̕�
		static const float sliderWidth = 50.0f;

		// ��]��(�x���@)��\��
		rotationDeg.RenderDrag("Degree", GetID_String(), sliderWidth);

		// �f�[�^�s���̕\�����I��
		EndPin(PinAttribute::Input);
	}
}