/**
* @file Engine.cpp
*/
#include "Engine.h"

#include "Window.h"
#include "InputManager.h"

#include "Debug.h"
#include "Time.h"
#include "Random.h"
#include "LightParameter.h"

#include "Math/Matrix.h"

#include "Components/Colliders/AabbCollider.h"
#include "Components/Colliders/SphereCollider.h"

#include "Configs/ShaderConfig.h"

#include "MainEditor/MainEditor.h"
#include "MainEditor/RenderView.h"
#include "MainEditor/SceneView.h"
#include "MainEditor/GameView.h"

#include "NodeScript/NodeScript.h"

#include <fstream>
#include <filesystem>
#include <vector>
#include <cmath>
#include <cstdio>

namespace PokarinEngine
{
#pragma region CopyParameter

	/// <summary>
	/// カメラのパラメータをGPUにコピー
	/// </summary>
	/// <param name="prog"> 使用するシェーダプログラムの管理番号 </param>
	/// <param name="camera"> GPUにコピーするカメラ </param>
	void CopyCameraParameters(
		GLuint prog, const Transform& camera)
	{
		// ----------------------------
		// カメラの座標
		// ----------------------------

		// 今は左手座標系の値になっていて、
		// OpenGLは右手座標系なので、右手座標系にする
		Vector3 position = camera.position;
		position.z *= -1;

		// カメラの座標をGPUにコピー
		glProgramUniform3fv(prog, UniformLocation::cameraPosition, 1, &position.x);

		// --------------------------------
		// カメラの回転角度
		// --------------------------------

		// カメラの回転
		// オブジェクトはカメラの回転方向とは逆に動くことになるので、符号を逆にする
		Vector3 rotation = -camera.rotation;

		// カメラの回転角度をGPUにコピー
		glProgramUniform3fv(prog, UniformLocation::cameraRotation, 1, &rotation.x);
	}

#pragma endregion

#pragma region Scene

	/// <summary>
	/// シーンを作成する
	/// </summary>
	/// <param name="name"> シーン名 </param>
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
		auto scene = std::make_shared<Scene>(*this, sceneID, name);

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
	/// <para> true : 正常に実行が完了した </para>
	/// <para> false : エラーが発生した </para>
	/// </returns>
	bool Engine::Run()
	{
		// ゲームエンジンを初期化する
		// 初期化に失敗したら異常終了
		if (!Initialize())
		{
			return false;
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

				// 破棄予定のゲームオブジェクトを削除する
				currentScene->RemoveDestroyedGameObject();

				Window::End();
			}

			// スクリプトグラフウィンドウ
			if (Window::Begin(WindowID::NodeScript))
			{
				NodeScript::Update();
				NodeScript::Render();

				Window::End();
			}
		}

		// エディタを終了
		MainEditor::Finalize();

		// ノードスクリプトの終了
		NodeScript::Finalize();

		// GLFWの終了
		glfwTerminate();

		return true;
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
		MainEditor::Initialize(*this);

		// -----------------------
		// シェーダの初期化
		// -----------------------

		Shader::Initialize();

		// ---------------------------------------
		// メッシュバッファを作成
		// テクスチャ作成コールバックを設定
		// ---------------------------------------

		// メッシュバッファのサイズは32メガバイトとする
		// 不足した場合はこの値を大きくする
		meshBuffer = MeshBuffer::Create(32'000'000);

		// テクスチャ作成コールバックを設定
		meshBuffer->SetTextureCallback(
			[this](const char* filename) { return GetTexture(filename); });

		// -----------------------------------
		// OBJファイルを読み込む
		// -----------------------------------

		// 全てのOBJファイルを読み込む
		for (const char* obj : StaticMeshFile_OBJ::allObj)
		{
			meshBuffer->LoadOBJ(obj);
		}

		// ----------------------------------
		// スカイスフィアを取得する
		// ----------------------------------

		skySphere = meshBuffer->GetStaticMesh(StaticMeshFile_OBJ::skySphere);

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

		// ------------------------------------------
		// シーン内のゲームオブジェクトを更新
		// ------------------------------------------

		currentScene->UpdateGameObject();

		// ------------------------
		// エディタを更新
		// ------------------------

		MainEditor::Update(isPlayGame);
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

		// バックバッファをクリアするときの色を指定
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

		// ------------------------------------------
		// 描画用ビューに描画情報を保持させる
		// ------------------------------------------

