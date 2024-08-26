/**
* @file FunctionalNode.cpp
*/
#include "FunctionalNode.h"

namespace PokarinEngine
{
	/// <summary>
	/// 実行処理
	/// </summary>
	void FunctionalNode::Run() 
	{
		if (RunNode())
		{
			runOutputPin->RunLinkNode();
		}
	}

	/// <summary>
	/// 初期化
	/// </summary>
	void FunctionalNode::Initialize() 
	{
		// --------------------------------
		// 実行ピンを作成する
		// --------------------------------

		// 入力用
		runInputPin = CreatePin<RunPin>(PinAttribute::Input);

		// 出力用
		runOutputPin = CreatePin<RunPin>(PinAttribute::Output);

		// --------------------------------
		// データピンを作成する
		// --------------------------------

		CreateDataPin();
	}

	/// <summary>
	/// ピンを表示する
	/// </summary>
	void FunctionalNode::RenderPin()
	{
		// ------------------------------
		// 実行ピンを表示する
		// ------------------------------

		// 入力用
		runInputPin->Begin();
		runInputPin->End();

		// 出力用
		runOutputPin->Begin();
		runOutputPin->End();

		// -----------------------------
		// データピンを表示する
		// -----------------------------

		RenderDataPin();
	}
}