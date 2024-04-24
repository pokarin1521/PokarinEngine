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
	/// <param name="nodeEditor"> 持ち主であるノードエディタ </param>
	/// <param name="nodeID"> ノードの識別番号 </param>
	void Node::CreateNode(NodeEditor& nodeEditor, int nodeID, const char* nodeTitle)
	{
		// 持ち主であるノードエディタを設定
		owner = &nodeEditor;

		// ノードの識別番号を設定
		id = nodeID;

		// タイトルを設定する
		title = nodeTitle;

		// マウスカーソルの位置にノードを設置
		ImVec2 mousePos = Input::Mouse::GetScreenPos(WindowID::NodeScript);
		ImNodes::SetNodeScreenSpacePos(id, mousePos);

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

#pragma region CreatePin

	/// <summary>
	/// ピンを作成する
	/// </summary>
	/// <param name="pinType"> ピンの種類 </param>
	/// <returns> 作成したピンの識別番号 </returns>
	int Node::CreatePin(PinType pinType)
	{
		return owner->CreatePin(id, pinType);
	}

#pragma endregion

#pragma region SetPin

	/// <summary>
	/// ピンの形
	/// </summary>
	enum class Node::PinShape
	{
		Triangle = ImNodesPinShape_TriangleFilled,
		Circle = ImNodesPinShape_CircleFilled,
	};

	/// <summary>
	/// ピンの設定を開始する
	/// </summary>
	/// <param name="pinID"> ピンの識別番号 </param>
	/// <param name="pinAttribute"> ピンの属性 </param>
	/// <param name="pinShape"> ピンの形 </param>
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
	/// データピンの設定を開始する
	/// </summary>
	/// <param name="pinID"> ピンの識別番号 </param>
	/// <param name="pinAttribute"> ピンの属性 </param>
	void Node::BeginDataPin(int pinID, PinAttribute pinAttribute)
	{
		BeginPin(pinID, pinAttribute, PinShape::Circle);
	}

	/// <summary>
	/// 実行ピンの設定を開始する
	/// </summary>
	/// <param name="pinID"> ピンの識別番号 </param>
	/// <param name="pinAttribute"> ピンの属性 </param>
	void Node::BeginRunPin(int pinID, PinAttribute pinAttribute)
	{
		BeginPin(pinID, pinAttribute, PinShape::Triangle);
	}

	/// <summary>
	/// ピンの設定を終了する
	/// </summary>
	/// <param name="pinAttribute"> ピンの属性 </param>
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
	GameObject& Node::GetGameObject()
	{
		return owner->GetOwner();
	}

#pragma endregion
}