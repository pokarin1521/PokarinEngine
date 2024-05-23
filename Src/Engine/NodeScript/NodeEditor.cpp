/**
* @file NodeEditor.cpp
*/
#include "NodeEditor.h"

#include "Nodes/Event/EventNode.h"

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
	NodeEditor::NodeEditor(GameObject& gameObject)
		: ownerObject(&gameObject), name(gameObject.GetName())
	{
		// ノードエディタの名前を設定
		// 分かりやすいように「持ち主の名前 + #持ち主の識別番号」にする
		name += " #" + std::to_string(gameObject.GetID());

		// コンテキスト作成
		nodeEditorContext = ImNodes::EditorContextCreate();
	}

	/// <summary>
	/// ノードの処理を実行する
	/// </summary>
	void NodeEditor::Run()
	{
		for (auto& [nodeID, eventNode] : eventNodeList)
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
		// ウィンドウが選択されていないので何もしない
		if (ImGui::Begin(name.c_str(), &isOpen, ImGuiWindowFlags_NoSavedSettings))
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

			// エディタ内のノードの状態を更新する
			UpdateNode();

			// ピンのリンク状態を更新する
			UpdateLink();

			// ImGuiウィンドウを終了
			ImGui::End();

			// ウィンドウが選択されているのでtrueを返す
			return true;
		}

		// 選択されていなくてもBegin関数が呼ばれているので、
		// ImGuiウィンドウを終了する
		ImGui::End();

		// ウィンドウが選択されていないのでfalseを返す
		return false;
	}

	/// <summary>
	/// 終了処理
	/// </summary>
	void NodeEditor::Finalize()
	{
		// ノードを全削除
		nodeList.clear();
		eventNodeList.clear();

		// ピンを全削除
		pinList.clear();

		// リンクを全削除
		linkPairList.clear();
	}

#pragma endregion

#pragma region Node

	/// <summary>
	/// ノードを追加する
	/// </summary>
	/// <param name="node"> 追加するノード </param>
	/// <returns> 追加したノードの識別番号 </returns>
	int NodeEditor::AddNode(NodePtr node)
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

	/// <summary>
	/// ノードを削除する
	/// </summary>
	/// <param name="nodeID"> 削除するノードの識別番号 </param>
	void NodeEditor::DestroyNode(int nodeID)
	{
		// 登録されていない識別番号なら何もしない
		if (nodeList.find(nodeID) == nodeList.end())
		{
			return;
		}

		eventNodeList.erase(nodeID);
		nodeList.erase(nodeID);
	}

	/// <summary>
	/// エディタ内のノードの状態を更新する
	/// </summary>
	void NodeEditor::UpdateNode()
	{
		// ノードエディタを作成
		ImNodes::BeginNodeEditor();
		{
			// 削除するノードの識別番号の管理用配列
			std::vector<int> destroyNodeIDList;
			destroyNodeIDList.reserve(nodeList.size());

			// [ノード識別番号, ノード]
			for (auto& [nodeID, node] : nodeList)
			{
				// ノードを表示する
				node->Render();

				// ノードを選択したうえで、Deleteキーを押したら
				// 削除用配列に識別番号を追加
				if (ImNodes::IsNodeSelected(nodeID) &&
					Input::GetKeyDown(KeyCode::Delete))
				{
					destroyNodeIDList.push_back(nodeID);
				}
			}

			// 配列に追加されている識別番号のノードを削除する
			for (auto destroyNodeID : destroyNodeIDList)
			{
				DestroyNode(destroyNodeID);
			}

			// ピンのリンク状態を表示する
			RenderLink();

			ImNodes::EndNodeEditor();
		}
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