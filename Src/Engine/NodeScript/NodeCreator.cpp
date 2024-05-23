/**
* @file NodeCreator.cpp
*/
#include "NodeEditor.h"

#include "Nodes/Event/EventUpdateNode.h"

#include "Nodes/Functional/AddWorldPositionNode.h"
#include "Nodes/Functional/AddWorldRotationNode.h"

namespace PokarinEngine
{
	/// <summary>
	/// �m�[�h�쐬�p�|�b�v�A�b�v�̏���
	/// </summary>
	void NodeEditor::CreateNodePopup()
	{
		CreateNodeButton<EventUpdateNode>("EventUpdate");
		CreateNodeButton<AddWorldPositionNode>("AddWorldPosition");
		CreateNodeButton<AddWorldRotationNode>("AddWorldRotation");
	}
}