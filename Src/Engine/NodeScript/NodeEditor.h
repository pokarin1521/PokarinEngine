/**
* @file NodeEditor.h
*/
#ifndef NODEEDITOR_H_INCLUDED
#define NODEEDITOR_H_INCLUDED

#include "ImGui/imgui_internal.h"

#include "ImGui/imnodes.h"
#include "ImGui/imnodes_internal.h"

#include "Nodes/Node.h"

#include "../UsingNames/UsingNodeEditor.h"

#include <vector>
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

	public: // -------------------- 型の別名を定義 ---------------------

		// <ノードAの識別番号, ノードBの識別番号>
		using LinkPair = std::pair<int, int>;
		using LinkPairList = std::vector<LinkPair>;

	public: // ------------------------- 制御 --------------------------

		/// <summary>
		/// 更新
		/// </summary>
		/// <returns>
		/// <para> true : ノードエディタのウィンドウが選択されている </para>
		/// <para> false : ノードエディタのウィンドウが選択されていない </para>
		/// </returns>
		bool Update();

	public: // --------------------- ノードの追加 ----------------------

		void AddNode(NodePtr node)
		{
			nodeList.emplace(node);
		}

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

	private: // ------------------------- 情報 -------------------------

		// ノード管理用配列
		NodeList nodeList;

		// ノードのリンク状態を管理する配列
		LinkPairList linkPairList;

		// ノードエディタの持ち主
		GameObject* owner = nullptr;

		// ImGuiウィンドウ
		ImGuiWindow* imGuiWindow = nullptr;

		// ノードエディタの名前
		// ImGuiウィンドウ作成時のタイトルにする
		std::string name = "";

		// ノードエディタが開いているならtrue
		bool isOpen = false;

		// ノードエディタ用コンテキスト
		ImNodesEditorContext* nodeEditorContext = nullptr;
	};

} // namespace PokarinEngine

#endif // !NODEEDITOR_H_INCLUDED

