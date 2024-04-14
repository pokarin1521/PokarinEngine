/**
* @file Node.cpp
*/
#include "Node.h"

#include "ImGui/imgui.h"
#include "ImGui/imnodes.h"

#include "../NodeEditor.h"

#include "../../Window.h"
#include "../../InputManager.h"

namespace PokarinEngine
{
#pragma region Create, Render

	/// <summary>
	/// ノード作成時の処理
	/// </summary>
	/// <param name="nodeEditor"> 持ち主であるノードエディタ </param>
	void Node::CreateNode(NodeEditor* nodeEditor)
	{
		// 持ち主であるノードエディタを設定
		owner = nodeEditor;

		// マウスカーソルの位置にノードを設置する
		ImVec2 mousePos = Input::Mouse::GetScreenPos(WindowID::NodeScript);
		ImNodes::SetNodeScreenSpacePos(id, mousePos);

		// 初期化
		Initialize();
	}

	/// <summary>
	/// ノードを表示する
	/// </summary>
	void Node::Render()
	{
		ImNodes::BeginNode(id);
		RenderInfo();
		ImNodes::EndNode();
	}

#pragma endregion

#pragma region ID

	/// <summary>
	/// 未設定の場合だけノードの識別番号を設定する
	/// </summary>
	void Node::SetID_OnlyOnce(int setID)
	{
		// 設定済みなら何もしない
		if (isSetID)
		{
			return;
		}

		// 識別番号を設定
		id = setID;

		// 設定済みにする
		isSetID = true;
	}

	/// <summary>
	/// 重複しない入出力用ピンの識別番号を取得する
	/// </summary>
	/// <returns> 重複しない識別番号 </returns>
	int Node::GetSinglePinID()
	{
		return owner->GetSinglePinID();
	}

#pragma endregion

#pragma region SetNode

	/// <summary>
	/// ノードの入力用ピンを設定する
	/// </summary>
	/// <param name="pinID"> 設定するピンの識別番号 </param>
	/// <param name="pinName"> 設定するピンの名前 </param>
	void Node::SetInputPin(int pinID, const char* pinName)
	{
		ImNodes::BeginInputAttribute(pinID, ImNodesPinShape_CircleFilled);

		ImGui::Text(pinName);

		ImNodes::EndInputAttribute();
	}

	/// <summary>
	/// ノードの出力用ピンを設定する
	/// </summary>
	/// <param name="pinID"> 設定するピンの識別番号 </param>
	/// <param name="pinName"> 設定するピンの名前 </param>
	void Node::SetOutputPin(int pinID, const char* pinName)
	{
		ImNodes::BeginOutputAttribute(pinID, ImNodesPinShape_CircleFilled);

		ImGui::Text(pinName);

		ImNodes::EndOutputAttribute();
	}

	/// <summary>
	/// ノードの入出力用ピンを設定する
	/// </summary>
	/// <param name="inputID"> 入力用ピンの識別番号 </param>
	/// <param name="outputID"> 出力用ピンの識別番号 </param>
	/// <param name="inputName"> 入力用ピンの名前 </param>
	/// <param name="outputName"> 出力用ピンの名前 </param>
	void Node::SetInOutPin(int inputID, int outputID, const char* inputName, const char* outputName)
	{
		// 入力用ピンの名前と出力用ピンの名前の間隔
		static const float spacing = 20;

		// 入出力用ピンを同じ行に表示する
		SetInputPin(inputID, inputName);
		ImGui::SameLine(0, spacing);
		SetOutputPin(outputID, outputName);
	}

#pragma endregion
}