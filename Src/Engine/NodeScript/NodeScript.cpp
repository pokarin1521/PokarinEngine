/**
* @file NodeScript.cpp
*/
#include "NodeScript.h"

#include "ImGui/imgui.h"
#include "ImGui/imgui_internal.h"
#include "ImGui/imgui_impl_glfw.h"
#include "ImGui/imgui_impl_opengl3.h"

#include "ImGui/imnodes.h"
#include "ImGui/imnodes_internal.h"

#include "NodeEditor.h"

#include "../Configs/ImGuiConfig.h"
#include "../ImGuiFontSetter.h"

#include "../Window.h"
#include "../InputManager.h"
#include "../Color.h"

#include <GLFW/glfw3.h>
#include <unordered_map>
#include <unordered_set>
#include <vector>

namespace PokarinEngine
{
	/// <summary>
	/// コンテキスト管理用クラス
	/// </summary>
	class ContextManager
	{
	public: // ------------------------ コンテキスト制御 -------------------------

		/// <summary>
		/// コンテキストを作成する
		/// </summary>
		static void CreateContext()
		{
			imGuiContext = ImGui::CreateContext();
			imNodesContext = ImNodes::CreateContext();
		}

		/// <summary>
		/// コンテキストを削除する
		/// </summary>
		static void DestroyContext()
		{
			// ImNodesコンテキストを削除
			if (imNodesContext)
			{
				ImNodes::SetCurrentContext(imNodesContext);
				ImNodes::DestroyContext(imNodesContext);
			}

			// ImGuiコンテキストを削除
			if (imGuiContext)
			{
				ImGui::SetCurrentContext(imGuiContext);

				ImGui_ImplGlfw_Shutdown();
				ImGui_ImplOpenGL3_Shutdown();
				ImGui::DestroyContext(imGuiContext);
			}
		}

		/// <summary>
		/// コンテキストを使用する
		/// </summary>
		static void UsingContext()
		{
			ImGui::SetCurrentContext(imGuiContext);
			ImNodes::SetCurrentContext(imNodesContext);
		}

	public: // ---------------------------- フォーカス ---------------------------

		/// <summary>
		/// ノードエディタをフォーカスする
		/// </summary>
		/// <param name="[in] nodeEditor"> フォーカスするノードエディタ </param>
		static void FocusEditor(const NodeEditorPtr& nodeEditor)
		{
			imGuiContext->NavWindow = &nodeEditor->GetImGuiWindow();
		}

	private:

		// ImGui用コンテキスト
		inline static ImGuiContext* imGuiContext = nullptr;

		// ImNodes用コンテキスト
		inline static ImNodesContext* imNodesContext = nullptr;
	};

	/// <summary>
	/// ノードエディタ管理用クラス
	/// </summary>
	class NodeEditorManager
	{
	public: // -------------------- ノードエディタの更新 ----------------------

		/// <summary>
		/// ノードエディタを更新する
		/// </summary>
		/// <param name="[in] dockSpaceID"> ドックスペースの識別番号 </param>
		static void UpdateEditor(const ImGuiID& dockSpaceID)
		{
			// ノードスクリプトウィンドウが閉じられたら
			// 全てのノードエディタウィンドウを閉じる
			if (glfwWindowShouldClose(&Window::GetWindow(WindowID::NodeScript)))
			{
				openEditorList.clear();
				return;
			}

			// ------------------------------------
			// ノードエディタを更新
			// ------------------------------------

			// 開いているノードエディタ
			for (auto& nodeEditor : openEditorList)
			{
				// ImGuiウィンドウが最初からドッキングされるように設定
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

	public: // -------------------- ノードエディタ管理用 ----------------------

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
		static void EraseClosedEditor()
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
				Window::CloseWindow(WindowID::NodeScript);
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
				ContextManager::FocusEditor(selectEditor);
			}

			// 削除したノードエディタの所有権を放棄
			closedEditor.reset();
		}

		/// <summary>
		/// ノードエディタを削除する
		/// </summary>
		/// <param name="[in] nodeEditor"> 削除するノードエディタ </param>
		static void EraseEditor(const NodeEditorPtr& nodeEditor)
		{
			openEditorList.erase(nodeEditor);
		}

	private: // -------------------- 型の別名を定義 -----------------------

		using OpenEditorList = std::unordered_set<NodeEditorPtr>;
		using ClosedEditorList = std::vector<NodeEditorPtr>;

	private: // -------------------- ノードエディタ -----------------------

		// 開いているノードエディタ配列
		inline static OpenEditorList openEditorList;

		// 閉じているノードエディタ配列
		inline static NodeEditorPtr closedEditor;

		// 選択中のノードエディタ
		inline static NodeEditorPtr selectEditor;
	};

	/// <summary>
	/// ImGuiの色設定用クラス
	/// </summary>
	class SetUpColor
	{
	public: // ----------------------- 色設定 --------------------------

		/// <summary>
		/// ImGuiの色設定を開始する
		/// </summary>
		/// <param name="[in] style"> 色を設定したい項目 </param>
		/// <param name="[in] styleColor"> 設定する色 </param>
		static void Push(ImGuiCol style, const Color& styleColor)
		{
			// ImVec4型に変換
			const ImVec4 color = { styleColor.r,styleColor.g,styleColor.b,styleColor.a };

			// 色を設定
			ImGui::PushStyleColor(style, color);

			// 設定数をカウント
			pushColorCount++;
		}

