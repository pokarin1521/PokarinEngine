/**
* @file Engine.cpp
*/
#include "Engine.h"

#include "Window.h"
#include "Debug.h"
#include "Input.h"
#include "Time.h"
#include "Random.h"
#include "Scene.h"
#include "LightParameter.h"

#include "Math/Matrix.h"

#include "Configs/ShaderConfig.h"

#include "NodeScript/NodeScript.h"

#include "Mesh/Mesh.h"

#include <fstream>
#include <filesystem>
#include <vector>
#include <cmath>
#include <cstdio>

namespace PokarinEngine
{
#pragma region CreateScene

	/// <summary>
	/// シーンを作成する
	/// </summary>
	/// <param name="[in] name"> シーン名 </param>
	/// <returns> 作成したシーンのポインタ </returns>
	ScenePtr Engine::CreateScene(const char* name)
	{
		if (sceneList.size() == INT_MAX)
		{
			LOG_WARNING("シーンの数が最大数になったので、シーンを作成することができません。");
			return nullptr;
		}

		// シーンの識別番号を作成
		int sceneID = CreateSceneID();

		// シーン作成
		auto scene = std::make_shared<Scene>(sceneID, name);

		// エンジンに登録
		sceneList.push_back(scene);

		return scene;
	}

	/// <summary>
	/// シーンの識別番号を作成する
	/// </summary>
	/// <returns> 作成した識別番号 </returns>
	int Engine::CreateSceneID()
	{
		// シーンの識別番号
		// 乱数で決める
		int sceneID = Random::Range(INT_MIN, INT_MAX);

		// シーンの識別番号を管理用配列に追加
		// 重複していたら再設定する
		while (!sceneIDList.emplace(sceneID).second)
		{
			sceneID = Random::Range(INT_MIN, INT_MAX);
		}

		return sceneID;
	}

#pragma endregion

#pragma region Engine

	/// <summary>
	/// ゲームエンジンを実行する
	/// </summary>
	/// <returns>
	/// <para> 0 : 正常に実行が完了した </para>
	/// <para> 0以外 : エラーが発生した </para>
	/// </returns>
	int Engine::Run()
	{
		// ゲームエンジンを初期化する
		// 初期化に失敗したら異常終了
		if (!Initialize())
		{
			return 1;
		}

		// メインウィンドウを開く
		Window::OpenWindow(WindowID::Main, "PokarinEngine");

		// メインループ
		// メインウィンドウが閉じるまで
		while (!Window::IsClosed(WindowID::Main))
		{
			// メインウィンドウ
			if (Window::Begin(WindowID::Main))
			{
				Update(); // 更新
				Render(); // 描画

				Window::End();
			}

			// ノードスクリプトウィンドウ
			if (Window::Begin(WindowID::NodeScript))
			{
				NodeScript::Update();
				NodeScript::Render();

				Window::End();
			}
		}

		// エディタを終了
		mainEditor.Finalize();

		// ノードスクリプトの終了
		NodeScript::Finalize();

		// GLFWの終了
		glfwTerminate();

		return 0;
	}

	/// <summary>
	/// ゲームエンジンを初期化する
	/// </summary>
	/// <returns>
	/// <para> true : 正常に初期化された </para>
	/// <para> false : エラーが発生した </para>
	/// </returns>
	bool Engine::Initialize()
	{
		// ---------------
		// GLFWの初期化
		// ---------------

		if (glfwInit() != GLFW_TRUE)
		{
			return false;	//	初期化失敗
		}

		// -----------------------------------------
		// 全てのウィンドウオブジェクトを作成
		// -----------------------------------------

		// 作成に失敗したら初期化を異常終了
		if (!Window::CreateAllWindow())
		{
			return false;
		}

		// --------------------------------
		// OpenGLコンテキストの作成
		// --------------------------------

		// メインウィンドウを使用する
		Window::SetCurrentWindow(WindowID::Main);

		// OpenGL関数のアドレスを取得
		gladLoadGLLoader(
			reinterpret_cast<GLADloadproc>(glfwGetProcAddress));

		// --------------------------------
		// メッセージコールバックの設定
		// --------------------------------

		glDebugMessageCallback(Debug::Callback, nullptr);

		// -----------------------------
		// エディタを初期化
		// -----------------------------

		// メインエディタ
		mainEditor.Initialize();

		// -----------------------
		// シェーダの初期化
		// -----------------------

		Shader::Initialize();

		// ------------------------------------------
		// メッシュ管理用クラスの初期化
		// ------------------------------------------

		Mesh::Initialize();

		// -----------------------------------
		// シーンがなければ作成
		// -----------------------------------

		if (sceneList.empty())
		{
			currentScene = CreateScene("SampleScene");
		}

		// ------------------------------------
		// ノードスクリプトを初期化
		// ------------------------------------

		NodeScript::Initialize();

		return true;
	}

	/// <summary>
	/// ゲームエンジンの状態を更新する
	/// </summary>
	void Engine::Update()
	{
		// ---------------------------
		// 時間を更新
		// ---------------------------

		Time::Update();

		// ------------------------
		// エディタを更新
		// ------------------------

		// エディタでの操作がすぐに反映されるように
		// 先に更新する
		mainEditor.Update(currentScene, isPlayGame);

		// ------------------------------------------
		// シーン内のゲームオブジェクトを更新
		// ------------------------------------------

		currentScene->Update(isPlayGame);
	}

	/// <summary>
	/// ゲームエンジンの状態を描画する
	/// </summary>
	void Engine::Render()
	{
		// --------------------------------------
		// ウィンドウオブジェクトを取得
		// --------------------------------------

		// GLFWウィンドウオブジェクト
		GLFWwindow& window = Window::GetWindow(WindowID::Main);

		// --------------------------
		// バックバッファをクリア
		// --------------------------

		// バックバッファのクリア色を黒色に設定
		glClearColor(0, 0, 0, 0);

		// バックバッファをクリア
		// 今回はカラーバッファと深度バッファを指定
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		// ----------------------------
		// ビューポートを設定
		// ----------------------------

		// フレームバッファの大きさを取得
		int fbWidth, fbHeight;
		glfwGetFramebufferSize(&window, &fbWidth, &fbHeight);

		// ビューポートを設定
		glViewport(0, 0, fbWidth, fbHeight);

		// -------------------------
		// 半透明合成を有効化
		// -------------------------

		glEnable(GL_BLEND);
		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
		glBlendEquation(GL_FUNC_ADD);

		// --------------------------
		// エディタ画面の描画
		// --------------------------

		mainEditor.Render();
	}

#pragma endregion

} // namespace PokarinEngine