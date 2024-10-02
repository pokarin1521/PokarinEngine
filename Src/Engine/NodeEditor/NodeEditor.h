/**
* @file NodeEditor.h
*/
#ifndef POKARINENGINE_NODEEDITOR_H_INCLUDED
#define POKARINENGINE_NODEEDITOR_H_INCLUDED

#include "ImGui/imgui.h"
#include "ImGui/imgui_internal.h"

#include "ImGui/imnodes.h"
#include "ImGui/imnodes_internal.h"

#include "Json/UsingNameJson.h"

#include "../UsingNames/UsingNodeEditor.h"

#include "../Random.h"

#include "Pin/Pin.h"

#include <string>
#include <memory>
#include <unordered_set>
#include <unordered_map>
#include <functional>

namespace PokarinEngine
{
	// -----------------------
	// 前方宣言
	// -----------------------

	class GameObject;
	class Node;
	class EventNode;

	/// <summary>
	/// ノードでオブジェクトに対する処理を編集する画面
	/// </summary>
	class NodeEditor
	{
	public: // ----------------- コンストラクタ・デストラクタ ------------------

		NodeEditor(GameObject& gameObject);

		~NodeEditor()
		{
			ImNodes::EditorContextFree(nodeEditorContext);
		}

	public: // --------------------------- 禁止事項 ----------------------------

		/* 持ち主が同じノードエディタが複数あると困るので、禁止する */

		// コピーコンストラクタの禁止
		NodeEditor(const NodeEditor&) = delete;

		// 代入の禁止
		NodeEditor& operator=(const NodeEditor&) = delete;

	public: // ----------------------------- 制御 ------------------------------

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

	public: // ---------------------- ノードの入出力用ピン ---------------------

		/// <summary>
		/// ノードのピンを作成する
		/// </summary>
		/// <typeparam name="T"> 作成するピンクラス </typeparam>
		/// <param name="[in] node"> ピンの持ち主になるノード </param>
		/// <param name="[in] pinName"> ピンの名前 </param>
		/// <param name="[in] pinAttribute"> ピンの入出力属性 </param>
		/// <returns> 作成したピンの識別番号 </returns>
		template <class T>
		std::shared_ptr<T> CreatePin(Node& node, const std::string& pinName, PinAttribute pinAttribute)
		{
			// ピン以外ならnullptr
			if constexpr (!std::is_base_of_v<Pin, T>)
			{
				return nullptr;
			}

			// ピン識別番号
			int pinID = Random::Range(INT_MIN, INT_MAX);

			// ピン識別番号を追加する
			// 重複している場合は追加できないので再度番号を取得する
			while (!pinList.emplace(pinID, nullptr).second)
			{
				pinID = Random::Range(INT_MIN, INT_MAX);
			}

			// ピンを作成して追加する
			auto pin = std::make_shared<T>(node, pinID, pinName, pinAttribute);
			pinList[pinID] = pin;

			// 作成したピンを返す
			return pin;
		}

	public: // -------------------------- 情報の取得 ---------------------------

		/// <summary>
		/// ImGuiウィンドウを取得する
		/// </summary>
		/// <returns> ImGuiウィンドウの参照 </returns>
		ImGuiWindow& GetImGuiWindow() const
		{
			return *imGuiWindow;
		}

		/// <summary>
		/// 持ち主であるゲームオブジェクトを取得する
		/// </summary>
		/// <returns> 持ち主であるゲームオブジェクト </returns>
		GameObject& GetOwnerObject()
		{
			return *ownerObject;
		}

		/// <summary>
		/// 名前を取得する
		/// </summary>
		/// <returns> ノードエディタの名前 </returns>
		const char* GetName() const
		{
			return name.c_str();
		}

	public: // -------------------------- 名前の更新 ---------------------------

		/// <summary>
		/// ノードエディタの名前を更新する(ゲームオブジェクトの名前を変更するときに呼び出す)
		/// </summary>
		void UpdateName();

	public: // -------------------------- 状態の制御 ---------------------------

		/// <summary>
		/// ノードエディタが開いているか取得する
		/// </summary>
		/// <returns> ノードエディタが開いているならtrue </returns>
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

	public: // ----------------------------- Json ------------------------------

		/// <summary>
		/// 情報をJson型に格納する
		/// </summary>
		/// <param name="[out] json"> 情報を格納するJson型 </param>
		void ToJson(Json& json) const;

		/// <summary>
		/// 情報をJson型から取得する
		/// </summary>
		/// <param name="[in] json"> 情報を格納しているJson型 </param>
		void FromJson(const Json& json);

	private: // ----------------------- 型の別名を定義 -------------------------

		// <入力用ピンの識別番号, 出力用ピンの識別番号>
		using LinkPair = std::pair<int, int>;

		// <リンク識別番号, <入力用ピンの識別番号, 出力用ピンの識別番号>>
		using LinkPairList = std::unordered_map<int, LinkPair>;

