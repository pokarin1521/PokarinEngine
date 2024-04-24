/**
* @file FunctionalNode.h
*/
#ifndef FUNCTIONALNODE_H_INCLUDED
#define FUNCTIONALNODE_H_INCLUDED

#include "../Node.h"

namespace PokarinEngine
{
	/// <summary>
	/// 機能用ノードの基底クラス
	/// </summary>
	class FunctionalNode : public Node
	{
	public: // ---------------- コンストラクタ・デストラクタ -------------------

		FunctionalNode() = default;
		virtual ~FunctionalNode() = default;

	public: // --------------------------- 実行処理  ---------------------------

		/// <summary>
		/// ノードの実行処理
		/// </summary>
		virtual void Run() = 0;

	private: // --------------------------- ピン制御 ---------------------------

		/// <summary>
		/// 初期化
		/// </summary>
		void Initialize() override
		{
			// --------------------------------
			// 実行ピンを作成する
			// --------------------------------

			// 入力用
			runInputPinID = CreatePin(PinType::Run);

			// 出力用
			runOutputPinID = CreatePin(PinType::Run);

			// --------------------------------
			// データピンを作成する
			// --------------------------------

			CreateDataPin();
		}

		/// <summary>
		/// ピンを表示する
		/// </summary>
		void RenderPin() override
		{
			// ------------------------------
			// 実行ピンを表示する
			// ------------------------------

			// 入力用
			BeginRunPin(runInputPinID, PinAttribute::Input);
			EndPin(PinAttribute::Input);

			// 出力用
			BeginRunPin(runOutputPinID, PinAttribute::Output);
			EndPin(PinAttribute::Output);

			// -----------------------------
			// データピンを表示する
			// -----------------------------

			RenderDataPin();
		}

		/// <summary>
		/// データピンを作成する
		/// </summary>
		virtual void CreateDataPin() = 0;

		/// <summary>
		/// データピンを表示する
		/// </summary>
		virtual void RenderDataPin() = 0;

	private: // ---------------------------- 実行ピン ----------------------------

		int runInputPinID = 0;
		int runOutputPinID = 0;
	};
}

#endif // !FUNCTIONALNODE_H_INCLUDED
