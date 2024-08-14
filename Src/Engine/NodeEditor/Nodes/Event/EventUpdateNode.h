/**
* @file EventUpdateNode.h
*/
#ifndef POKARINENGINE_EVENTUPDATENODE_H_INCLUDED
#define POKARINENGINE_EVENTUPDATENODE_H_INCLUDED

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

	private: // ---------------------------- 実行処理 -----------------------------

		/// <summary>
		/// ノード別の実行処理
		/// </summary>
		/// <returns>
		/// <para> true : 次のノードを実行する </para>
		/// <para> false : 次のノードを実行しない </para>
		/// </returns>
		bool RunNode() override
		{
			// 次のノードを実行する
			return true;
		}
	};
}

#endif // !POKARINENGINE_EVENTUPDATENODE_H_INCLUDED
