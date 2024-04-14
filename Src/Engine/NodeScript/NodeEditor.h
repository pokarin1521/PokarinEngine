/**
* @file NodeEditor.h
*/
#ifndef NODEEDITOR_H_INCLUDED
#define NODEEDITOR_H_INCLUDED

#include "ImGui/imgui.h"
#include "ImGui/imgui_internal.h"

#include "ImGui/imnodes.h"
#include "ImGui/imnodes_internal.h"

#include "Nodes/Node.h"

#include "../UsingNames/UsingNode.h"
#include "../UsingNames/UsingNodeEditor.h"

#include <vector>
#include <unordered_set>
#include <memory>

namespace PokarinEngine
{
	// -----------------------
	// 前方宣言
	// -----------------------

	class GameObject;

	/// <summary>
	/// ノードでオブジェクトに対する処理を編集する画面
	/// </summary>
	class NodeEditor
	{
	public: // -------------------------- 作成 --------------------------

		/// <summary>
		/// ノードエディタを作成する
		/// </summary>
		/// <returns> 作成したノードエディタ </returns>
		static NodeEditorPtr Create(GameObject* ownerObject)
		{
			return std::make_shared<NodeEditor>(ownerObject);
		}

	public: // -------------- コンストラクタ・デストラクタ --------------

		NodeEditor(GameObject* ownerObject);

		~NodeEditor()
		{
			ImNodes::EditorContextFree(nodeEditorContext);
		}

	public: // ------------------------- 制御 --------------------------

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
		/// ノードの入出力用ピンの識別番号を取得する
		/// </summary>
		/// <returns> 重複しない識別番号 </returns>
		int GetSinglePinID();

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

		// <ノードAの識別番号, ノードBの識別番号>
		using LinkPair = std::pair<int, int>;
		using LinkPairList = std::vector<LinkPair>;

		using IDList = std::unordered_set<int>;

	private: // ------------------------- ノード作成用 -------------------------

		/// <summary>
		/// ノード作成用ボタンの処理
		/// </summary>
		/// <typeparam name="T"> ノードクラス </typeparam>
		template <class T>
		void CreateNodeButton()
		{
			// ボタンを押したらノード作成
			if(ImGui::Button(T::name))
			{
				CreateNode(std::make_shared<T>());
			}
		}

		/// <summary>
		/// ノード作成時の処理
		/// </summary>
		/// <param name="node"> 作成したノード </param>
		void CreateNode(NodePtr node);

		/// <summary>
		/// ノード作成用ポップアップの処理
		/// </summary>
		void CreateNodePopup();

	private: // --------------------------- 識別番号 ---------------------------

		/// <summary>
		/// 重複しない識別番号を配列に追加する
		/// </summary>
		/// <param name="idList"> 識別番号を追加する配列 </param>
		/// <returns> 重複しない識別番号 </returns>
		int AddSingleID(IDList& idList);

	private: // ------------------------- ノード管理用 -------------------------

		// ノード管理用配列
		NodeList nodeList;

		// ノードのリンク状態を管理する配列
		LinkPairList linkPairList;

		// ノードの識別番号を管理する配列
		IDList nodeIDList;

		// ノードの入出力用ピンの識別番号を管理する配列
		IDList nodePinIDList;

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

