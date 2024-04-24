/**
* @file NodeEditor.h
*/
#ifndef NODEEDITOR_H_INCLUDED
#define NODEEDITOR_H_INCLUDED

#include "ImGui/imgui.h"
#include "ImGui/imgui_internal.h"

#include "ImGui/imnodes.h"
#include "ImGui/imnodes_internal.h"

#include "../UsingNames/UsingNodeEditor.h"

#include "Pin/PinType.h"

#include <string>
#include <unordered_set>
#include <unordered_map>
#include <memory>
#include <cassert>

namespace PokarinEngine
{
	// -----------------------
	// 前方宣言
	// -----------------------

	class GameObject;
	class Node;
	class EventNode;
	class Pin;

	/// <summary>
	/// ノードでオブジェクトに対する処理を編集する画面
	/// </summary>
	class NodeEditor
	{
	public: // -------------- コンストラクタ・デストラクタ --------------

		NodeEditor(GameObject& ownerObject);

		~NodeEditor()
		{
			ImNodes::EditorContextFree(nodeEditorContext);
		}

	public: // ------------------------- 制御 --------------------------

		/// <summary>
		/// ノードの処理を実行する
		/// </summary>
		void Run();

		/// <summary>
		/// 更新
		/// </summary>
		/// <returns>
		/// <para> true : ノードエディタのウィンドウが選択されている </para>
		/// <para> false : ノードエディタのウィンドウが選択されていない </para>
		/// </returns>
		bool Update();

	public: // ----------------- ノードの入出力用ピン ------------------

		/// <summary>
		/// ノードのピンを作成する
		/// </summary>
		/// <param name="pinID"> ピンの持ち主になるノードの識別番号 </param>
		/// <param name="pinType"> 作成するピンの種類 </param>
		/// <returns> 作成したピンの識別番号 </returns>
		int CreatePin(int nodeID, PinType pinType);

	public: // ---------------------- 情報の取得 -----------------------

		/// <summary>
		/// ImGuiウィンドウを取得する
		/// </summary>
		/// <returns> ImGuiウィンドウの参照 </returns>
		ImGuiWindow& GetImGuiWindow() const
		{
			return *imGuiWindow;
		}

		/// <summary>
		/// 名前を取得する
		/// </summary>
		/// <returns> ノードエディタの名前 </returns>
		const char* GetName() const
		{
			return name.c_str();
		}

		/// <summary>
		/// ノードエディタの持ち主を取得する
		/// </summary>
		/// <returns> 持ち主であるゲームオブジェクト </returns>
		GameObject& GetOwner()
		{
			return *owner;
		}

	public: // ---------------------- 状態の制御 -----------------------

		/// <summary>
		/// ノードエディタが開いているか取得する
		/// </summary>
		/// <returns>
		/// <para> true : 開いている </para>
		/// <para> false : 開いていない </para>
		/// </returns>
		bool IsOpen() const
		{
			return isOpen;
		}

		/// <summary>
		/// ノードエディタを開く
		/// </summary>
		void OpenEditor()
		{
			isOpen = true;
		}

	private: // ----------------------- 型の別名を定義 -------------------------

		// <入力用ピンの識別番号, 出力用ピンの識別番号>
		using LinkPair = std::pair<int, int>;

		// <リンク識別番号, リンクの組>
		using LinkPairList = std::unordered_map<int, LinkPair>;

		using NodePtr = std::shared_ptr<Node>;
		using EventNodePtr = std::shared_ptr<EventNode>;
		using PinPtr = std::shared_ptr<Pin>;

		// <識別番号, ノード>
		using NodeList = std::unordered_map<int, NodePtr>;
		using PinList = std::unordered_map<int, PinPtr>;

		using EventNodeList = std::unordered_set<EventNodePtr>;

	private: // ------------------------- ノード作成用 -------------------------

		/// <summary>
		/// ノード作成用ボタンの処理
		/// </summary>
		/// <typeparam name="T"> ノードクラス </typeparam>
		/// <param name="nodeTitle"> ノードのタイトル </param>
		template <class T>
		void CreateNodeButton(const std::string& nodeTitle)
		{
			// タイトルが設定されてるか確認
			assert(!nodeTitle.empty());

			// ノードのタイトル
			const char* title = nodeTitle.c_str();

			// ボタンを押したらノード作成
			if (ImGui::Button(title))
			{
				// ノード
				auto node = std::make_shared<T>();

				// イベントノードなら配列に追加する
				if constexpr (std::is_base_of_v<EventNode, T>)
				{
					eventNodeList.emplace(node);
				}

				// ノード作成時の処理を実行
				CreateNode(node, title);
			}
		}

		/// <summary>
		/// ノード作成時の処理
		/// </summary>
		/// <param name="node"> 作成したノード </param>
		/// <param name="nodeTitle"> 作成したノードのタイトル </param>
		void CreateNode(NodePtr node, const char* nodeTitle);

		/// <summary>
		/// ノード作成用ポップアップの処理
		/// </summary>
		void CreateNodePopup();

	private: // --------------------------- 識別番号 ---------------------------

		/// <summary>
		/// 重複しないノード識別番号を取得する
		/// </summary>
		/// <param name="node"> ノード </param>
		/// <returns> 重複しない識別番号 </returns>
		int GetSingleNodeID(NodePtr node);

	private: // ----------------------- ピン同士のリンク -----------------------

		/// <summary>
		/// ピン同士のリンク状態を更新する
		/// </summary>
		void UpdateLink();

		/// <summary>
		/// ピン同士のリンク状態を表示する
		/// </summary>
		void RenderLink();

		/// <summary>
		/// リンクする組を追加する
		/// </summary>
		/// <param name="linkPair"> 追加する組 </param>
		void AddLinkPair(const LinkPair& linkPair);

		/// <summary>
		/// 指定した組のリンクを削除する
		/// </summary>
		/// <param name="linkPairID"> 削除するリンクの識別番号 </param>
		void DestroyLink(int linkPairID);

	private: // ------------------------- ノード管理用 -------------------------

		// ノード管理用配列
		NodeList nodeList;

		// ピン管理用配列
		PinList pinList;

		// リンクしているピンの識別番号を管理する配列
		LinkPairList linkPairList;

	private: // --------------------- イベントノード管理用 ---------------------

		// イベントノード管理用配列
		EventNodeList eventNodeList;

	private: // --------------------- ノードエディタの情報 ---------------------

		// ノードエディタ用コンテキスト
		ImNodesEditorContext* nodeEditorContext = nullptr;

		// ImGuiウィンドウ
		ImGuiWindow* imGuiWindow = nullptr;

		// ノードエディタの持ち主
		GameObject* owner = nullptr;

		// ノードエディタの名前
		// ImGuiウィンドウ作成時のタイトルにする
		std::string name = "";

		// ノードエディタが開いているならtrue
		bool isOpen = false;

	private: // ------------------------- ノード作成用 -------------------------

		// ノード作成用ポップアップの名前
		const char* createNodePopup = "CreateNode";
	};

} // namespace PokarinEngine

#endif // !NODEEDITOR_H_INCLUDED

