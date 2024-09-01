/**
* @file MainEditor.cpp
*/
#include "MainEditor.h"

#include "ImGui/imgui.h"
#include "ImGui/imgui_internal.h"
#include "ImGui/imgui_impl_glfw.h"
#include "ImGui/imgui_impl_opengl3.h"

#include "Toolbar.h"

#include "../Scene.h"
#include "../GameObject.h"
#include "../Debug.h"

#include "../ImGuiHelper.h"

#include "../Window.h"
#include "../Input.h"
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
				if (ImGui::MenuItem("Save"))
				{
					currentScene->SaveScene();
				}

				ImGui::EndMenu();
			}
			
			// ファイル関係のメニュー
			if (ImGui::BeginMenu("File"))
			{
				if (ImGui::MenuItem("Load"))
				{
					currentScene->LoadScene();
				}

				ImGui::EndMenu();
			}

			ImGui::EndMainMenuBar();
		}
	}

#pragma region MainEditor

	/// <summary>
	/// デストラクタ
	/// </summary>
	MainEditor::~MainEditor()
	{
		// ImGuiの終了
		ImGuiHelper::Finalize(imGuiContext);
	}

	/// <summary>
	/// 初期化
	/// </summary>
	void MainEditor::Initialize()
	{
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

		// 保存先のファイル名
		static const char* settingFile = "My project/Settings/imgui.ini";

		// 保存先を設定
		io.IniFilename = settingFile;

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

		ImGuiHelper::SetFont(io);

		// ----------------------------------
		// ビューの初期化
		// ----------------------------------

		// シーンビューの初期化
		sceneView.Initialize();

		// ゲームビューの初期化
		gameView.Initialize();

		// ----------------------------------------
		// ツールバーの初期化
		// ----------------------------------------

		Toolbar::Initialize();
	}

	/// <summary>
	/// 更新
	/// </summary>
	/// <param name="[in] _currentScene"> 現在のシーン </param>
	/// <param name="[out] isPlayGame"> ゲーム再生中ならtrue </param>
	void MainEditor::Update(const ScenePtr& _currentScene, bool& isPlayGame)
	{
		// ------------------------------------
		// メインエディタの情報を更新する
		// ------------------------------------

		// 現在のシーンを設定する
		currentScene = _currentScene;

		// ヒエラルキーで選択中のゲームオブジェクトを設定する
		selectObject = hierarchy.GetSelectObject();

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
		hierarchy.Update(currentScene);

		// インスペクターウィンドウ
		inspector.Update(selectObject);

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
		// シーンビューの描画
		sceneView.Render(currentScene, selectObject);

		// ゲームビューの描画
		gameView.Render(currentScene);

		// ImGuiの描画
		ImGui::Render();
		ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
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
