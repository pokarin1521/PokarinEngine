/**
* @file Node.cpp
*/
#include "Node.h"

#include "ImGui/imgui.h"
#include "ImGui/imnodes.h"

#include "Json/Json.h"

#include "../NodeEditor.h"
#include "../Pin/Pin.h"

#include "../../Window.h"
#include "../../Input.h"

#include "../../Color.h"

#include "../../GameObject.h"

namespace PokarinEngine
{
#pragma region Node

	/// <summary>
	/// ノード作成時の処理
	/// </summary>
	/// <param name="[in] nodeEditor"> 持ち主であるノードエディタ </param>
	/// <param name="[in] nodeID"> ノードの識別番号 </param>
	/// <param name="[in] nodeName"> ノードの名前 </param>
	void Node::CreateNode(NodeEditor& nodeEditor, int nodeID, const std::string& nodeName)
	{
		// 持ち主であるノードエディタを設定
		ownerEditor = &nodeEditor;

		// ノードの識別番号を設定
		id = nodeID;

		// タイトルを設定する
		name = nodeName;

		// マウスカーソルの位置にノードを設置
		Vector2 mousePos = Input::Mouse::GetScreenPos(WindowID::NodeEditor);
		ImNodes::SetNodeScreenSpacePos(id, ImVec2(mousePos.x, mousePos.y));

		// 初期化
		Initialize();
	}

	/// <summary>
	/// タイトルを表示する
	/// </summary>
	void Node::RenderTitle()
	{
		// 名前をタイトルとして表示する
		ImNodes::BeginNodeTitleBar();
		ImGui::Text(name.c_str());
		ImNodes::EndNodeTitleBar();
	}

	/// <summary>
	/// ノードを表示する
	/// </summary>
	void Node::Render()
	{
		ImNodes::BeginNode(id);
		RenderTitle();
		RenderPin();
		ImNodes::EndNode();
	}

	/// <summary>
	/// 持っている全てのピンのリンクを解除する
	/// </summary>
	void Node::UnLinkAllPin()
	{
		for (auto& pin : pinList)
		{
			pin->UnLinkAll();
		}
	}

	/// <summary>
	/// 情報をJson型に格納する
	/// </summary>
	/// <param name="[out] json"> 情報を格納するJson型 </param>
	void Node::ToJson(Json& json) const
	{
		// ピン情報を格納する
		for (auto& pin : pinList)
		{
			pin->ToJson(json[pin->GetName()]);
		}
	}

	/// <summary>
	/// 情報をJson型から取得する
	/// </summary>
	/// <param name="[in] json"> 情報を格納しているJson型 </param>
	void Node::FromJson(const Json& json)
	{
		// ピン情報を取得する
		for (auto& pin : pinList)
		{
			pin->FromJson(json[pin->GetName()]);
		}
	}

#pragma endregion

#pragma region GameObject

	/// <summary>
	/// ノードエディタの持ち主であるゲームオブジェクトを取得する
	/// </summary>
	/// <returns> 持ち主であるゲームオブジェクト </returns>
	GameObject& Node::GetOnwerObject()
	{
		return ownerEditor->GetOwnerObject();
	}

#pragma endregion
}