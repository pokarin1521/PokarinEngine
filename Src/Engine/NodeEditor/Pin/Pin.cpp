/**
* @file Pin.cpp
*/
#include "Pin.h"

#include "ImGui/imnodes.h"

#include "Json/Json.h"

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
		// 解除済みのリンクなら何もしない
		if (linkList.find(linkID) == linkList.end())
		{
			return;
		}

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

	/// <summary>
	/// 情報をJson型に格納する
	/// </summary>
	/// <param name="[out] json"> 情報を格納するJson型 </param>
	void Pin::ToJson(Json& json) const
	{
		// 識別番号を格納する
		json["ID"] = id;

		// ピン別の情報を格納する
		PinToJson(json);
	}

	/// <summary>
	/// 情報をJson型から取得する
	/// </summary>
	/// <param name="[in] json"> 情報を格納しているJson型 </param>
	void Pin::FromJson(const Json& json)
	{
		// 識別番号を取得する
		json["ID"].get_to(id);

		// ピン別の情報を取得する
		PinFromJson(json);
	}
}