		using EventNodePtr = std::shared_ptr<EventNode>;

		using NodePtr = std::shared_ptr<Node>;

		// <識別番号, ノード>
		using NodeList = std::unordered_map<int, NodePtr>;

		// <識別番号, ピン>
		using PinList = std::unordered_map<int, PinPtr>;

		// <識別番号, イベントノード>
		using EventNodeList = std::unordered_map<int, EventNodePtr>;

		// <ノードの名前, ノード作成用関数>
		using CreateNodeFuncList = std::unordered_map<
			std::string, std::function<NodePtr(NodeEditor&, const std::string&, int)>>;

	private: // ------------------------- ノード作成用 -------------------------

		/// <summary>
		/// ノード作成用ボタンの処理
		/// </summary>
		/// <typeparam name="T"> ノードクラス </typeparam>
		/// <param name="[in] nodeName"> ノードの名前 </param>
		template <class T>
		void CreateNodeButton(const std::string& nodeName)
		{
			// ノード作成用ボタン
			// ポップアップならメニュー用のボタンの方が見栄えがいいので、こちらを使う
			if (ImGui::MenuItem(nodeName.c_str()))
			{
				// 押されたらノード作成
				CreateNode<T>(nodeName);

				// ポップアップを閉じる
				ImGui::CloseCurrentPopup();
			}
		}

		/// <summary>
		/// ノード作成用ボタンの処理
		/// </summary>
		/// <typeparam name="T"> ノードクラス </typeparam>
		/// <param name="[in] nodeName"> ノードの名前 </param>
		template <class T>
		NodePtr CreateNode(const std::string& nodeName, int nodeID = 0)
		{
			// ノード
			auto node = std::make_shared<T>();

			// 指定された識別番号での追加を試みて
			// 実際に設定された識別番号を代入する
			nodeID = AddNode(node, nodeID);

			// イベントノードなら配列に追加する
			if constexpr (std::is_base_of_v<EventNode, T>)
			{
				eventNodeList.emplace(nodeID, node);
			}

			// 作成時の処理を実行
			node->CreateNode(*this, nodeID, nodeName);

			return node;
		}

		/// <summary>
		/// ノード作成用ポップアップの処理
		/// </summary>
		void CreateNodePopup();

	private: // ------------------------- ノードの追加 -------------------------

		/// <summary>
		/// ノードを追加する
		/// </summary>
		/// <param name="[in] node"> 追加するノード </param>
		/// <param name="[in] nodeID"> 追加するノードの識別番号 </param>
		/// <returns> 追加したノードの識別番号 </returns>
		int AddNode(const NodePtr& node, int nodeID = 0);

	private: // ------------------------- ノード削除用 -------------------------

		/// <summary>
		/// ノードを削除する
		/// </summary>
		/// <param name="[in] node"> 削除するノード </param>
		void DestroyNode(const NodePtr& node);

	private: // ------------------------- ノード制御用 -------------------------

		/// <summary>
		/// エディタ内のノードの状態を更新する
		/// </summary>
		void UpdateNode();

	private: // ----------------------- ピン同士のリンク -----------------------

		/// <summary>
		/// ピン同士のリンク作成状態を更新する
		/// </summary>
		void UpdateCreatedLink();

		/// <summary>
		/// ピン同士のリンク状態を表示する
		/// </summary>
		void RenderLink();

		/// <summary>
		/// リンクする組を追加する
		/// </summary>
		/// <param name="[in] linkPair"> リンクの組 </param>
		void AddLinkPair(const LinkPair& linkPair);

		/// <summary>
		/// ピン同士をリンクする
		/// </summary>
		/// <param name="[in] linkID"> リンク識別番号 </param>
		/// <param name="[in] linkPair"> リンクする組 </param>
		void LinkPin(int linkID, const LinkPair& linkPair);

		/// <summary>
		/// ピン同士のリンクを解除する
		/// </summary>
		/// <param name="[in] linkID"> 解除するリンクの識別番号 </param>
		void UnLinkPin(int linkID);

	private: // ---------------------------- 全削除 ----------------------------

		/// <summary>
		/// 情報を全削除する
		/// </summary>
		void Clear();

	private: // ------------------------- ノード管理用 -------------------------

		// ノード管理用配列
		// <識別番号, ノード>
		NodeList nodeList;

		// イベントノード管理用配列
		NodeList eventNodeList;

		// ピン管理用配列
		// <識別番号, ピン>
		PinList pinList;

		// リンク管理用配列
		// <リンク識別番号, <入力用ピンの識別番号, 出力用ピンの識別番号>>
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

		// ノード作成用関数の配列
		static CreateNodeFuncList createNodeFuncList;
	};

} // namespace PokarinEngine

#endif // !POKARINENGINE_NODEEDITOR_H_INCLUDED

