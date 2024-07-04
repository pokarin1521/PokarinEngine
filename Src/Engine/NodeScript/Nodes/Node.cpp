/**
* @file Node.cpp
*/
#include "Node.h"

#include "ImGui/imgui.h"
#include "ImGui/imnodes.h"

#include "../NodeEditor.h"
#include "../Pin/Pin.h"

#include "../../Window.h"
#include "../../InputManager.h"

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
		Vector2 mousePos = Input::Mouse::GetScreenPos(WindowID::NodeScript);
		ImNodes::SetNodeScreenSpacePos(id, ImVec2(mousePos.x, mousePos.y));

		// 初期化
		Initialize();
	}

	/// <summary>
	/// 実行処理
	/// </summary>
	void Node::Run()
	{
		// ノード別の処理を実行
		if (RunNode())
		{
			// 次のノードが設定されているので、実行
			RunNextNode();
		}
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

#pragma region CreatePin

	/// <summary>
	/// ピンを作成する
	/// </summary>
	/// <param name="[in] pinType"> ピンの種類 </param>
	/// <returns> 作成したピンの識別番号 </returns>
	int Node::CreatePin(PinType pinType)
	{
		return ownerEditor->CreatePin(id, pinType);
	}

#pragma endregion

#pragma region RenderPin

	/// <summary>
	/// ピンの形
	/// </summary>
	enum class Node::PinShape
	{
		Triangle = ImNodesPinShape_TriangleFilled,
		Circle = ImNodesPinShape_CircleFilled,
	};

	/// <summary>
	/// ピンの表示を開始する
	/// </summary>
	/// <param name="[in] pinID"> ピンの識別番号 </param>
	/// <param name="[in] pinAttribute"> ピンの属性 </param>
	/// <param name="[in] pinShape"> ピンの形 </param>
	void Node::BeginPin(int pinID, PinAttribute pinAttribute, PinShape pinShape)
	{
		// 入力用ピン
		if (pinAttribute == PinAttribute::Input)
		{
			ImNodes::BeginInputAttribute(pinID, ImNodesPinShape(pinShape));
		}
		// 出力用ピン
		else
		{
			ImNodes::BeginOutputAttribute(pinID, ImNodesPinShape(pinShape));
		}
	}

	/// <summary>
	/// データピンの表示を開始する
	/// </summary>
	/// <param name="[in] pinID"> ピンの識別番号 </param>
	/// <param name="[in] pinAttribute"> ピンの属性 </param>
	void Node::BeginDataPin(int pinID, PinAttribute pinAttribute)
	{
		BeginPin(pinID, pinAttribute, PinShape::Circle);
	}

	/// <summary>
	/// 実行ピンの表示を開始する
	/// </summary>
	/// <param name="[in] pinID"> ピンの識別番号 </param>
	/// <param name="[in] pinAttribute"> ピンの属性 </param>
	void Node::BeginRunPin(int pinID, PinAttribute pinAttribute)
	{
		BeginPin(pinID, pinAttribute, PinShape::Triangle);
	}

	/// <summary>
	/// ピンの表示を終了する
	/// </summary>
	/// <param name="[in] pinAttribute"> ピンの属性 </param>
	void Node::EndPin(PinAttribute pinAttribute)
	{
		// 入力用ピン
		if (pinAttribute == PinAttribute::Input)
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
	/// 次に設定するピンを同じ行に表示する
	/// </summary>
	void Node::PinSameLin()
	{
		// ピン同士の間隔
		static const float spacing = 0;
		ImGui::SameLine(0, spacing);
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