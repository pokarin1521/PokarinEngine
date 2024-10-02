/**
* @file NodeEditorManager.h
*/
#ifndef POKARINENGINE_NODEEDITORMANAGER_H_INCLUDED
#define POKARINENGINE_NODEEDITORMANAGER_H_INCLUDED

#include "../UsingNames/UsingNodeEditor.h"

#include "ImGui/imgui.h"

#include <memory>
#include <unordered_set>

// --------------------
// 前方宣言
// --------------------

struct ImGuiContext;
struct ImNodesContext;

namespace PokarinEngine
{
	/// <summary>
	/// ノードエディタ管理用クラス
	/// </summary>
	class NodeEditorManager
	{
	public: // -------------- コンストラクタ・デストラクタ --------------

		NodeEditorManager() = default;

		/// <summary>
		/// デストラクタ
		/// </summary>
		~NodeEditorManager();

	public: // ------------------------ 禁止事項 ------------------------

		/* コンテキストの管理がおかしくなるので禁止する */

		// コピーコンストラクタの禁止
		NodeEditorManager(const NodeEditorManager&) = delete;

		// 代入の禁止
		NodeEditorManager& operator=(const NodeEditorManager&) = delete;

	public: // -------------------------- 制御 --------------------------

		/* デストラクタで終了処理を行えるように、
		インスタンス化して制御させるようにする */

		/// <summary>
		/// 初期化
		/// </summary>
		void Initialize();

		/// <summary>
		/// 更新
		/// </summary>
		void Update();

		/// <summary>
		/// 描画
		/// </summary>
		void Render();

	public: // -------------------- 閉じたときの処理 --------------------

		/// <summary>
		/// ウィンドウを閉じたときの処理
		/// </summary>
		static void ClosedWindow();

	public: // ------------------ ノードエディタの開閉 ------------------

		/// <summary>
		/// ノードエディタを開く
		/// </summary>
		/// <param name="[in] nodeEditor"> ノードエディタ </param>
		static void OpenNodeEditor(const NodeEditorPtr& nodeEditor);

		/// <summary>
		/// ノードエディタを閉じる
		/// </summary>
		/// <param name="[in] nodeEditor"> ノードエディタ </param>
		static void CloseNodeEditor(const NodeEditorPtr& nodeEditor);

	private: // ----------------- ノードエディタの更新 ------------------

		/// <summary>
		/// ノードエディタを更新する
		/// </summary>
		static void UpdateNodeEditor();

	private: // ------------------ コンテキストの制御 -------------------

		/// <summary>
		/// コンテキストの使用を開始する
		/// </summary>
		static void UseContext();

	private: // ---------------------- フォーカス -----------------------

		/// <summary>
		/// ノードエディタをフォーカスする
		/// </summary>
		/// <param name="[in] nodeEditor"> フォーカスするノードエディタ </param>
		static void FocusEditor(const NodeEditorPtr& nodeEditor);

		/// <summary>
		/// ノードエディタを追加する
		/// </summary>
		/// <param name="[in] nodeEditor"> 追加するノードエディタ </param>
		static void AddEditor(const NodeEditorPtr& nodeEditor)
		{
			openEditorList.emplace(nodeEditor);
		}

		/// <summary>
		/// 閉じているノードエディタを配列から削除する
		/// </summary>
		static void EraseClosedEditor();

	private: // --------------------- コンテキスト ----------------------

		// ImGui用コンテキスト
		inline static ImGuiContext* imGuiContext = nullptr;

		// ImNodes用コンテキスト
		inline static ImNodesContext* imNodesContext = nullptr;

	private: // -------------------- ノードエディタ ---------------------

		using NodeEditorList = std::unordered_set<NodeEditorPtr>;

		// 開いているノードエディタ管理用配列
		inline static NodeEditorList openEditorList;

		// 閉じているノードエディタ
		inline static NodeEditorPtr closedEditor;

		// 選択中のノードエディタ
		inline static NodeEditorPtr selectEditor;

	private: // --------------------- ドッキング用 ----------------------

		// ドッキングスペースの識別番号
		inline static int dockSpaceID = 0;
	};

} // namespace PokarinEngine

#endif // !POKARINENGINE_NODEEDITORMANAGER_H_INCLUDED
