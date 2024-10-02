/**
* @file NodeEditorManager.cpp
*/
#include "NodeEditorManager.h"

#include "ImGui/imgui_internal.h"

#include "ImGui/imnodes.h"
#include "ImGui/imnodes_internal.h"

#include "NodeEditor.h"

#include "../ImGuiHelper.h"
#include "../Window.h"
#include "../Input.h"
#include "../Color.h"

#include <GLFW/glfw3.h>
#include <unordered_map>
#include <unordered_set>
#include <vector>

namespace PokarinEngine
{
	/// <summary>
	/// デストラクタ
	/// </summary>
	NodeEditorManager::~NodeEditorManager()
	{
		// ImNodesコンテキストを削除する
		ImNodes::DestroyContext(imNodesContext);

		// ImGuiを終了してコンテキストを削除する
		ImGuiHelper::Finalize(imGuiContext);
	}

	/// <summary>
	/// 初期化
	/// </summary>
	void NodeEditorManager::Initialize()
	{
		// ---------------------------
		// コンテキスト作成
		// ---------------------------

		// ノードエディタ用のウィンドウを使用する
		Window::SetCurrentWindow(WindowID::NodeEditor);

		// コンパイル時と実行時でImGuiのバージョンが同じか確認する
		// バージョンが違った場合はプログラムが停止する
		IMGUI_CHECKVERSION();

		// ImGuiコンテキストを作成
		imGuiContext = ImGui::CreateContext();

		// ImNodesコンテキストを作成
		imNodesContext = ImNodes::CreateContext();

		// コンテキストを使用を開始する
		UseContext();

		// ---------------------------------------
		// ドッキングウィンドウの有効化
		// ---------------------------------------

		// ImGuiの設定用
		ImGuiIO& io = ImGui::GetIO();

		// ドッキングウィンドウの有効化
		io.ConfigFlags |= ImGuiConfigFlags_DockingEnable;

		// -------------------------------------------------------
		// ImGuiウィンドウの情報を保存しないように設定
		// -------------------------------------------------------

		// 自動保存されると余計な保存データが増えていくので、
		// 自動保存されないように保存先ファイルを無効にする
		io.IniFilename = nullptr;

		// ------------------------
		// フォントを設定
		// ------------------------

		ImGuiHelper::SetFont(io);

		// ------------------------
		// ImGuiの初期化
		// ------------------------

		// ノードエディタ用ウィンドウを指定
		ImGuiHelper::Initialize(WindowID::NodeEditor);
	}

	/// <summary>
	/// 更新
	/// </summary>
	void NodeEditorManager::Update()
	{
		// ----------------------------------------
		// ImGuiの更新
		// ----------------------------------------

		ImGuiHelper::Update(imGuiContext);

		// ------------------------------------
		// ImGuiの丸み設定
		// ------------------------------------

		// タブの丸みを無くす
		ImGui::PushStyleVar(ImGuiStyleVar_::ImGuiStyleVar_TabRounding, 0);

		// ------------------------------------
		// ImGuiの色設定
		// ------------------------------------

		ImGuiHelper::PushColor(ImGuiCol_::ImGuiCol_Tab, Color::gray);
		ImGuiHelper::PushColor(ImGuiCol_::ImGuiCol_TitleBgActive, Color::gray);

		// -----------------------------------------------------------------------
		// ウィンドウ全体でウィンドウをドッキング出来るようにする
		// -----------------------------------------------------------------------

		// ドッキングの機能
		ImGuiDockNodeFlags dockFlags = ImGuiDockNodeFlags_::ImGuiDockNodeFlags_NoSplit |
			ImGuiDockNodeFlags_::ImGuiDockNodeFlags_NoUndocking |
			ImGuiDockNodeFlagsPrivate_::ImGuiDockNodeFlags_NoWindowMenuButton |
			ImGuiDockNodeFlagsPrivate_::ImGuiDockNodeFlags_NoCloseButton;

		// ドッキングウィンドウを開始する
		// ドッキングスペースの識別番号を設定
		dockSpaceID = ImGui::DockSpaceOverViewport(ImGui::GetMainViewport(), dockFlags);

		/* ここ以下で作成されるImGuiウィンドウがドッキング可能になる */

		// --------------------------------------
		// ノードエディタを更新
		// --------------------------------------

		UpdateNodeEditor();

		// --------------------------------------
		// ImGuiの丸み設定を終了する
		// --------------------------------------

		ImGui::PopStyleVar();

		// -------------------------------------
		// ImGuiの色設定を終了する
		// -------------------------------------

		ImGuiHelper::PopColor();

		// ----------------------------------------
		// カラーバッファをクリアする
		// ----------------------------------------

		// カラーバッファをクリアするときの色を設定
		glClearColor(0, 0, 0, 0);

		// バックバッファをクリア
		// 今回はカラーバッファを指定
		glClear(GL_COLOR_BUFFER_BIT);
	}

