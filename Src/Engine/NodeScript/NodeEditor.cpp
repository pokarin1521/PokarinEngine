/**
* @file NodeEditor.cpp
*/
#include "NodeEditor.h"

#include "Nodes/TestNode.h"

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
	NodeEditor::NodeEditor(GameObject* ownerObject)
	{
		// 持ち主を設定
		owner = ownerObject;

		// ノードエディタの名前を設定
		// 分かりやすいように「持ち主の名前 + #持ち主の識別番号」にする
		name = owner->GetName();
		name += " #" + std::to_string(owner->GetID());

		nodeEditorContext = ImNodes::EditorContextCreate();
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

			// ノードエディタを作成
			ImNodes::BeginNodeEditor();
			{
				// 作成済みのノードを表示する
				for (auto& node : nodeList)
				{
					node->Render();
				}

				// リンク済みのピン同士を線で繋げる
				for (int i = 0; i < linkPairList.size(); ++i)
				{
					int _s = linkPairList[i].first;
					int _e = linkPairList[i].second;
					ImNodes::Link(i, _s, _e);
				}

				ImNodes::EndNodeEditor();
			}

			// ピン同士がリンクしたら配列に追加する
			int s = 0, e = 0;
			if (ImNodes::IsLinkCreated(&s, &e))
			{
				linkPairList.push_back(LinkPair(s, e));
			}

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
	void NodeEditor::CreateNode(NodePtr node)
	{
		// ノード配列に追加
		nodeList.emplace(node);

		// 重複しないノードの識別番号
		int nodeID = AddSingleID(nodeIDList);

		// ノードの識別番号を設定
		node->SetID_OnlyOnce(nodeID);

		// ノード作成時の処理を実行
		node->CreateNode(this);
	}

	/// <summary>
	/// ノード作成用ポップアップの処理
	/// </summary>
	void NodeEditor::CreateNodePopup()
	{
		CreateNodeButton<TestNode>();
	}

#pragma endregion

#pragma region SingleID

	/// <summary>
	/// ノードの入出力用ピンの識別番号を取得する
	/// </summary>
	/// <returns> 重複しない識別番号 </returns>
	int NodeEditor::GetSinglePinID()
	{
		return AddSingleID(nodePinIDList);
	}

	/// <summary>
	/// 重複しない識別番号を配列に追加する
	/// </summary>
	/// <param name="idList"> 識別番号を追加する配列 </param>
	/// <returns> 重複しない識別番号 </returns>
	int NodeEditor::AddSingleID(IDList& idList)
	{
		// 識別番号
		int singleID = Random::Range(INT_MIN, INT_MAX);

		// 識別番号を追加する
		// 重複している場合は再度番号を取得する
		while (!idList.emplace(singleID).second)
		{
			singleID = Random::Range(INT_MIN, INT_MAX);
		}

		return singleID;
	}

#pragma endregion

} // namespace PokarinEngine