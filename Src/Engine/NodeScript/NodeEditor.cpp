/**
* @file NodeEditor.cpp
*/
#include "NodeEditor.h"

#include "Nodes/Event/EventUpdateNode.h"
#include "Nodes/Functional/AddRotationNode.h"

#include "Nodes/Node.h"
#include "Pin/Pin.h"

#include "../GameObject.h"
#include "../Random.h"
#include "../InputManager.h"

namespace PokarinEngine
{
#pragma region NodeEditor

	/// <summary>
	/// コンストラクタ
	/// </summary>
	/// <param name="object"> 持ち主になるゲームオブジェクト </param>
	NodeEditor::NodeEditor(GameObject& ownerObject)
		: owner(&ownerObject), name(owner->GetName())
	{
		// ノードエディタの名前を設定
		// 分かりやすいように「持ち主の名前 + #持ち主の識別番号」にする
		name += " #" + std::to_string(owner->GetID());

		nodeEditorContext = ImNodes::EditorContextCreate();
	}

	/// <summary>
	/// ノードの処理を実行する
	/// </summary>
	void NodeEditor::Run()
	{
		for (auto& eventNode : eventNodeList)
		{
			eventNode->Run();
		}
	}

	/// <summary>
	/// 更新
	/// </summary>
	/// <returns>
	/// <para> true : ノードエディタのウィンドウが選択されている </para>
	/// <para> false : ノードエディタのウィンドウが選択されていない </para>
	/// </returns>
	bool NodeEditor::Update()
	{
		// ウィンドウが開いてないなら何もしない
		if (!isOpen)
		{
			return false;
		}

		// ImGuiウィンドウを作成
		// ウィンドウが選択された時に処理する
		if (ImGui::Begin(name.c_str(), &isOpen))
		{
			// ImGuiウィンドウを登録
			if (!imGuiWindow)
			{
				imGuiWindow = ImGui::GetCurrentWindow();
			}

			// ノードエディタ用コンテキストの使用を開始
			ImNodes::EditorContextSet(nodeEditorContext);

			// 右クリックでノード作成用ポップアップを展開する
			if (Input::GetKeyUp(KeyCode::MouseRight))
			{
				ImGui::OpenPopup(createNodePopup);
			}

			// ポップアップ展開中に処理を実行する
			if (ImGui::BeginPopup(createNodePopup))
			{
				CreateNodePopup();

				ImGui::EndPopup();
			}

			// 入出力ピンの識別番号
			// ピンのリンク判定用
			static int inputPinID = 0, outputPinID = 0;

			// ノードエディタを作成
			ImNodes::BeginNodeEditor();
			{
				// 作成済みのノードを表示する
				for (auto& [nodeID, node] : nodeList)
				{
					node->Render();
				}

				// ピンのリンク状態を表示する
				RenderLink();

				ImNodes::EndNodeEditor();
			}

			// ピンのリンク状態を更新する
			UpdateLink();

			ImGui::End();

			// ウィンドウが選択されているのでtrueを返す
			return true;
		}

		// 選択状態に関わらずBegin関数が呼ばれているので
		// End関数を呼ぶ
		ImGui::End();

		// ウィンドウが選択されてないのでfalseを返す
		return false;
	}

#pragma endregion

#pragma region CreateNode

	/// <summary>
	/// ノード作成時の処理
	/// </summary>
	/// <param name="node"> 作成したノード </param>
	/// <param name="nodeTitle"> 作成したノードのタイトル </param>
	void NodeEditor::CreateNode(NodePtr node, const char* nodeTitle)
	{
		// 重複しないノードの識別番号
		int nodeID = GetSingleNodeID(node);

		// 作成時の処理を実行
		node->CreateNode(*this, nodeID, nodeTitle);
	}

	/// <summary>
	/// ノード作成用ポップアップの処理
	/// </summary>
	void NodeEditor::CreateNodePopup()
	{
		CreateNodeButton<AddRotationNode>("AddRotation");
		CreateNodeButton<EventUpdateNode>("EventUpdate");
	}

#pragma endregion

#pragma region CreatePin

	/// <summary>
	/// ノードのピンを作成する
	/// </summary>
	/// <param name="pinID"> ピンの持ち主になるノードの識別番号 </param>
	/// <param name="pinType"> 作成するピンの種類 </param>
	/// <returns> 作成したピンの識別番号 </returns>
	int NodeEditor::CreatePin(int nodeID, PinType pinType)
	{
		// 識別番号
		int singleID = Random::Range(INT_MIN, INT_MAX);

		// 識別番号を追加する
		// 重複している場合は追加できないので再度番号を取得する
		while (!pinList.emplace(singleID, nullptr).second)
		{
			singleID = Random::Range(INT_MIN, INT_MAX);
		}

		// ピンを作成して登録する
		pinList[singleID] = std::make_shared<Pin>(nodeID, singleID, pinType);

		// 作成したピンの識別番号を返す
		return singleID;
	}

#pragma endregion

#pragma region ID

