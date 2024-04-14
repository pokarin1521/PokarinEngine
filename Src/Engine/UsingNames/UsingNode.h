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
	// ‘O•ûéŒ¾
	// -----------------------

	class Node;

	// -----------------------------
	// Œ^‚Ì•Ê–¼‚ğ’è‹`
	// -----------------------------

	using NodePtr = std::shared_ptr<Node>;
	using NodeList = std::unordered_set<NodePtr>;
}

#endif // !USINGNODE_H_INCLUDED
