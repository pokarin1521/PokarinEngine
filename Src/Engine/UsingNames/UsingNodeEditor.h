/**
* @file UsingNodeEditor.h
*/
#ifndef USINGNODEEDITOR_H_INCLUDED
#define USINGNODEEDITOR_H_INCLUDED

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

#endif // !USINGNODEEDITOR_H_INCLUDED