		// メインカメラを使用してゲームビューに描画する
		DrawRenderView(*currentScene->GetMainCamera().transform, MainEditor::GetGameView());

		// シーンビュー
		const SceneView& sceneView = MainEditor::GetSceneView();

		// シーンビュー用カメラを使用してシーンビューに描画する
		DrawRenderView(sceneView.GetCamera(), sceneView);

		// --------------------------
		// エディタ画面の描画
		// --------------------------

		MainEditor::Render();
	}

	/// <summary>
	/// 描画用ビューにカメラからの描画情報を保持させる
	/// </summary>
	/// <param name="camera"> 使用するカメラ </param>
	/// <param name="renderView"> 描画情報を保持する描画用ビュー </param>
	void Engine::DrawRenderView(const Transform& camera, const RenderView& renderView)
	{
		// -------------------------------------
		// GPUにデータをコピー
		// -------------------------------------

		// アスペクト比と視野角を設定
		const float aspectRatio = Window::GetAspectRatio(WindowID::Main);

		// 全てのシェーダプログラムの管理番号
		Shader::ProgList allProg = Shader::GetAllProgram();

		// [シェーダプログラムの種類, シェーダプログラムの管理番号]
		for (auto& [type, prog] : allProg)
		{
			// アスペクト比と視野角による拡大率を設定
			// GPU側での除算を避けるため、逆数にして渡す
			glProgramUniform2f(prog, UniformLocation::aspectRatioAndScaleFov,
				1 / aspectRatio, currentScene->GetMainCameraInfo().GetFovScale());

			// カメラの情報をGPUにコピー
			CopyCameraParameters(prog, camera);
		}

		// -------------------------
		// 深度テストを有効化
		// -------------------------

		glEnable(GL_DEPTH_TEST);

		// -------------------------
		// 半透明合成を有効化
		// -------------------------

		glEnable(GL_BLEND);
		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
		glBlendEquation(GL_FUNC_ADD);

		// ----------------------------------
		// ライト情報をGPUにコピー
		// ----------------------------------

		LightParameter::CopyGPU(currentScene->GetMainCamera());

		// -----------------------------------
		// 描画用ビューのFBOをバインド
		// -----------------------------------

		renderView.BindFBO();

		// -----------------------------------
		// スカイスフィアを描画
		// -----------------------------------

		DrawSkySphere();

		// -----------------------------------
		// ゲームオブジェクトを描画
		// -----------------------------------

		// VAOをOpenGLコンテキストにバインド
		glBindVertexArray(*meshBuffer->GetVAO());

		// シーン内のゲームオブジェクトを描画する
		currentScene->DrawGameObjectAll();

		// 誤操作のないようにバインドを解除
		glBindVertexArray(0);

		// ----------------------------------
		// 描画用ビューのバインド解除
		// ----------------------------------

		renderView.UnBindFBO();
	}

#pragma endregion

#pragma region Texture

	/// <summary>
	/// <para> テクスチャを取得する </para>
	/// </summary>
	/// <param name="name"> テクスチャファイル名 </param>
	/// <returns> 名前がnameと一致するテクスチャ </returns>
	TexturePtr Engine::GetTexture(const char* name)
	{
		// ----------------------------------------
		// キャッシュにあれば
		// キャッシュされたテクスチャを返す
		// ----------------------------------------

		// テクスチャを検索
		// なければendが入る
		auto itr = textureCache.find(name);

		// テクスチャが見つかった場合
		if (itr != textureCache.end())
		{
			// キャッシュされたテクスチャを返す
			return itr->second;
		}

		// --------------------------------------------
		// キャッシュになければ
		// テクスチャを読み込んでキャッシュに登録
		// --------------------------------------------

		/* make_shared関数はEngineクラスのメンバ関数ではない
		なので、補助クラスを作成して
		間接的にコンストラクタ、デストラクタを呼び出す */

		// テクスチャ
		std::shared_ptr<TexHelper> tex;

		// ファイルからテクスチャを読み込む
		tex = std::make_shared<TexHelper>(name);

		// テクスチャをキャッシュに登録
		textureCache.emplace(name, tex);

		// テクスチャを返す
		return tex;
	}

	/// <summary>
	/// テクスチャを取得する
	/// </summary>
	/// <param name="width"> 幅 </param>
	/// <param name="height"> 高さ </param>
	/// <returns> 指定した大きさのテクスチャ </returns>
	TexturePtr Engine::GetTexture(GLsizei width, GLsizei height)
	{
		/* make_shared関数はEngineクラスのメンバ関数ではない
		なので、補助クラスを作成して
		間接的にコンストラクタ、デストラクタを呼び出す */

		// テクスチャ
		std::shared_ptr<TexHelper> tex;

		// テクスチャを作成
		tex = std::make_shared<TexHelper>(width, height);

		// 作成したテクスチャを返す
		return tex;
	}

