/**
* @file UsingNode.h
*/
#ifndef USINGNODE_H_INCLUDED
#define USINGNODE_H_INCLUDED

#include <memory>
#include <unordered_set>

namespace PokarinEngine
{
	// -----------------------
	// �O���錾
	// -----------------------

	class Node;

	// -----------------------------
	// �^�̕ʖ����`
	// -----------------------------

	using NodePtr = std::shared_ptr<Node>;
	using NodeList = std::unordered_set<NodePtr>;
}

#endif // !USINGNODE_H_INCLUDED
