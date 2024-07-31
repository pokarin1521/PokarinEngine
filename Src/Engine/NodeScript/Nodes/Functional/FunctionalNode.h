/**
* @file FunctionalNode.h
*/
#ifndef POKARINENGINE_FUNCTIONALNODE_H_INCLUDED
#define POKARINENGINE_FUNCTIONALNODE_H_INCLUDED

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

	private: // --------------------------- 実行処理 ---------------------------

		/// <summary>
		/// ノード別の実行処理
		/// </summary>
		/// <returns>
		/// <para> true : 次のノードを実行する </para>
		/// <para> false : 次のノードを実行しない </para>
		/// </returns>
		bool RunNode() override = 0;

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

#endif // !POKARINENGINE_FUNCTIONALNODE_H_INCLUDED