#pragma endregion

#pragma region SkySphere

	/// <summary>
	/// スカイスフィアを描画する
	/// </summary>
	/// <param name="skySphereMaterial"> スカイスフィア用マテリアル </param>
	void Engine::DrawSkySphere(const MaterialPtr skySphereMaterial)
	{
		// --------------------------------------------------------
		// スカイスフィア用モデルがない場合は描画しない
		// --------------------------------------------------------

		if (!skySphere)
		{
			return;
		}

		// -------------------------------------
		// 使用するシェーダを指定する
		// -------------------------------------

		// ライティング無しのシェーダ
		static const GLuint progUnlit = Shader::GetProgram(Shader::ProgType::Unlit);

		// 空にライティングすると不自然なので
		// アンリットシェーダで描画
		glUseProgram(progUnlit);

		// VAOにバインド
		glBindVertexArray(*meshBuffer->GetVAO());

		// 深度バッファへの書き込みを禁止
		glDepthMask(GL_FALSE);

		// ----------------------------------------
		// 座標変換行列をGPUにコピーする
		// ----------------------------------------

		/* スカイスフィアは移動と回転はしないので拡大率だけを設定する

		スカイスフィアは最も遠くに描画される物体なので、
		カメラが描画できる範囲のうち、できるだけ遠い位置に描画したい

		現在の最大描画範囲を使いたいが
		スカイスフィアもポリゴンモデルなので微妙な凹凸がある
		凹凸が範囲からはみ出さないように*/

		// 最大描画範囲
		const float far = currentScene->GetMainCameraInfo().GetDrawFar();

		// スカイスフィアの半径
		static const float skySphereRadius = 0.5f;

		// 拡大率
		// 最大描画範囲の95%の位置に描画できるように設定
		const float scale = far * 0.95f / skySphereRadius;

		// 座標変換行列
		const Matrix4x4 transformMatrix = {
			{ scale,     0,     0,     0 },
			{     0, scale,     0,     0 },
			{     0,     0, scale,     0 },
			{     0,     0,     0, scale },
		};

		// 座標変換行列をGPUにコピー
		glProgramUniformMatrix4fv(progUnlit, UniformLocation::transformMatrix,
			1, GL_FALSE, &transformMatrix[0].x);

		// -----------------------------------
		// 色をGPUメモリにコピー
		// -----------------------------------

		// 色はマテリアルカラーで調整するので白を設定
		// (実際に描画される色は「オブジェクトカラー」と「マテリアルカラー」の乗算)
		static const Color color = { 1, 1, 1, 1 };
		glProgramUniform4fv(progUnlit, UniformLocation::color, 1, &color.r);

		// -----------------------------------
		// カメラの座標をGPUにコピー
		// -----------------------------------

		// スカイスフィアは常にカメラを中心に描画したいので、
		// カメラを一時的に原点に移動させる
		static const Vector3 skySphereCameraPos = { 0, 0, 0 };
		glProgramUniform3fv(progUnlit, UniformLocation::cameraPosition,
			1, &skySphereCameraPos.x);

		// -----------------------------------
		// スカイスフィアを描画する
		// -----------------------------------

		// 描画できるように配列に格納
		const MaterialList materials(1, skySphereMaterial);

		// スカイスフィア用マテリアルが指定されているならそれを使う
		if (skySphereMaterial)
		{
			// スカイスフィアを描画する
			Draw(*skySphere, progUnlit, materials);
		}
		// 指定されていないならスタティックメッシュにあるマテリアルを使う
		else
		{
			// スカイスフィアを描画する
			Draw(*skySphere, progUnlit, skySphere->materials);
		}

		// カメラの座標を元に戻す
		glProgramUniform3fv(progUnlit, UniformLocation::cameraPosition,
			1, &currentScene->GetMainCamera().transform->position.x);

		// 深度バッファへの書き込みを許可
		glDepthMask(GL_TRUE);

		// 標準シェーダに戻す
		glUseProgram(Shader::GetProgram(Shader::ProgType::Standard));
	}

#pragma endregion

} // namespace PokarinEngine