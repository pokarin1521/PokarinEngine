/**
* @file AddWorldPositionNode.cpp
*/
#include "AddWorldPositionNode.h"

#include "../../../GameObject.h"
#include "../../../Time.h"

namespace PokarinEngine
{
	/// <summary>
	/// �m�[�h�ʂ̎��s����
	/// </summary>
	/// <returns>
	/// <para> true : ���̃m�[�h�����s���� </para>
	/// <para> false : ���̃m�[�h�����s���Ȃ� </para>
	/// </returns>
	bool AddWorldPositionNode::RunNode()
	{
		// �Q�[���I�u�W�F�N�g
		GameObject& gameObject = GetOnwerObject();

		// ���Z�l�̕������ړ�������
		gameObject.transform->position += translate;

		// ���̃m�[�h�����s����
		return true;
	}

	/// <summary>
	/// �f�[�^�s����\������
	/// </summary>
	void AddWorldPositionNode::RenderDataPin()
	{
		// �f�[�^�s���̕\�����J�n
		BeginDataPin(inputPositionPin, PinAttribute::Input);

		// �X���C�_�[�̕�
		static const float sliderWidth = 50.0f;

		// �ړ��ʂ�\��
		translate.RenderDrag("Translate", GetID_String(), sliderWidth);

		// �f�[�^�s���̕\�����I��
		EndPin(PinAttribute::Input);
	}
}