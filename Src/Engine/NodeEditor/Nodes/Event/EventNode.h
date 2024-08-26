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
		void Run() final override;

	private: // -------------------------- ノード制御 --------------------------

		/// <summary>
		/// ノード別の実行処理
		/// </summary>
		/// <returns>
		/// <para> true : 次のノードを実行する </para>
		/// <para> false : 次のノードを実行しない </para>
		/// </returns>
		virtual bool RunNode() = 0;

		/// <summary>
		/// 初期化
		/// </summary>
		void Initialize() final override;

	private: // --------------------------- ピン制御 ---------------------------

		/// <summary>
		/// ピンを表示する
		/// </summary>
		void RenderPin() final override;

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
		RunPinPtr runOutputPin;
	};
}

#endif // !POKARINENGINE_EVENTNODE_H_INCLUDED
