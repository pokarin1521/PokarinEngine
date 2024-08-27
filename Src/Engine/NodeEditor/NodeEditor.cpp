/**
* @file NodeEditor.cpp
*/
#include "NodeEditor.h"

#include "Nodes/Event/EventNode.h"

#include "Nodes/Node.h"

#include "../GameObject.h"
#include "../Input.h"

namespace PokarinEngine
{
#pragma region NodeEditor

	/// <summary>
	/// コンストラクタ
	/// </summary>
	NodeEditor::NodeEditor(GameObject& gameObject)
		: ownerObject(&gameObject), name(gameObject.name)
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

#pragma endregion

#pragma region Node

	/// <summary>
	/// ノードを追加する
	/// </summary>
	/// <param name="[in] node"> 追加するノード </param>
	/// <returns> 追加したノードの識別番号 </returns>
	int NodeEditor::AddNode(const NodePtr& node)
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
	/// <param name="[in] node"> 削除するノード </param>
	void NodeEditor::DestroyNode(const NodePtr& node)
	{
		node->UnLinkAllPin();

		// 削除するノードの識別番号
		int nodeID = node->GetID();

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
			// 削除するノードの管理用配列
			std::vector<NodePtr> destroyNodeList;
			destroyNodeList.reserve(nodeList.size());

			// [ノード識別番号, ノード]
			for (auto& [nodeID, node] : nodeList)
			{
				// ノードを表示する
				node->Render();

				// ノードを選択したうえで、Deleteキーを押したら
				// 削除用配列に追加する
				if (ImNodes::IsNodeSelected(nodeID) &&
					Input::GetKeyDown(KeyCode::Delete))
				{
					destroyNodeList.push_back(node);
				}
			}

			// 配列に追加されている識別番号のノードを削除する
			for (auto& destroyNode : destroyNodeList)
			{
				DestroyNode(destroyNode);
			}

			// ピンのリンク状態を表示する
			RenderLink();

			ImNodes::EndNodeEditor();
		}
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
			const PinPtr inputPin = pinList[inputPinID];

			// 出力用ピン
			const PinPtr outputPin = pinList[outputPinID];

			// ピンの型が同じ場合のみリンクする
			if (inputPin->GetClass() == outputPin->GetClass())
			{
				// リンクするピンの組を追加
				AddLinkPair(LinkPair(inputPin, outputPin));
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
		// リンク済みのピン同士を線で繋げる
		for (const auto& [pinID, pin] : pinList)
		{
			// 入力用ピンから出力用ピンに線を繋げたいので、
			// 入力用ピンでないなら次のピンへ
			if (pin->GetAttribute() != PinAttribute::Input)
			{
				continue;
			}

			// リンク中のピン配列
			const auto& linkPinList = pin->GetLinkList();

			// [リンク識別番号, リンク中の出力用ピン]
			for (const auto& [linkID, outputPin] : linkPinList)
			{
				// リンク状態を可視化
				ImNodes::Link(linkID, pin->GetID(), outputPin->GetID());
			}
		}
	}

	/// <summary>
	/// リンクする組を追加する
	/// </summary>
	/// <param name="[in] linkPair"> 追加する組 </param>
	void NodeEditor::AddLinkPair(const LinkPair& linkPair)
	{
		// リンク識別番号
		int linkID = Random::Range(INT_MIN, INT_MAX);

		// 識別番号を追加する
		// 重複している場合は追加できないので再度番号を取得する
		while (!linkPairList.emplace(linkID, linkPair).second)
		{
			linkID = Random::Range(INT_MIN, INT_MAX);
		}

		// 入力用ピン
		const PinPtr inputPin = linkPair.first;

		// 出力用ピン
		const PinPtr outputPin = linkPair.second;

		// ピンのリンク処理を実行
		inputPin->Link(linkID, *outputPin);
		outputPin->Link(linkID, *inputPin);
	}

	/// <summary>
	/// 指定した組のリンクを削除する
	/// </summary>
	/// <param name="[in] linkID"> 削除するリンクの識別番号 </param>
	void NodeEditor::DestroyLink(int linkID)
	{
		// 入力用ピン
		const PinPtr inputPin = linkPairList[linkID].first;

		// 出力用ピン
		const PinPtr outputPin = linkPairList[linkID].second;

		// 削除するリンクが実行ピン同士のものなら
		// ピン同士のリンクを解除する
		if (inputPin->GetType() == PinType::Run)
		{
			inputPin->UnLink(linkID);
			outputPin->UnLink(linkID);
		}

		// リンクを削除
		linkPairList.erase(linkID);
	}

#pragma endregion

} // namespace PokarinEngine