/**
* @file UsingNodeEditor.h
*/
#ifndef POKARINENGINE_USINGNODEEDITOR_H_INCLUDED
#define POKARINENGINE_USINGNODEEDITOR_H_INCLUDED

#include <memory>

namespace PokarinEngine
{
	// -----------------------
	// �O���錾
	// -----------------------
	
	class NodeEditor;

	// --------------------------------
	// �^�̕ʖ����`
	// --------------------------------

	using NodeEditorPtr = std::shared_ptr<NodeEditor>;
}

#endif // !POKARINENGINE_USINGNODEEDITOR_H_INCLUDED
