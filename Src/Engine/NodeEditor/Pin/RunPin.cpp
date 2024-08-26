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
	/// <param name="pin"> リンクさせるピン </param>
	void RunPin::LinkPin(Pin& pin)
	{
		// 自身とはリンクさせない
		if (&pin == this)
		{
			return;
		}

		// 自身が出力用ピンの場合は
		// リンクできるピンを1つだけにしたいので、リンク中のピンを削除する
		if (GetAttribute() == PinAttribute::Output)
		{
			linkList.clear();
		}

		// リンクしたピンを追加する
		linkList.emplace(&pin);
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

		GetOwnerNode().Run();
	}
}