/**
* @file NodeEditor.cpp
*/
#include "NodeEditor.h"

#include "Json/Json.h"

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

			// ピンのリンク作成状態を更新する
			UpdateCreatedLink();

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
	/// 情報を全削除する
	/// </summary>
	void NodeEditor::Clear()
	{
		nodeList.clear();
		pinList.clear();
		linkPairList.clear();
	}

	/// <summary>
	/// 情報をJson型に格納する
	/// </summary>
	/// <param name="[out] json"> 情報を格納するJson型 </param>
	void NodeEditor::ToJson(Json& json) const
	{
		// ---------------------------------
		// ノード情報を格納する
		// ---------------------------------

		// ノード識別番号(文字列)の配列
		std::vector<std::string> nodeIDList_string;
		nodeIDList_string.reserve(nodeList.size());

		for (const auto& [noeID, node] : nodeList)
		{
			// ノード識別番号(文字列)
			const std::string id_string = node->GetID_String();

			// ノードの名前を格納
			json[id_string]["Name"] = node->GetName();

			// ノード別の情報を格納
			node->ToJson(json[id_string]);

			// 後でまとめて格納できるように
			// ノード識別番号(文字列)を配列に追加
			nodeIDList_string.push_back(id_string);
		}

		// ノード識別番号の配列を格納
		json["NodeIDList"] = nodeIDList_string;

		// --------------------------------
		// リンク情報を格納する
		// --------------------------------

		json["LinkPairList"] = linkPairList;
	}

	/// <summary>
	/// 情報をJson型から取得する
	/// </summary>
	/// <param name="[in] json"> 情報を格納しているJson型 </param>
	void NodeEditor::FromJson(const Json& json)
	{
		// --------------------------
		// 情報を全削除する
		// --------------------------

		Clear();

		// ----------------------------------
		// ノードの情報を取得する
		// ----------------------------------

		// ノード識別番号(文字列)の配列
		auto nodeIDList_string = json["NodeIDList"].get<std::vector<std::string>>();

		for (std::string id_string : nodeIDList_string)
		{
			// ノードの名前
			const auto nodeName = json[id_string]["Name"].get<std::string>();

			// 名前に対応したノードを追加する
			// ピン情報の取得ができるように格納時の識別番号をそのまま使う
			NodePtr node = createNodeFuncList[nodeName](*this, nodeName, std::stoi(id_string));

			// 追加したノードの情報をJson型から取得する
			node->FromJson(json[id_string]);
		}

		// -----------------------------------
		// ピン配列を更新する
		// -----------------------------------

		// ピンを追加し直す前のピン配列
		PinList beforePinList = pinList;

		// ピンを追加し直せるように全削除する
		pinList.clear();

		// ピンはノードを作成した時点で作成されるので、
		// 作成後に識別番号を再設定する
		// なので、ピン配列に追加し直すことで対応する
		for (auto& [pinID, pin] : beforePinList)
		{
			pinList.emplace(pin->GetID(), pin);
		}

		// --------------------------------------
		// リンク情報を取得する
		// --------------------------------------

		// リンク配列を取得する
		json["LinkPairList"].get_to(linkPairList);

		// ピン同士をリンク
		for (auto& [linkID, linkPair] : linkPairList)
		{
			LinkPin(linkID, linkPair);
		}
	}

#pragma endregion

#pragma region Node

	/// <summary>
	/// ノードを追加する
	/// </summary>
	/// <param name="[in] node"> 追加するノード </param>
	/// <param name="[in] nodeID"> 追加するノードの識別番号 </param>
	/// <returns> 追加したノードの識別番号 </returns>
	int NodeEditor::AddNode(const NodePtr& node, int nodeID)
	{
		// 識別番号
		int singleID = Random::Range(INT_MIN, INT_MAX);

		// 0以外が指定されているなら、指定された番号での追加を試みる
		if (nodeID != 0)
		{
			singleID = nodeID;
		}

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
	/// ピン同士のリンク作成状態を更新する
	/// </summary>
	void NodeEditor::UpdateCreatedLink()
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
				AddLinkPair(LinkPair(inputPinID, outputPinID));
			}
		}
	}

	/// <summary>
	/// ピン同士のリンク状態を表示する
	/// </summary>
	void NodeEditor::RenderLink()
	{
		// リンクの組をリセット
		linkPairList.clear();

		// リンク削除用配列
		std::vector<int> destroyLinkList;

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
				// 入力用ピンの識別番号
				const int inputPinID = pin->GetID();

				// 出力用ピンの識別番号
				const int outputPinID = outputPin->GetID();

				// リンクの組を追加
				linkPairList.emplace(linkID, LinkPair(inputPinID, outputPinID));

				// リンク状態を可視化
				ImNodes::Link(linkID, inputPinID, outputPinID);

				// deleteキーで選択中のリンクを削除用配列に追加する
				if (ImNodes::IsLinkSelected(linkID) &&
					Input::GetKeyDown(KeyCode::Delete))
				{
					destroyLinkList.push_back(linkID);
				}
			}
		}

		// 削除用配列にあるリンクを削除する
		for (int linkID : destroyLinkList)
		{
			int pinID = linkPairList[linkID].first;

			pinList[pinID]->UnLink(linkID);
		}
	}

	/// <summary>
	/// リンクする組を追加する
	/// </summary>
	/// <param name="[in] linkPair"> リンクの組 </param>
	void NodeEditor::AddLinkPair(const LinkPair& linkPair)
	{
		// リンク識別番号
		int linkID = Random::Range(INT_MIN, INT_MAX);

		// リンクの識別番号と組を追加する
		// 重複している場合は追加できないので再度番号を取得する
		while (!linkPairList.emplace(linkID, linkPair).second)
		{
			linkID = Random::Range(INT_MIN, INT_MAX);
		}

		// ピン同士をリンク
		LinkPin(linkID, linkPair);
	}

	/// <summary>
	/// ピン同士をリンクする
	/// </summary>
	/// <param name="[in] linkID"> リンク識別番号 </param>
	/// <param name="[in] linkPair"> リンクする組 </param>
	void NodeEditor::LinkPin(int linkID, const LinkPair& linkPair)
	{
		// 入力用ピン
		PinPtr inputPin = pinList[linkPair.first];

		// 出力用ピン
		PinPtr outputPin = pinList[linkPair.second];

		inputPin->Link(linkID, *outputPin);
		outputPin->Link(linkID, *inputPin);
	}

	/// <summary>
	/// 指定した組のリンクを解除する
	/// </summary>
	/// <param name="[in] linkID"> 解除するリンクの識別番号 </param>
	void NodeEditor::UnLinkPin(int linkID)
	{
		// リンクの組
		// <入力用ピンの識別番号, 出力用ピンの識別番号>
		LinkPair linkPair = linkPairList[linkID];

		// 入力用ピン
		const PinPtr inputPin = pinList[linkPair.first];

		// 出力用ピン
		const PinPtr outputPin = pinList[linkPair.second];

		// 削除するリンクが実行ピン同士のものなら
		// ピン同士のリンクを解除する
		if (inputPin->GetType() == PinType::Run)
		{
			inputPin->UnLink(linkID);
			outputPin->UnLink(linkID);
		}

		// 配列から削除する
		linkPairList.erase(linkID);
	}

#pragma endregion

} // namespace PokarinEngine