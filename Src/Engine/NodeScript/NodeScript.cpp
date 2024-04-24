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
	/// クラス
	/// </summary>
	namespace
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
			void CreateContext()
			{
				imGuiContext = ImGui::CreateContext();
				imNodesContext = ImNodes::CreateContext();
			}

			/// <summary>
			/// コンテキストを削除する
			/// </summary>
			void DestroyContext()
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
			void UsingContext() const
			{
				ImGui::SetCurrentContext(imGuiContext);
				ImNodes::SetCurrentContext(imNodesContext);
			}

		public: // ---------------------------- フォーカス ---------------------------

			/// <summary>
			/// ノードエディタをフォーカスする
			/// </summary>
			/// <param name="nodeEditor"> フォーカスするノードエディタ </param>
			void FocusEditor(NodeEditorPtr nodeEditor) const
			{
				imGuiContext->NavWindow = &nodeEditor->GetImGuiWindow();
			}

		private:

			// ImGui用コンテキスト
			ImGuiContext* imGuiContext = nullptr;

			// ImNodes用コンテキスト
			ImNodesContext* imNodesContext = nullptr;
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
			void UpdateEditor(const ImGuiID& dockSpaceID, const ContextManager& contextManager)
			{
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
					EraseClosedEditor(contextManager);
				}
			}

		public: // -------------------- ノードエディタ管理用 ----------------------

			/// <summary>
			/// ノードエディタを追加する
			/// </summary>
			/// <param name="nodeEditor"> 追加するノードエディタ </param>
			void AddEditor(NodeEditorPtr nodeEditor)
			{
				openEditorList.emplace(nodeEditor);
			}

			/// <summary>
			/// 閉じているノードエディタを配列から削除する
			/// </summary>
			void EraseClosedEditor(const ContextManager& contextManager)
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
					contextManager.FocusEditor(selectEditor);
				}

				// 削除したノードエディタの所有権を放棄
				closedEditor.reset();
			}

		private: // -------------------- 型の別名を定義 -----------------------

			using OpenEditorList = std::unordered_set<NodeEditorPtr>;
			using ClosedEditorList = std::vector<NodeEditorPtr>;

		private: // -------------------- ノードエディタ -----------------------

			// 開いているノードエディタ配列
			OpenEditorList openEditorList;

			// 閉じているノードエディタ配列
			NodeEditorPtr closedEditor;

			// 選択中のノードエディタ
			NodeEditorPtr selectEditor;
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
			/// <param name="style"> 色を設定したい項目 </param>
			/// <param name="styleColor"> 設定する色 </param>
			void Push(ImGuiCol style, const Color& styleColor)
			{
				ImGui::PushStyleColor(style, styleColor);
				pushColorCount++;
			}

			/// <summary>
			/// ImGuiの色設定を終了する
			/// </summary>
			void Pop()
			{
				ImGui::PopStyleColor(pushColorCount);
				pushColorCount = 0;
			}

		private: // -------------------------- 情報 --------------------------

			// ImGuiの色設定の回数
			int pushColorCount = 0;
		};
	}

	/// <summary>
	/// 変数
	/// </summary>
	namespace
	{
		// コンテキスト管理用
		ContextManager contextManager;

		// ノードエディタ管理用
		NodeEditorManager nodeEditorManager;

		// ImGuiの色設定用
		SetUpColor setUpColor;
	}

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

			// ノードスクリプト用のウィンドウを使用するように設定
			glfwMakeContextCurrent(&Window::GetWindow(WindowID::NodeScript));

			// ImGuiのバージョンを確認
			IMGUI_CHECKVERSION();

			// コンテキスト作成
			contextManager.CreateContext();

			// コンテキストを使用する
			contextManager.UsingContext();

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

			// コンテキストの使用
			contextManager.UsingContext();

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

			setUpColor.Push(ImGuiCol_::ImGuiCol_Tab, BasicColor::gray);
			setUpColor.Push(ImGuiCol_::ImGuiCol_TitleBgActive, BasicColor::gray);

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

			nodeEditorManager.UpdateEditor(dockSpaceID, contextManager);

			// --------------------------------------
			// ImGuiの丸み設定を終了する
			// --------------------------------------

			ImGui::PopStyleVar();

			// -------------------------------------
			// ImGuiの色設定を終了する
			// -------------------------------------

			setUpColor.Pop();

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
			contextManager.UsingContext();

			// ImGuiの描画
			ImGui::Render();
			ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
		}

		/// <summary>
		/// 終了
		/// </summary>
		void Finalize()
		{
			contextManager.DestroyContext();
		}

		/// <summary>
		/// ノードエディタを開く
		/// </summary>
		/// <param name="nodeEditor"> ノードエディタ </param>
		void OpenNodeEditor(const NodeEditorPtr nodeEditor)
		{
			// ノードエディタを開く
			nodeEditor->OpenEditor();

			// 管理用配列に追加
			nodeEditorManager.AddEditor(nodeEditor);

			// 開いたノードエディタをフォーカスする
			contextManager.FocusEditor(nodeEditor);
		}

	} // namespace NodeScript

} // namespace PokarinEngine