	/// <summary>
	/// 描画
	/// </summary>
	void NodeEditorManager::Render()
	{
		// ImGuiの描画
		ImGuiHelper::Render(imGuiContext);
	}

	/// <summary>
	/// ウィンドウを閉じたときの処理
	/// </summary>
	void NodeEditorManager::ClosedWindow()
	{
		// 開いているノードエディタを全て閉じる
		openEditorList.clear();
	}

	/// <summary>
	/// ノードエディタを開く
	/// </summary>
	/// <param name="[in] nodeEditor"> ノードエディタ </param>
	void NodeEditorManager::OpenNodeEditor(const NodeEditorPtr& nodeEditor)
	{
		// ノードエディタを開く
		nodeEditor->OpenEditor();

		// 管理用配列に追加
		AddEditor(nodeEditor);

		// 開いたノードエディタをフォーカスする
		FocusEditor(nodeEditor);
	}

	/// <summary>
	/// ノードエディタを閉じる
	/// </summary>
	/// <param name="[in] nodeEditor"> ノードエディタ </param>
	void NodeEditorManager::CloseNodeEditor(const NodeEditorPtr& nodeEditor)
	{
		// 管理用配列から削除する
		openEditorList.erase(nodeEditor);
	}

	/// <summary>
	/// ノードエディタを更新する
	/// </summary>
	void NodeEditorManager::UpdateNodeEditor()
	{
		// ------------------------------------
		// ノードエディタを更新
		// ------------------------------------

		// 開いているノードエディタ
		for (auto& nodeEditor : openEditorList)
		{
			// ImGuiウィンドウが常にドッキングされるように設定
			ImGui::DockBuilderDockWindow(nodeEditor->GetName(), dockSpaceID);

			// ノードエディタの更新
			if (nodeEditor->Update())
			{
				// 選択中のノードエディタを設定
				selectEditor = nodeEditor;
			}

			// ノードエディタが閉じられたら
			// 後で削除できるように変数に入れる
			if (!nodeEditor->IsOpen())
			{
				closedEditor = nodeEditor;
			}
		}

		// ----------------------------------------------------------
		// 閉じているノードエディタがあるのなら削除する
		// ----------------------------------------------------------

		if (closedEditor)
		{
			EraseClosedEditor();
		}
	}

	/// <summary>
	/// コンテキストの使用を開始する
	/// </summary>
	void NodeEditorManager::UseContext()
	{
		ImGui::SetCurrentContext(imGuiContext);
		ImNodes::SetCurrentContext(imNodesContext);
	}

	/// <summary>
	/// ノードエディタをフォーカスする
	/// </summary>
	/// <param name="[in] nodeEditor"> フォーカスするノードエディタ </param>
	void NodeEditorManager::FocusEditor(const NodeEditorPtr& nodeEditor)
	{
		imGuiContext->NavWindow = &nodeEditor->GetImGuiWindow();
	}

	/// <summary>
	/// 閉じているノードエディタを配列から削除する
	/// </summary>
	void NodeEditorManager::EraseClosedEditor()
	{
		// 閉じているノードエディタを配列から削除
		openEditorList.erase(closedEditor);

		// 開いているノードエディタがなくなった場合
		if (openEditorList.empty())
		{
			// 所有権を放棄
			selectEditor.reset();

			// 表示する意味がないので
			// ウィンドウを閉じる
			Window::CloseWindow(WindowID::NodeEditor);
		}

		// 選択中のノードエディタが閉じられた場合
		if (closedEditor == selectEditor)
		{
			// 先頭のノードエディタを選択中にする
			selectEditor = *openEditorList.begin();
		}

		if (selectEditor)
		{
			// 選択中ノードエディタのウィンドウをフォーカスする
			FocusEditor(selectEditor);
		}

		// 削除したノードエディタの所有権を放棄
		closedEditor.reset();
	}

} // namespace PokarinEngine