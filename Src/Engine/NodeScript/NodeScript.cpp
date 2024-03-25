/**
* @file NodeScript.cpp
*/
#include "NodeScript.h"

#include "ImGui/imgui.h"
#include "ImGui/imgui_internal.h"
#include "ImGui/imgui_impl_glfw.h"
#include "ImGui/imgui_impl_opengl3.h"
#include "ImGui/imnodes.h"

#include "NodeEditor.h"

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
	/// ノードエディタを管理する機能
	/// </summary>
	namespace NodeScript
	{
		// ---------------------------------
		// 型の別名を定義
		// ---------------------------------

		using OpenEditorList = std::unordered_set<NodeEditorPtr>;
		using ClosedEditorList = std::vector<NodeEditorPtr>;

		// ---------------------------------------
		// 変数
		// ---------------------------------------

		// ImGui用コンテキスト
		ImGuiContext* imGuiContext = nullptr;

		// 開いているノードエディタ配列
		OpenEditorList openEditorList;

		// 閉じているノードエディタ配列
		NodeEditorPtr closedEditor;

		// 選択中のノードエディタ
		NodeEditorPtr selectEditor;

		// ImGuiの色設定の回数
		int pushColorCount = 0;

		// -------------------------------------------
		// 関数
		// -------------------------------------------

		/// ここでしか使わないので、cppのみに書く
		/// <summary>
		/// ImGuiの色設定を開始する
		/// </summary>
		/// <param name="style"> 色を設定したい項目 </param>
		/// <param name="styleColor"> 設定する色 </param>
		void PushStyleColor(ImGuiCol style, const Color& styleColor)
		{
			ImGui::PushStyleColor(style, styleColor);
			pushColorCount++;
		}

		/// ここでしか使わないので、cppのみに書く
		/// <summary>
		/// ImGuiの色設定を終了する
		/// </summary>
		void PopStyleColor()
		{
			ImGui::PopStyleColor(pushColorCount);
			pushColorCount = 0;
		}

		/// ここでしか使わないので、cppのみに書く
		/// <summary>
		/// 閉じているノードエディタを配列から削除する
		/// </summary>
		void EraseClosedEditor()
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
				imGuiContext->NavWindow = &selectEditor->GetImGuiWindow();
			}

			// 削除したノードエディタの所有権を放棄
			closedEditor.reset();
		}

		/// <summary>
		/// 初期化
		/// </summary>
		void Initialize()
		{
			// ---------------------------
			// コンテキスト作成
			// ---------------------------

			// ノードスクリプト用のウィンドウを使用するように設定
			glfwMakeContextCurrent(&Window::GetWindow(WindowID::NodeScript));

			// ImGuiのバージョンを確認
			IMGUI_CHECKVERSION();

			// ImGuiコンテキスト作成
			imGuiContext = ImGui::CreateContext();

			// 作成したコンテキストを使用する用に設定
			ImGui::SetCurrentContext(imGuiContext);

			// ImNodesコンテキストを作成
			ImNodes::CreateContext();

			// ---------------------------------------
			// ドッキングウィンドウの有効化
			// ---------------------------------------

			ImGuiIO& io = ImGui::GetIO();
			io.ConfigFlags |= ImGuiConfigFlags_::ImGuiConfigFlags_DockingEnable;

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

			// 使用するコンテキストを設定
			ImGui::SetCurrentContext(imGuiContext);

			// ImGuiの更新
			ImGui_ImplGlfw_NewFrame();
			ImGui_ImplOpenGL3_NewFrame();
			ImGui::NewFrame();

			// ------------------------------------
			// ImGuiの色設定
			// ------------------------------------

			PushStyleColor(ImGuiCol_::ImGuiCol_Tab, BasicColor::gray);
			PushStyleColor(ImGuiCol_::ImGuiCol_TitleBgActive, BasicColor::gray);

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

			// -------------------------------------
			// ImGuiの色設定を終了する
			// -------------------------------------

			PopStyleColor();

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
			// ImGuiの描画
			ImGui::Render();
			ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
		}

		/// <summary>
		/// 終了
		/// </summary>
		void Finalize()
		{
			if (ImNodes::GetCurrentContext())
			{
				ImNodes::DestroyContext();
			}

			if (imGuiContext)
			{
				ImGui::SetCurrentContext(imGuiContext);

				ImGui_ImplGlfw_Shutdown();
				ImGui_ImplOpenGL3_Shutdown();
				ImGui::DestroyContext(imGuiContext);
			}
		}

		/// <summary>
		/// ノードエディタを開く
		/// </summary>
		/// <param name="nodeEditor"> ノードエディタ </param>
		void OpenNodeEditor(const NodeEditorPtr nodeEditor)
		{
			// 管理用配列に追加
			// 追加済みならfalseになる
			openEditorList.emplace(nodeEditor);

			// 既に追加されているので、フォーカスする
			imGuiContext->NavWindow = &nodeEditor->GetImGuiWindow();
		}

	} // namespace NodeScript

} // namespace PokarinEngine