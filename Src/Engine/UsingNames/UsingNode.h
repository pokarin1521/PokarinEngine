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
	// 前方宣言
	// -----------------------

	class Node;

	// -----------------------------
	// 型の別名を定義
	// -----------------------------

	using NodePtr = std::shared_ptr<Node>;
	using NodeList = std::unordered_set<NodePtr>;
}

#endif // !USINGNODE_H_INCLUDED
