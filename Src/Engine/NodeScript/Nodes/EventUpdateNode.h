/**
* @file EventUpdateNode.h
*/
#ifndef EVENTUPDATENODE_H_INCLUDED
#define EVENTUPDATENODE_H_INCLUDED

#include "EventNode.h"

namespace PokarinEngine
{
	/// <summary>
	/// 更新イベント用ノード(リンクしているノードを毎フレーム実行する)
	/// </summary>
	class EventUpdateNode : public EventNode
	{
	public: // ----------------- コンストラクタ・デストラクタ --------------------

		EventUpdateNode() = default;
		~EventUpdateNode() = default;

	private: // --------------------------- ノード表示 ----------------------------

		/// <summary>
		/// ノードを表示する
		/// </summary>
		void RenderNode() override
		{

		}
	};
}

#endif // !EVENTUPDATENODE_H_INCLUDED
