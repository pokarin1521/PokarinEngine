/**
* @file EventNode.h
*/
#ifndef POKARINENGINE_EVENTNODE_H_INCLUDED
#define POKARINENGINE_EVENTNODE_H_INCLUDED

#include "../Node.h"

#include "../../Pin/RunPin.h"

namespace PokarinEngine
{
	/// <summary>
	/// イベント制御用ノードの基底クラス
	/// </summary>
	class EventNode : public Node
	{
	public: // ---------------- コンストラクタ・デストラクタ -------------------

		EventNode() = default;
		virtual ~EventNode() = default;

	public: // ------------------------------ 実行 -----------------------------

		/// <summary>
		/// 実行処理
		/// </summary>
		void Run() override final;

	private: // -------------------------- ノード制御 --------------------------

		/// <summary>
		/// ノード別の実行処理
		/// </summary>
		/// <returns> 次のノードを実行するならtrue </returns>
		virtual bool RunNode() = 0;

		/// <summary>
		/// 初期化
		/// </summary>
		void Initialize() override final;

	private: // --------------------------- ピン制御 ---------------------------

		/// <summary>
		/// ピンを表示する
		/// </summary>
		void RenderPin() override final;

		/// <summary>
		/// データピンを作成する
		/// </summary>
		virtual void CreateDataPin() {}

		/// <summary>
		/// データピンを表示する
		/// </summary>
		virtual void RenderDataPin() {}

	private: // ---------------------------- 実行ピン ----------------------------

		// 出力用実行ピン
		RunPinPtr outputRunPin;
	};
}

#endif // !POKARINENGINE_EVENTNODE_H_INCLUDED
