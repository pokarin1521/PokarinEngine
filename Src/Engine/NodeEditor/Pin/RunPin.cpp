/**
* RunPin.cpp
*/
#include "RunPin.h"

#include "ImGui/imgui.h"

#include "../Nodes/Node.h"

namespace PokarinEngine
{
	/// <summary>
	/// ピン同士をリンクした時の処理
	/// </summary>
	/// <param name="[in] linkID"> リンク識別番号 </param>
	/// <param name="[in] pin"> リンクさせるピン </param>
	void RunPin::Link(int linkID, Pin& pin)
	{
		// 自身とはリンクさせない
		if (&pin == this)
		{
			return;
		}

		// 自身が出力用ピンの場合は
		// リンクできるピンを1つだけにしたいので、全てのリンクを解除する
		if (GetAttribute() == PinAttribute::Output)
		{
			UnLinkAll();
		}

		// リンクしたピンを追加する
		linkList.emplace(linkID, &pin);
	}

	/// <summary>
	/// <para> リンク先のノードを実行する </para>
	/// <para> 出力用ピンでなければ実行できない </para>
	/// </summary>
	void RunPin::RunLinkNode()
	{
		// 入力用ピンなら何もしない
		if (GetAttribute() == PinAttribute::Input)
		{
			return;
		}

		// 出力先のピンは最大1つなので、
		// リンク中のピンの持ち主であるノードの処理を実行する
		// 配列が空か確認するのが面倒なのでfor文で処理する
		for (const auto& [linkID, pin] : linkList)
		{
			pin->GetOwnerNode().Run();
		}
	}
}