/**
* @file EventNode.h
*/
#ifndef EVENTNODE_H_INCLUDED
#define EVENTNODE_H_INCLUDED

#include "../Node.h"

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

	private: // -------------------------- ノード制御 -------------------------

		/// <summary>
		/// ノード別の実行処理
		/// </summary>
		/// <returns>
		/// <para> true : 次のノードを実行する </para>
		/// <para> false : 次のノードを実行しない </para>
		/// </returns>
		virtual bool RunNode() override = 0;

		/// <summary>
		/// 初期化
		/// </summary>
		void Initialize() override
		{
			// 実行ピンを作成する
			runOutputPinID = CreatePin(PinType::Run);

			// データピンを作成する
			CreateDataPin();
		}

	private: // --------------------------- ピン制御 ---------------------------

		/// <summary>
		/// ピンを表示する
		/// </summary>
		void RenderPin() override
		{
			// 実行ピンを表示する
			BeginRunPin(runOutputPinID, PinAttribute::Output);
			EndPin(PinAttribute::Output);

			// データピンを表示する
			RenderDataPin();
		}

		/// <summary>
		/// データピンを作成する
		/// </summary>
		virtual void CreateDataPin() {}

		/// <summary>
		/// データピンを表示する
		/// </summary>
		virtual void RenderDataPin() {}

	private: // ---------------------------- 実行ピン ----------------------------

		int runOutputPinID = 0;
	};
}

#endif // !EVENTNODE_H_INCLUDED
