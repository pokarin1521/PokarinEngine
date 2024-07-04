/**
* @file MainEditor.cpp
*/
#include "MainEditor.h"

#include "ImGui/imgui.h"
#include "ImGui/imgui_internal.h"
#include "ImGui/imgui_impl_glfw.h"
#include "ImGui/imgui_impl_opengl3.h"

#include "Toolbar.h"

#include "../Engine.h"
#include "../GameObject.h"
#include "../Debug.h"

#include "../Configs/ImGuiConfig.h"
#include "../Configs/MeshConfig.h"

#include "../ImGuiFontSetter.h"

#include "../Window.h"
#include "../InputManager.h"
#include "../Color.h"

#include <fstream>
#include <filesystem>
#include <functional>

namespace PokarinEngine
{
	/// <summary>
	/// メインメニュー
	/// </summary>
	void MainEditor::MainMenu()
	{
		// メインメニュー作成
		ImGui::BeginMainMenuBar();
		{
			// ファイル関係のメニュー
			if (ImGui::BeginMenu("File"))
			{
				if (ImGui::Button("Save"))
				{
					engine->GetCurrentScene().SaveScene();
				}

				ImGui::EndMenu();
			}
			
			// ファイル関係のメニュー
			if (ImGui::BeginMenu("File"))
			{
				if (ImGui::Button("Load"))
				{
					engine->GetCurrentScene().SaveScene();
				}

				ImGui::EndMenu();
			}

			ImGui::EndMainMenuBar();
		}
	}

#pragma region MainEditor

	/// <summary>
	/// 初期化
	/// </summary>
	/// <param name="[in] e"> エンジンクラスの参照 </param>
	void MainEditor::Initialize(Engine& e)
	{
		// ----------------------------
		// エンジンを登録
		// ----------------------------

		engine = &e;

		// ---------------------------
		// コンテキスト作成
		// ---------------------------

		// メインウィンドウを使用する
		Window::SetCurrentWindow(WindowID::Main);

		// ImGuiのバージョンを確認
		IMGUI_CHECKVERSION();

		// コンテキスト作成
		imGuiContext = ImGui::CreateContext();

		// コンテキストを使用する
		ImGui::SetCurrentContext(imGuiContext);

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

		io.ConfigFlags |= ImGuiConfigFlags_::ImGuiConfigFlags_DockingEnable;

		// ------------------------
		// ImGuiの初期化
		// ------------------------

		// GLFW
		ImGui_ImplGlfw_InitForOpenGL(&Window::GetWindow(WindowID::Main), true);

		// GLSLのバージョンを指定
		ImGui_ImplOpenGL3_Init(glslVersion);

		// ----------------------------------
		// フォントを設定
		// ----------------------------------

		ImGuiFontSetter::SetFont(io);

		// ----------------------------
		// 描画用ビューの初期化
		// ----------------------------

		// シーンビュー
		sceneView.Initialize(*engine);

		// ゲームビュー
		gameView.Initialize(*engine);

		// ----------------------------------------
		// ヒエラルキーウィンドウの初期化
		// ----------------------------------------

		hierarchy.Initialize(*engine);

		// ----------------------------------------
		// ツールバーの初期化
		// ----------------------------------------

		Toolbar::Initialize(*engine);
	}

	/// <summary>
	/// 更新
	/// </summary>
	/// <param name="[out] isPlayGame"> ゲーム再生中ならtrue </param>
	void MainEditor::Update(bool& isPlayGame)
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

		PushColor(ImGuiCol_::ImGuiCol_WindowBg, Color::gray);
		PushColor(ImGuiCol_::ImGuiCol_Tab, Color::gray);
		PushColor(ImGuiCol_::ImGuiCol_TabUnfocusedActive, Color::gray);
		PushColor(ImGuiCol_::ImGuiCol_TitleBgActive, Color::black);

		// --------------------------------------------------------------
		// 画面全体でウィンドウをドッキングできるようにする
		// --------------------------------------------------------------

		ImGui::DockSpaceOverViewport(ImGui::GetMainViewport());

		// ------------------------------
		// メインメニュー
		// ------------------------------

		MainMenu();

		// -------------------------------------------
		// エディタ内ウィンドウ・ビューの更新
		// -------------------------------------------

		// シーンビュー
		sceneView.Update();

		// ゲームビュー
		gameView.Update();

		// ヒエラルキーウィンドウ
		hierarchy.Update();

		// インスペクターウィンドウ
		inspector.Update(hierarchy.GetSelectObject());

		// ツールバー
		Toolbar::Update(isPlayGame);

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

		PopColor();
	}

	/// <summary>
	/// 描画
	/// </summary>
	void MainEditor::Render()
	{
		// エディタ画面を描画
		ImGui::Render();
		ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
	}

	/// <summary>
	/// 終了
	/// </summary>
	void MainEditor::Finalize()
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

#pragma endregion

#pragma region ImGuiColor

	/// <summary>
	/// ImGuiの色設定を開始する
	/// </summary>
	/// <param name="[in] style"> 色を設定したい項目 </param>
	/// <param name="[in] styleColor"> 設定する色 </param>
	void MainEditor::PushColor(ImGuiCol style, const Color& styleColor)
	{
		ImVec4 color = { styleColor.r,styleColor.g,styleColor.b,styleColor.a };

		// 色設定を開始
		ImGui::PushStyleColor(style, color);

		// 設定数をカウント
		pushColorCount++;
	}

	/// <summary>
	/// ImGuiの色設定を終了する
	/// </summary>
	void MainEditor::PopColor()
	{
		// 色設定を終了
		ImGui::PopStyleColor(pushColorCount);

		// 設定数をリセット
		pushColorCount = 0;
	}

#pragma endregion

} // namespace PokarinEngine
