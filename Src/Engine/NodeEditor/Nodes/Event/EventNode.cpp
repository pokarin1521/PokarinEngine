/**
* @file EventNode.cpp
*/
#include "EventNode.h"

namespace PokarinEngine
{
	/// <summary>
	/// 実行処理
	/// </summary>
	void EventNode::Run()
	{
		// ノード別の実行処理
		if (RunNode())
		{
			// リンク先のノードを実行
			outputRunPin->RunLinkNode();
		}
	}

	/// <summary>
	/// 初期化
	/// </summary>
	void EventNode::Initialize()
	{
		// 実行ピンを作成する
		outputRunPin = CreatePin<RunPin>("outputRunPin", PinAttribute::Output);

		// データピンを作成する
		CreateDataPin();
	}

	/// <summary>
	/// ピンを表示する
	/// </summary>
	void EventNode::RenderPin()
	{
		// 実行ピンを表示する
		outputRunPin->Begin();
		outputRunPin->End();

		// データピンを表示する
		RenderDataPin();
	}
}