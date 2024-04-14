/**
* @file EventNode.cpp
*/
#include "EventNode.h"

#include "ImGui/imnodes.h"

namespace PokarinEngine
{
	/// <summary>
	/// �m�[�h����\������
	/// </summary>
	void EventNode::RenderInfo()
	{
		// �^�C�g����ݒ�
		ImNodes::BeginNodeTitleBar();
		ImGui::Text(name);
		ImNodes::EndNodeTitleBar();

		/* �C�x���g�m�[�h��1�ԍŏ��Ɏg����m�[�h�Ȃ̂ŁA
		�����N�p�s���͏o�͂����ɂ��� */

		// �����N�p�o�̓s����ݒ�
		ImNodes::BeginOutputAttribute(linkOutputPin, ImNodesPinShape_TriangleFilled);
		ImNodes::EndOutputAttribute();

		// �m�[�h�ʂ̏���\������
		RenderNode();
	}
}