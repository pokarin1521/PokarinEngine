/**
* @file Node.cpp
*/
#include "Node.h"

#include "ImGui/imgui.h"
#include "ImGui/imnodes.h"

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
	/// <param name="[in] nodeTitle"> ノードのタイトル </param>
	void Node::CreateNode(NodeEditor& nodeEditor, int nodeID, const std::string& nodeTitle)
	{
		// 持ち主であるノードエディタを設定
		ownerEditor = &nodeEditor;

		// ノードの識別番号を設定
		id = nodeID;

		// タイトルを設定する
		title = nodeTitle;

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
		ImNodes::BeginNodeTitleBar();
		ImGui::Text(title.c_str());
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