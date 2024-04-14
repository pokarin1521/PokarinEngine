/**
* @file EventNode.cpp
*/
#include "EventNode.h"

#include "ImGui/imnodes.h"

namespace PokarinEngine
{
	/// <summary>
	/// ノード情報を表示する
	/// </summary>
	void EventNode::RenderInfo()
	{
		// タイトルを設定
		ImNodes::BeginNodeTitleBar();
		ImGui::Text(name);
		ImNodes::EndNodeTitleBar();

		/* イベントノードは1番最初に使われるノードなので、
		リンク用ピンは出力だけにする */

		// リンク用出力ピンを設定
		ImNodes::BeginOutputAttribute(linkOutputPin, ImNodesPinShape_TriangleFilled);
		ImNodes::EndOutputAttribute();

		// ノード別の情報を表示する
		RenderNode();
	}
}