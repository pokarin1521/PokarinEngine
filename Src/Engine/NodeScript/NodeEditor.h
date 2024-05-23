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

		NodeEditor(GameObject& gameObject);

		~NodeEditor()
		{
			Finalize();
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
		/// 持ち主であるゲームオブジェクトを取得する
		/// </summary>
		/// <returns> 持ち主であるゲームオブジェクト </returns>
		GameObject& GetOwnerObject()
		{
			return *ownerObject;
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

		// <識別番号, ピン>
		using PinList = std::unordered_map<int, PinPtr>;

		// <識別番号, イベントノード>
		using EventNodeList = std::unordered_map<int, EventNodePtr>;

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

			// ノード作成用ボタン
			if (ImGui::Button(nodeTitle.c_str()))
			{
				// 押されたらノード作成
				CreateNode<T>(nodeTitle);
			}
		}

		/// <summary>
		/// ノード作成用ボタンの処理
		/// </summary>
		/// <typeparam name="T"> ノードクラス </typeparam>
		/// <param name="nodeTitle"> ノードのタイトル </param>
		template <class T>
		void CreateNode(const std::string& nodeTitle)
		{
			// ノード
			auto node = std::make_shared<T>();

			// 重複しないノードの識別番号
			int nodeID = AddNode(node);

			// イベントノードなら配列に追加する
			if constexpr (std::is_base_of_v<EventNode, T>)
			{
				eventNodeList.emplace(nodeID, node);
			}

			// 作成時の処理を実行
			node->CreateNode(*this, nodeID, nodeTitle);
		}

		/// <summary>
		/// ノード作成用ポップアップの処理
		/// </summary>
		void CreateNodePopup();

	private: // ------------------------- ノードの追加 -------------------------

		/// <summary>
		/// ノードを追加する
		/// </summary>
		/// <param name="node"> 追加するノード </param>
		/// <returns> 追加したノードの識別番号 </returns>
		int AddNode(NodePtr node);

	private: // ------------------------- ノード削除用 -------------------------

		/// <summary>
		/// ノードを削除する
		/// </summary>
		/// <param name="nodeID"> 削除するノードの識別番号 </param>
		void DestroyNode(int nodeID);

	private: // ------------------------- ノード制御用 -------------------------

		/// <summary>
		/// エディタ内のノードの状態を更新する
		/// </summary>
		void UpdateNode();

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

	private: // --------------------------- 終了処理 ---------------------------

		/// <summary>
		/// 終了処理
		/// </summary>
		void Finalize();

	private: // ------------------------- ノード管理用 -------------------------

		// ノード管理用配列
		NodeList nodeList;

		// イベントノード管理用配列
		NodeList eventNodeList;

		// ピン管理用配列
		PinList pinList;

		// リンクしているピンの識別番号を管理する配列
		LinkPairList linkPairList;

	private: // --------------------- ノードエディタの情報 ---------------------

		// ノードエディタ用コンテキスト
		ImNodesEditorContext* nodeEditorContext = nullptr;

		// ImGuiウィンドウ
		ImGuiWindow* imGuiWindow = nullptr;

		// 持ち主であるゲームオブジェクト
		GameObject* ownerObject = nullptr;

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

