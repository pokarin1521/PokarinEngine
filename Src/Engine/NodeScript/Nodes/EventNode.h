/**
* @file EventNode.h
*/
#ifndef EVENTNODE_H_INCLUDED
#define EVENTNODE_H_INCLUDED

#include "Node.h"

namespace PokarinEngine
{
	/// <summary>
	/// イベント制御用ノードの基底クラス
	/// </summary>
	class EventNode : public Node
	{
	public: // ----------------- コンストラクタ・デストラクタ --------------------

		EventNode() = default;
		virtual ~EventNode() = default;

	public: // ---------------------------- 初期化 -------------------------------

		/// <summary>
		/// 初期化
		/// </summary>
		void Initialize() override
		{
			linkOutputPin = GetSinglePinID();
		}

	public: // ----------------------------- 名前 --------------------------------

		inline static const char* name = "No Title";

	private: // ----------------------------- 制御 -------------------------------

		/// <summary>
		/// ノード別の情報を表示する
		/// </summary>
		virtual void RenderNode() = 0;

		/// <summary>
		/// ノード情報を表示する
		/// </summary>
		void RenderInfo() override;

	private: // ----------------------------- 情報 -------------------------------

		// リンク用出力ピンの識別番号
		int linkOutputPin = 0;
	};
}

#endif // !EVENTNODE_H_INCLUDED
