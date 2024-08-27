/**
* @file Pin.cpp
*/
#include "Pin.h"

#include "ImGui/imnodes.h"

#include "../Nodes/Node.h"

namespace PokarinEngine
{
	/// <summary>
	/// ピンの表示を開始する
	/// </summary>
	void Pin::Begin() const
	{
		// ピンの形
		// 初期では値用ピンに使う円形を指定
		ImNodesPinShape pinShape = ImNodesPinShape_CircleFilled;

		// 実行用ピンなら三角形を指定
		if (GetType() == PinType::Run)
		{
			pinShape = ImNodesPinShape_TriangleFilled;
		}

		// 入力用ピン
		if (attribute == PinAttribute::Input)
		{
			ImNodes::BeginInputAttribute(id, pinShape);
		}
		// 出力用ピン
		else
		{
			ImNodes::BeginOutputAttribute(id, pinShape);
		}
	}

	/// <summary>
	/// ピンの表示を終了する
	/// </summary>
	void Pin::End() const
	{
		// 入力用ピン
		if (attribute == PinAttribute::Input)
		{
			ImNodes::EndInputAttribute();
		}
		// 出力用ピン
		else
		{
			ImNodes::EndOutputAttribute();
		}
	}

	/// <summary>
	/// ピン同士のリンクを解除する
	/// </summary>
	/// <param name="[in] linkID"> リンク識別番号 </param>
	void Pin::UnLink(int linkID)
	{
		// リンク解除
		linkList[linkID]->linkList.erase(linkID);
		linkList.erase(linkID);
	}

	/// <summary>
	/// 全てのピン同士のリンクを解除する
	/// </summary>
	void Pin::UnLinkAll()
	{
		// リンク配列のコピー
		// 配列の削除ができるようにコピーする
		LinkList copyLinkList = linkList;

		// リンク解除
		for (const auto& [id, linkPin] : copyLinkList)
		{
			UnLink(id);
		}
	}
}