/**
* @file MainEditor.cpp
*/
#include "MainEditor.h"

#include "ImGui/imgui.h"
#include "ImGui/imgui_impl_glfw.h"
#include "ImGui/imgui_impl_opengl3.h"

#include "Hierarchy.h"
#include "Inspector.h"
#include "SceneView.h"
#include "GameView.h"

#include "../Engine.h"
#include "../GameObject.h"
#include "../Debug.h"

#include "../Settings/MeshSettings.h"

#include "../Window.h"
#include "../InputManager.h"
#include "../Color.h"

#include <fstream>
#include <filesystem>
#include <functional>

namespace PokarinEngine
{
	/// <summary>
	/// 変数
	/// </summary>
	namespace
	{
		// GLSLのバージョン
		const char* glslVersion = "#version 450";

		// シーンビュー
		SceneView sceneView;

		// ゲームビュー
		GameView gameView;

		// ImGui用コンテキスト
		ImGuiContext* imGuiContext = nullptr;

		// ImGuiの色設定の数
		int pushColorCount = 0;
	}

	/// <summary>
	/// ImGuiの色設定用関数
	/// </summary>
	namespace StyleColor
	{
		/// <summary>
		/// ImGuiの色設定を開始する
		/// </summary>
		/// <param name="style"> 色を設定したい項目 </param>
		/// <param name="styleColor"> 設定する色 </param>
		void Push(ImGuiCol style, const Color& styleColor)
		{
			// 色設定を開始
			ImGui::PushStyleColor(style, styleColor);

			// 設定数をカウント
			pushColorCount++;
		}

		/// <summary>
		/// ImGuiの色設定を終了する
		/// </summary>
		void Pop()
		{
			// 色設定を終了
			ImGui::PopStyleColor(pushColorCount);

			// 設定数をリセット
			pushColorCount = 0;
		}
	}

	/// <summary>
	/// メインエディタ管理用
	/// </summary>
	namespace MainEditor
	{
		/// ここでしか使わないので、cppのみに書く
		/// <summary>
		/// メインメニュー
		/// </summary>
		void MainMenu()
		{
			// メインメニュー作成
			ImGui::BeginMainMenuBar();
			{
				if (ImGui::BeginMenu("Menu"))
				{
					ImGui::Text("OpenMenu");
					ImGui::EndMenu();
				}

				ImGui::EndMainMenuBar();
			}
		}

		/// <summary>
		/// 初期化
		/// </summary>
		/// <param name="engine"> エンジンクラスの参照 </param>
		void Initialize(Engine& engine)
		{
			// ---------------------------
			// コンテキスト作成
			// ---------------------------

			glfwMakeContextCurrent(&Window::GetWindow(WindowID::Main));

			// ImGuiのバージョンを確認
			IMGUI_CHECKVERSION();

			// コンテキスト作成
			imGuiContext = ImGui::CreateContext();

			// コンテキストを使用する
			ImGui::SetCurrentContext(imGuiContext);

			// ---------------------------------------
			// ドッキングウィンドウの有効化
			// ---------------------------------------

			ImGuiIO& io = ImGui::GetIO();
			io.ConfigFlags |= ImGuiConfigFlags_::ImGuiConfigFlags_DockingEnable;

			// ------------------------
			// ImGuiの初期化
			// ------------------------

			// GLFW
			ImGui_ImplGlfw_InitForOpenGL(&Window::GetWindow(WindowID::Main), true);

			// GLSLのバージョンを指定
			ImGui_ImplOpenGL3_Init(glslVersion);

			// ----------------------------------
			// フォントを変更する
			// ----------------------------------

			io.Fonts->Clear();
			io.Fonts->AddFontFromFileTTF("Res/Fonts/arial.ttf", 20.0f);

			// ----------------------------
			// 描画用ビューの初期化
			// ----------------------------

			// シーンビュー
			sceneView.Initialize(engine);

			// ゲームビュー
			gameView.Initialize(engine);

			// ----------------------------------------
			// ヒエラルキーウィンドウの初期化
			// ----------------------------------------

			Hierarchy::Initialize(engine);
		}

		/// <summary>
		/// 更新
		/// </summary>
		void Update()
		{
			// -------------------------
			// ImGuiフレームの更新
			// -------------------------

			ImGui::SetCurrentContext(imGuiContext);

			ImGui_ImplGlfw_NewFrame();
			ImGui_ImplOpenGL3_NewFrame();
			ImGui::NewFrame();

			// ---------------------------------
			// タブの丸みを設定
			// ---------------------------------

			// 丸み無し
			ImGui::PushStyleVar(ImGuiStyleVar_::ImGuiStyleVar_TabRounding, 0);

			// -------------------------------------
			// ウィンドウの背景色を設定
			// -------------------------------------

			StyleColor::Push(ImGuiCol_::ImGuiCol_WindowBg, BasicColor::gray);
			StyleColor::Push(ImGuiCol_::ImGuiCol_Tab, BasicColor::gray);
			StyleColor::Push(ImGuiCol_::ImGuiCol_TabUnfocusedActive, BasicColor::gray);
			StyleColor::Push(ImGuiCol_::ImGuiCol_TitleBgActive, BasicColor::black);

			// --------------------------------------------------------------
			// 画面全体でウィンドウをドッキングできるようにする
			// --------------------------------------------------------------

			ImGui::DockSpaceOverViewport(ImGui::GetMainViewport());

			// ------------------------------
			// メインメニュー
			// ------------------------------

			MainMenu();

			// ------------------------------
			// ビューの更新(仮)
			// ------------------------------

			// シーンビュー
			sceneView.Update();

			// ゲームビュー
			gameView.Update();

			// ヒエラルキーウィンドウ
			Hierarchy::Update();

			// インスペクターウィンドウ
			Inspector::Update(Hierarchy::GetSelectObject());

			// --------------------------
			// デモ(機能確認用)
			// --------------------------

			ImGui::ShowDemoWindow();

			// ------------------------------------
			// タブの丸み設定を終了
			// ------------------------------------

			ImGui::PopStyleVar();

			// ------------------------------------
			// 色関係の設定を終了
			// ------------------------------------

			StyleColor::Pop();
		}

		/// <summary>
		/// 描画
		/// </summary>
		void Render()
		{
			// エディタ画面を描画
			ImGui::Render();
			ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
		}

		/// <summary>
		/// 終了
		/// </summary>
		void Finalize()
		{
			// -----------------
			// ImGuiの終了
			// -----------------

			// コンテキストを設定
			ImGui::SetCurrentContext(imGuiContext);

			ImGui_ImplGlfw_Shutdown();
			ImGui_ImplOpenGL3_Shutdown();

			// ------------------------
			// コンテキストの削除
			// ------------------------

			ImGui::DestroyContext(imGuiContext);
		}

		/// <summary>
		/// シーンビューを取得する
		/// </summary>
		/// <returns> シーンビュー </returns>
		const SceneView& GetSceneView()
		{
			return sceneView;
		}

		/// <summary>
		/// ゲームビューを取得する
		/// </summary>
		/// <returns> ゲームビュー </returns>
		const GameView& GetGameView()
		{
			return gameView;
		}

	} // namespace MainEditor

} // namespace PokarinEngine
