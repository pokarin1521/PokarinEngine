/**
* @file EventUpdateNode.h
*/
#ifndef EVENTUPDATENODE_H_INCLUDED
#define EVENTUPDATENODE_H_INCLUDED

#include "EventNode.h"

namespace PokarinEngine
{
	/// <summary>
	/// 更新イベント用ノード
	/// </summary>
	class EventUpdateNode : public EventNode
	{
	public: // ----------------- コンストラクタ・デストラクタ --------------------

		EventUpdateNode() = default;
		~EventUpdateNode() = default;

	public: // ---------------------------- 実行処理 -----------------------------

		/// <summary>
		/// 実行処理
		/// </summary>
		void Run()
		{
			// 次のノードの処理を実行
			RunNextNode();
		}
	};
}

#endif // !EVENTUPDATENODE_H_INCLUDED
