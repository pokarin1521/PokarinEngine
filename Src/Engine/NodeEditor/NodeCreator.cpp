/**
* @file NodeCreator.cpp
*/
#include "NodeEditor.h"

#include "Nodes/Event/EventUpdateNode.h"

#include "Nodes/Functional/AddWorldPositionNode.h"
#include "Nodes/Functional/AddWorldRotationNode.h"

namespace PokarinEngine
{
	NodeEditor::CreateNodeFuncList NodeEditor::createNodeFuncList = {
		{ "EventUpdate", &NodeEditor::CreateNode<EventUpdateNode>},
		{ "AddWorldPosition", &NodeEditor::CreateNode<AddWorldPositionNode> },
		{ "AddWorldRotation", &NodeEditor::CreateNode<AddWorldRotationNode> },
	};

	/// <summary>
	/// ノード作成用ポップアップの処理
	/// </summary>
	void NodeEditor::CreateNodePopup()
	{
		CreateNodeButton<EventUpdateNode>("EventUpdate");
		CreateNodeButton<AddWorldPositionNode>("AddWorldPosition");
		CreateNodeButton<AddWorldRotationNode>("AddWorldRotation");
	}
}