	/// <summary>
	/// 重複しないノード識別番号を取得する
	/// </summary>
	/// <param name="node"> ノード </param>
	/// <returns> 重複しない識別番号 </returns>
	int NodeEditor::GetSingleNodeID(NodePtr node)
	{
		// 識別番号
		int singleID = Random::Range(INT_MIN, INT_MAX);

		// 識別番号を追加する
		// 重複している場合は追加できないので再度番号を取得する
		while (!nodeList.emplace(singleID, node).second)
		{
			singleID = Random::Range(INT_MIN, INT_MAX);
		}

		return singleID;
	}

#pragma endregion

#pragma region Link

	/// <summary>
	/// ピン同士のリンク状態を更新する
	/// </summary>
	void NodeEditor::UpdateLink()
	{
		// ピンの識別番号
		static int inputPinID = 0, outputPinID = 0;

		// ピン同士がリンクしたら配列に追加する
		if (ImNodes::IsLinkCreated(&inputPinID, &outputPinID))
		{
			// 入力用ピン
			const Pin& inputPin = *pinList[inputPinID];

			// 出力用ピン
			const Pin& outputPin = *pinList[outputPinID];

			// ピンの種類が同じ場合のみリンクを許可する
			if (inputPin.GetType() == outputPin.GetType())
			{
				// リンクするピンの組を追加
				AddLinkPair(LinkPair(inputPinID, outputPinID));
			}
		}

		// マウスカーソルと重なっているリンク線の識別番号
		static int hoveredLinkID = 0;

		// Altキーを入力しながらリンク線を左クリックすることで
		// リンクを解除する
		if (ImNodes::IsLinkHovered(&hoveredLinkID))
		{
			if (Input::GetKey(KeyCode::LeftAlt) &&
				Input::GetKeyDown(KeyCode::MouseLeft))
			{
				DestroyLink(hoveredLinkID);
			}
		}
	}

	/// <summary>
	/// ピン同士のリンク状態を表示する
	/// </summary>
	void NodeEditor::RenderLink()
	{
		// ピンの識別番号
		static int inputPinID = 0, outputPinID = 0;

		// リンク済みのピン同士を線で繋げる
		for (auto& [linkID, linkPair] : linkPairList)
		{
			// ピンの識別番号を取得
			inputPinID = linkPair.first;
			outputPinID = linkPair.second;

			// リンク状態を可視化
			ImNodes::Link(linkID, inputPinID, outputPinID);
		}
	}

	/// <summary>
	/// リンクする組を追加する
	/// </summary>
	/// <param name="linkPair"> 追加する組 </param>
	void NodeEditor::AddLinkPair(const LinkPair& linkPair)
	{
		// 識別番号
		int singleID = Random::Range(INT_MIN, INT_MAX);

		// 識別番号を追加する
		// 重複している場合は追加できないので再度番号を取得する
		while (!linkPairList.emplace(singleID, linkPair).second)
		{
			singleID = Random::Range(INT_MIN, INT_MAX);
		}

		// 入力用ピン
		const Pin& inputPin = *pinList[linkPair.first];

		// 出力用ピン
		const Pin& outputPin = *pinList[linkPair.second];

		// 追加する組が実行ピン同士
		if (inputPin.GetType() == PinType::Run)
		{
			// 入力側のノード
			Node& inputNode = *nodeList[inputPin.GetNodeID()];

			// 出力側のノード
			Node* outputNode = nodeList[outputPin.GetNodeID()].get();

			// 入力側の次に実行するノードとして
			// 出力側のノードを登録する
			inputNode.SetNextNode(outputNode);
		}
	}

	/// <summary>
	/// 指定した組のリンクを削除する
	/// </summary>
	/// <param name="linkPairID"> 削除するリンクの識別番号 </param>
	void NodeEditor::DestroyLink(int linkPairID)
	{
		// ピンの識別番号
		int pinID = linkPairList[linkPairID].first;

		// ピン
		const Pin& pin = *pinList[pinID];

		// 削除するリンクが実行ピン同士のものなら
		// 次に実行するノードの設定を解除する
		if (pin.GetType() == PinType::Run)
		{
			Node& node = *nodeList[pin.GetNodeID()];
			node.SetNextNode(nullptr);
		}

		// リンクを削除
		linkPairList.erase(linkPairID);
	}

#pragma endregion

} // namespace PokarinEngine