		/// <summary>
		/// ImGuiの色設定を終了する
		/// </summary>
		static void Pop()
		{
			ImGui::PopStyleColor(pushColorCount);
			pushColorCount = 0;
		}

	private: // -------------------------- 情報 --------------------------

		// ImGuiの色設定の回数
		inline static int pushColorCount = 0;
	};

	/// <summary>
	/// ノードエディタを管理する機能
	/// </summary>
	namespace NodeScript
	{
		/// <summary>
		/// 初期化
		/// </summary>
		void Initialize()
		{
			// ---------------------------
			// コンテキスト作成
			// ---------------------------

			// ノードスクリプト用のウィンドウを使用する
			Window::SetCurrentWindow(WindowID::NodeScript);

			// ImGuiのバージョンを確認
			IMGUI_CHECKVERSION();

			// コンテキスト作成
			ContextManager::CreateContext();

			// コンテキストを使用する
			ContextManager::UsingContext();

			// ----------------------------------------
			// ImGuiの保存先ファイルを設定
			// ----------------------------------------

			// ImGuiの設定用
			ImGuiIO& io = ImGui::GetIO();

			// 保存先を設定
			io.IniFilename = ImGuiConfig::File::setting;

			// ---------------------------------------
			// ドッキングウィンドウの有効化
			// ---------------------------------------

			io.ConfigFlags |= ImGuiConfigFlags_DockingEnable;

			// -------------------------------------------------------
			// ImGuiウィンドウの情報を保存しないように設定
			// -------------------------------------------------------

			// 更新時に設定しているので、保存しておく必要がない
			io.IniFilename = nullptr;

			// ------------------------
			// フォントを設定
			// ------------------------

			ImGuiFontSetter::SetFont(io);

			// ------------------------
			// ImGuiの初期化
			// ------------------------

			// GLFW
			ImGui_ImplGlfw_InitForOpenGL(&Window::GetWindow(WindowID::NodeScript), true);

			// GLSLのバージョンを指定
			ImGui_ImplOpenGL3_Init("#version 450");
		}

		/// <summary>
		/// 更新
		/// </summary>
		void Update()
		{
			// ----------------------------------------
			// ImGuiの更新
			// ----------------------------------------

			// コンテキストの使用
			ContextManager::UsingContext();

			// ImGuiの更新
			ImGui_ImplGlfw_NewFrame();
			ImGui_ImplOpenGL3_NewFrame();
			ImGui::NewFrame();

			// ------------------------------------
			// ImGuiの丸み設定
			// ------------------------------------

			// タブの丸みを無くす
			ImGui::PushStyleVar(ImGuiStyleVar_::ImGuiStyleVar_TabRounding, 0);

			// ------------------------------------
			// ImGuiの色設定
			// ------------------------------------

			SetUpColor::Push(ImGuiCol_::ImGuiCol_Tab, Color::gray);
			SetUpColor::Push(ImGuiCol_::ImGuiCol_TitleBgActive, Color::gray);

			// -----------------------------------------------------------------------
			// ウィンドウ全体でウィンドウをドッキング出来るようにする
			// -----------------------------------------------------------------------

			// ドッキングの機能
			ImGuiDockNodeFlags dockFlags = ImGuiDockNodeFlags_::ImGuiDockNodeFlags_NoSplit |
				ImGuiDockNodeFlags_::ImGuiDockNodeFlags_NoUndocking |
				ImGuiDockNodeFlagsPrivate_::ImGuiDockNodeFlags_NoWindowMenuButton |
				ImGuiDockNodeFlagsPrivate_::ImGuiDockNodeFlags_NoCloseButton;

			// ドッキングスペースの識別番号
			ImGuiID dockSpaceID = ImGui::DockSpaceOverViewport(ImGui::GetMainViewport(), dockFlags);

			/* ここ以下で作成されるImGuiウィンドウがドッキング可能になる */

			// --------------------------------------
			// ノードエディタを更新
			// --------------------------------------

			NodeEditorManager::UpdateEditor(dockSpaceID);

			// --------------------------------------
			// ImGuiの丸み設定を終了する
			// --------------------------------------

			ImGui::PopStyleVar();

			// -------------------------------------
			// ImGuiの色設定を終了する
			// -------------------------------------

			SetUpColor::Pop();

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
		void Render()
		{
			// コンテキストを設定
			ContextManager::UsingContext();

			// ImGuiの描画
			ImGui::Render();
			ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
		}

		/// <summary>
		/// 終了
		/// </summary>
		void Finalize()
		{
			ContextManager::DestroyContext();
		}

		/// <summary>
		/// ノードエディタを開く
		/// </summary>
		/// <param name="[in] nodeEditor"> ノードエディタ </param>
		void OpenNodeEditor(const NodeEditorPtr& nodeEditor)
		{
			// ノードエディタを開く
			nodeEditor->OpenEditor();

			// 管理用配列に追加
			NodeEditorManager::AddEditor(nodeEditor);

			// 開いたノードエディタをフォーカスする
			ContextManager::FocusEditor(nodeEditor);
		}

		/// <summary>
		/// ノードエディタを閉じる
		/// </summary>
		/// <param name="[in] nodeEditor"> ノードエディタ </param>
		void CloseNodeEditor(const NodeEditorPtr& nodeEditor)
		{
			NodeEditorManager::EraseEditor(nodeEditor);
		}

	} // namespace NodeScript

} // namespace PokarinEngine