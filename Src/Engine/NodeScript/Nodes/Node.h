/**
* @file Node.h
*/
#ifndef NODE_H_INCLUDED
#define NODE_H_INCLUDED

#include <string>
#include <vector>
#include <memory>

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
	using NodeList = std::vector<NodePtr>;

	/// <summary>
	/// ノードの基底クラス
	/// </summary>
	class Node
	{
	public: // ------------------ コンストラクタ・デストラクタ -------------------

		Node() = default;
		virtual ~Node() = default;

	public: // ------------------------------ 情報 ------------------------------

		// ノードの名前
		std::string name = "";
	};

} // namespace PokarinEngine

#endif // !NODE_H_INCLUDED
