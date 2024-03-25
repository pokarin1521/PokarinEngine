/**
* @file Engine.cpp
*/
#include "Engine.h"

#include "Window.h"
#include "InputManager.h"

#include "Debug.h"
#include "LightManager.h"

#include "EasyAudio/EasyAudio.h"

#include "Components/Colliders/AabbCollider.h"
#include "Components/Colliders/SphereCollider.h"

#include "Settings/ShaderSettings.h"

#include "MainEditor/RenderView.h"

#include "NodeScript/NodeScript.h"

#include <fstream>
#include <filesystem>
#include <vector>
#include <cmath>
#include <stdio.h>

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
		// カメラパラメータを設定
		glProgramUniform3fv(prog, LocationNum::cameraPosition,
			1, &camera.position.x);

		// 回転の計算をするためにsin,cosで渡す
		// カメラの回転方向とオブジェクトの移動方向が逆なので、マイナスにする
		// (カメラが右に回転する = オブジェクトが左に回転する)
		glProgramUniform3f(prog, LocationNum::cameraRotation,
			-camera.rotation.x,
			-camera.rotation.y,
			-camera.rotation.z);
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
		// シーン作成
		auto scene = Scene::Create(*this, name);

		// エンジンに登録
		scenes.push_back(scene);

		return scene;
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
		editor.Finalize();

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

		// メインウィンドウ
		GLFWwindow* mainWindow = &Window::GetWindow(WindowID::Main);

		// --------------------------------
		// OpenGLコンテキストの作成
		// --------------------------------

		// コンテキスト作成
		glfwMakeContextCurrent(mainWindow);

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

		editor.Initialize(*this);

		// サブウィンドウに設定されている可能性があるので
		// コンテキストをメインウィンドウに設定する
		glfwMakeContextCurrent(&Window::GetWindow(WindowID::Main));

		// -----------------------
		// ライトを初期化
		// -----------------------

		LightManager::InitializeLight();

		// -----------------------
		// シェーダの初期化
		// -----------------------

		Shader::Initialize(shaderProgList);

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

		// -----------------------------------
		// シーンがなければ作成
		// -----------------------------------

		if (scenes.empty())
		{
			currentScene = Scene::Create(*this, "SampleScene");
		}
		
		// -----------------------------------------------
		// ノードスクリプトを初期化する
		// -----------------------------------------------

		NodeScript::Initialize();

		return true;
	}

	/// <summary>
	/// ゲームエンジンの状態を更新する
	/// </summary>
	void Engine::Update()
	{
		// ------------------------------------------
		// デルタタイム(前回の更新からの経過時間)
		// ------------------------------------------

		// デルタタイム(前回の更新からの経過時間)を計算
		const double currentTime = glfwGetTime(); // 現在時刻
		deltaTime = static_cast<float>(currentTime - previousTime);
		previousTime = currentTime;

		// 経過時間が長すぎる場合は適当に短くする(デバッグ対策)
		if (deltaTime >= 0.5f)
		{
			deltaTime = 1.0f / 60.0f;
		}

		// ------------------------------------------
		// シーン内のゲームオブジェクトを更新
		// ------------------------------------------

		currentScene->UpdateGameObject(deltaTime);

		// ------------------------------------
		// ゲームオブジェクトの衝突処理
		// ------------------------------------

		HandleGameObjectCollision();

		// ------------------------
		// エディタを更新
		// ------------------------

		editor.Update();
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
		//glClearColor(0.0f, 1.0f, 0.8f, 1.0f);
		glClearColor(0.2f, 0.2f, 0.2f, 1.0f);

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
		DrawRenderView(*currentScene->GetMainCamera().transform, editor.GetGameView());

		// シーンビュー用カメラを使用してシーンビューに描画する
		DrawRenderView(editor.GetSceneView().GetCamera(), editor.GetSceneView());

		// --------------------------
		// エディタ画面の描画
		// --------------------------

		editor.Render();
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

		// [シェーダプログラムの種類, シェーダプログラムの管理番号]
		for (auto& [type, prog] : shaderProgList)
		{
			// アスペクト比と視野角による拡大率を設定
			// GPU側での除算を避けるため、逆数にして渡す
			glProgramUniform2f(prog, LocationNum::aspectRatioAndScaleFov,
				1 / aspectRatio, fovScale);

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

		// -------------------------------
		// ライトデータをGPUにコピー
		// -------------------------------

		LightManager::Update(shaderProgList[Shader::ProgType::Standard], currentScene->GetMainCamera());

		// -----------------------------------
		// 描画用ビューのFBOをバインド
		// -----------------------------------

		renderView.BindFBO();

		// -----------------------------------
		// ゲームオブジェクトを描画
		// -----------------------------------

		// VAOをOpenGLコンテキストにバインド
		glBindVertexArray(*meshBuffer->GetVAO());

		// シーン内のゲームオブジェクトを描画する
		currentScene->DrawGameObjectAll(shaderProgList);

		// 誤操作のないようにバインドを解除
		glBindVertexArray(0);

		// ----------------------------------
		// 描画用ビューのバインド解除
		// ----------------------------------

		renderView.UnBindFBO();
	}

#pragma endregion

#pragma region Collision

	/// ここでしか使わないのでcppのみに書く
	/// <summary>
	/// 型によって交差判定関数を呼び分けるための関数テンプレート
	/// </summary>
	/// <typeparam name="T"> 判定対象の型(衝突する側) </typeparam>
	/// <typeparam name="U"> 判定対象の型(衝突される側) </typeparam>
	/// <param name="[in] a"> 判定対象のコライダー() </param>
	/// <param name="[in] b"> 判定対象のコライダー2 </param>
	/// <param name="[out] p"> 貫通ベクトル </param>
	/// <returns></returns>
	template<typename T, typename U>
	bool CallIntersect(
		const ColliderPtr& a, const ColliderPtr& b, Vec3& p)
	{
		return Intersect(static_cast<T&>(*a).GetShape(),
			static_cast<U&>(*b).GetShape(), p);
	}

	/// ここでしか使わないのでcppのみに書く
	/// <summary>
	/// <para> 型によって交差判定関数を呼び分けるための関数テンプレート </para>
	/// <para> 交差判定関数に渡す引数を逆数にするバージョン </para>
	/// </summary>
	/// <typeparam name="T"> 判定対象の型(衝突される側) </typeparam>
	/// <typeparam name="U"> 判定対象の型(衝突する側) </typeparam>
	/// <param name="[in] a"> 判定対象のコライダー(衝突される側) </param>
	/// <param name="[in] b"> 判定対象のコライダー(衝突する側) </param>
	/// <param name="[out] p"> 貫通ベクトル </param>
	/// <returns>
	/// <para> true  : 交差した </para>
	/// <para> false : 交差していない </para>
	/// </returns>
	template<typename T, typename U>
	bool CallIntersectReverse(
		const ColliderPtr& a, const ColliderPtr& b, Vec3& p)
	{
		if (Intersect(static_cast<U&>(*b).GetShape(),
			static_cast<T&>(*a).GetShape(), p))
		{
			// 衝突する側と衝突される側が逆になるので
			// 貫通ベクトルを逆向きにする
			p *= -1;

			return true;
		}

		return false;
	}

	/// <summary>
	/// ゲームオブジェクトの衝突を処理する
	/// </summary>
	void Engine::HandleGameObjectCollision()
	{
		// --------------------------------------
		// ワールド座標系の衝突判定を作成
		// --------------------------------------

		// ゲームオブジェクトごとのコライダーを管理する配列
		std::vector<WorldColliderList> colliders;

		// シーン内のゲームオブジェクト
		GameObjectList gameObjectList = currentScene->GetGameObjectAll();

		// ゲームオブジェクト分の容量を予約
		colliders.reserve(gameObjectList.size());

		// [ゲームオブジェクト識別番号, ゲームオブジェクト]
		for (auto& gameObject : gameObjectList)
		{
			// ゲームオブジェクトにコライダーがなければ何もしない
			if (gameObject->colliders.empty())
			{
				continue;
			}

			// 「接地していない」状態にする
			gameObject->isGrounded = false;

			// -------------------------------------------------
			// コライダーをコピーしてワールド座標に変換
			// -------------------------------------------------

			// ワールド座標系コライダーの配列
			WorldColliderList list(gameObject->colliders.size());

			// ワールド座標系に変換
			for (int i = 0; i < gameObject->colliders.size(); ++i)
			{
				// オブジェクトのコライダー
				ColliderPtr collider = gameObject->colliders[i];

				// 座標変換行列
				Mat4 transformMatrix = gameObject->transform->GetTransformMatrix();

				// ゲームオブジェクトのコライダーを設定
				list[i].origin = collider;

				// 座標変換をしたコライダーを設定
				list[i].world = collider->GetTransformedCollider(transformMatrix);
			}

			// ワールド座標系コライダーを追加
			colliders.push_back(list);
		}

		// ----------------------------------
		// コライダーの衝突判定
		// ----------------------------------

		// コライダーを持つオブジェクトが
		// 2つ以上ないと衝突判定できないので、個数チェック
		if (colliders.size() >= 2)
		{
			/* ゲームオブジェクトは複数のコライダーを持てるので、
			ゲームオブジェクトが持つ全てのコライダーを処理しなければならない */

			// 1つ目のオブジェクトのコライダー配列
			for (auto collidersA = colliders.begin(); collidersA != colliders.end() - 1; ++collidersA)
			{
				// 1つ目のオブジェクト
				const GameObject& objectA = collidersA->at(0).origin->GetOwner();

				// コライダーの持ち主が削除済みなら何もしない
				if (objectA.IsDestroyed())
				{
					continue;
				}

				// 2つ目のオブジェクトのコライダー配列
				for (auto collidersB = collidersA + 1; collidersB != colliders.end(); ++collidersB)
				{
					// 2つ目のオブジェクト
					const GameObject& objectB = collidersA->at(0).origin->GetOwner();

					// コライダーの持ち主が削除済みなら何もしない
					if (objectB.IsDestroyed())
					{
						continue;
					}

					// コライダー単位の衝突判定
					HandleWorldColliderCollision(&*collidersA, &*collidersB);

				} // for collidersB
			} // for collidersA
		}

	} // HandleGameObjectCollision

	/// <summary>
	/// コライダー単位の衝突判定
	/// </summary>
	/// <param name="collidersA"> 
	/// 判定対象のワールドコライダー配列(衝突する側) </param>
	/// <param name="collidersB"> 
	/// 判定対象のワールドコライダー配列(衝突される側) </param>
	void Engine::HandleWorldColliderCollision(
		WorldColliderList* collidersA, WorldColliderList* collidersB)
	{
		// 関数ポインタ型を定義
		using FuncType = bool(*)(
			const ColliderPtr&, const ColliderPtr&, Vec3&);

		// 組み合わせに対応する交差判定関数を選ぶための配列
		static const FuncType funcList[2][2] = {
			{
				CallIntersect<AabbCollider, AabbCollider>,
				CallIntersect<AabbCollider, SphereCollider>,
			},
			{
				CallIntersectReverse<SphereCollider, AabbCollider>,
				CallIntersect<SphereCollider, SphereCollider>,
			},
		};

		// ----------------------------------
		// コライダー単位の衝突判定
		// ----------------------------------

		// コライダー(衝突する側)
		for (auto& colA : *collidersA)
		{
			// コライダー(衝突される側)
			for (auto& colB : *collidersB)
			{
				// スタティックコライダー同士は衝突しない
				if (colA.origin->isStatic && colB.origin->isStatic)
				{
					continue;
				}

				// 貫通ベクトル
				Vec3 penetration = Vec3(0);

				// colAの図形の種類番号
				const int typeA = static_cast<int>(colA.origin->GetType());

				// colBの図形の種類番号
				const int typeB = static_cast<int>(colB.origin->GetType());

				// 衝突判定を行う
				if (funcList[typeA][typeB](
					colA.world, colB.world, penetration))
				{
					// 衝突したゲームオブジェクト
					GameObject& gameObjectA = colA.origin->GetOwner();

					// 衝突されたゲームオブジェクト
					GameObject& gameObjectB = colB.origin->GetOwner();

					// ----------------------------------------------
					// コライダーが重ならないように座標を調整
					// ----------------------------------------------

					// 両方が重複可能なコライダーでないことを確認
					if (!colA.origin->isTrigger && !colB.origin->isTrigger)
					{
						// Aは動かないので、Bを移動させる
						if (colA.origin->isStatic)
						{
							/* AがBにぶつかった際の処理なので、
							AがBに貫通した分、
							同じ方向にBを移動させる */

							// 貫通した分移動
							ApplyPenetration(
								collidersB, gameObjectB, penetration);
						}
						// Bは動かないので、Aを移動させる
						else if (colB.origin->isStatic)
						{
							/* AがBにぶつかった際の処理なので、
							AがBに貫通した分、
							逆方向にAを移動させる */

							// 貫通した分移動
							ApplyPenetration(
								collidersA, gameObjectA, -penetration);
						}
						// 両方動かないので、均等に移動させる
						else
						{
							// 貫通ベクトルの半分
							// 均等に移動させるため
							Vec3 halfPenetration = penetration * 0.5f;

							// 貫通距離の半分だけ
							// 貫通した方向に移動
							ApplyPenetration(
								collidersB, gameObjectB, halfPenetration);

							// 貫通距離の半分だけ
							// 逆方向に移動
							ApplyPenetration(
								collidersA, gameObjectA, -halfPenetration);
						}
					}

					// ------------------------------
					// 衝突イベントを実行
					// ------------------------------

					gameObjectA.OnCollision(colA.origin, colB.origin);
					gameObjectB.OnCollision(colB.origin, colA.origin);

					// イベントの結果、
					// どちらかのゲームオブジェクトが破棄された場合
					if (gameObjectA.IsDestroyed() || gameObjectB.IsDestroyed())
					{
						// 衝突処理を終了
						return;
					}
				}

			} // for colB 
		} // for col A
	}

	/// <summary>
	/// 貫通ベクトルをゲームオブジェクトに反映する
	/// </summary>
	/// <param name="worldColliders"> ワールド座標系のコライダー配列 </param>
	/// <param name="gameObject"> ゲームオブジェクト </param>
	/// <param name="penetration"> 貫通ベクトル </param>
	void Engine::ApplyPenetration(WorldColliderList* worldColliders,
		GameObject& gameObject, const Vec3& penetration)
	{
		// ---------------------------------------------------------
		// 接地判定
		// 貫通ベクトルが垂直に近い場合に、床に触れたとみなす
		// ---------------------------------------------------------

		/* 「貫通ベクトル」と「垂直ベクトル」の角度で判定する

		内積を使って角度を求める
		貫通ベクトル(px, py, pz)と垂直ベクトル(0, 1, 0)で計算すると
		「なす角 = 貫通ベクトルのY要素」ということが分かる

		そして、貫通ベクトルは衝突面に対して垂直に作られる
		(それが最短距離だから)
		そのため、「貫通ベクトルがワールド座標系の垂直に近い」
		ならば「衝突面はワールド座標系の水平に近い」と言える */

		// 床とみなす角度
		static const float cosGround = cos(Radians(30));

		// 貫通ベクトルが下方向を向いている
		// (下方向でない場合は、計算するだけ無駄なので計算前に判定する)
		if (penetration.y > 0)
		{
			// 貫通距離
			const float distance = Vec3_Function::Length(penetration);

			// 貫通ベクトルの向きが30度以下なら床と判断
			// acosを避けるためcosで比較しているので、
			// 角度で比較する時と不等号の向きが逆
			if (penetration.y >= distance * cosGround)
			{
				// 接地した
				gameObject.isGrounded = true;
			}
		}

		// ----------------------------------------
		// ゲームオブジェクトとコライダーに
		// 衝突による貫通分の移動を反映
		// ----------------------------------------

		// ゲームオブジェクトを移動
		gameObject.transform->position += penetration;

		// 全てのワールドコライダーを移動
		for (auto& collider : *worldColliders)
		{
			collider.world->AddPosition(penetration);
		}
	}

#pragma endregion

#pragma region FOV

	/// <summary>
	/// 垂直視野角を設定する
	/// </summary>
	/// <param name="fovY"> 設定する垂直視野角(度数法) </param>
	void Engine::SetFovY(float fovY)
	{
		degFovY = fovY; // 垂直視野角(度数法)を設定
		radFovY = degFovY * pi / 180; // 弧度法に変換
		fovScale = 1 / tan(radFovY / 2); // 視野角による拡大率の逆数
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
		// なければend()が入る
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

		// コンストラクタ、デストラクタを
		// 呼べるようにするための補助クラス
		struct TexHelper : public Texture
		{
			TexHelper(const char* p) : Texture(p) {}
		};

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

		// コンストラクタ、デストラクタを
		// 呼べるようにするための補助クラス
		struct TexHelper : public Texture
		{
			TexHelper(GLsizei w, GLsizei h) : Texture(w, h) {}
		};

		// テクスチャ
		std::shared_ptr<TexHelper> tex;

		// テクスチャを作成
		tex = std::make_shared<TexHelper>(width, height);

		// 作成したテクスチャを返す
		return tex;
	}

#pragma endregion

#pragma region Ray

	/// <summary>
	/// マウス座標から発射される光線を取得する
	/// </summary>
	/// <returns> マウス座標から発射される光線 </returns>
	Ray Engine::GetRayFromMousePosition() const
	{
		// ----------------------------------------------
		// スクリーン座標系のマウスカーソル座標を取得
		// ----------------------------------------------

		// マウスカーソル座標
		Vec2 mousePos = Input::Mouse::GetScreenPos(WindowID::Main);

		// -------------------------------------
		// スクリーン座標系からNDC座標系
		// (正規化デバイス座標系)に変換
		// -------------------------------------

		/* NDC座標系は-1 ～ +1の範囲の座標系

		Windowsのスクリーン座標系は
		画面のピクセル数をそのまま表現する座標系

		そこで、スクリーン座標をピクセル数で割る
		これで0 ～ 1の値になるので、2倍して1引くとNDC座標が得られる

		また、Windowsのスクリーン座標系のY軸は下がプラス、
		NDC座標系のY軸は上がプラスなので、Y軸を符号反転する */

		// スクリーンのピクセル数
		Vec2 windowSize = Window::GetWindowSize(WindowID::Main);

		// NDC座標系に変換

		// 光線の発射点の座標
		Vec3 nearPos = {
			static_cast<float>(mousePos.x / windowSize.x * 2 - 1),
			-static_cast<float>(mousePos.y / windowSize.y * 2 - 1),
			-1 };

		// 光線の終着点の座標
		Vec3 farPos = { nearPos.x, nearPos.y, 1 };

		// -------------------------------------
		// NDC座標系からクリップ座標系に変換
		// -------------------------------------

		/* NDC座標系は、
		クリップ座標系のX,Y,Z要素をW要素で除算した座標系

		なので、頂点シェーダの計算、GPUがする「wによる除算」を
		逆に実行する */

		// --------- 深度値の計算結果が-1 ～ +1になるような -------------
		// --------- パラメータA,Bを計算					-------------
		// --------- (頂点シェーダの値と一致させる)			-------------

		// 描画開始距離
		const float near = 0.35f;

		// 描画終了距離
		const float far = 1000;

		// -1 <= A / 深度値 + B <= 1 となる定数A,Bを求める
		// (standard.vertの式と同じ)
		const float A = -2 * far * near / (far - near);
		const float B = (far + near) / (far - near);

		// ------- クリップ座標系に変換 --------

		//「wによる除算」の逆なので
		// 深度値を掛ける
		nearPos *= near;
		farPos *= far;

		// 頂点シェーダで 深度値 * B + A をしたので、
		// 逆算して計算前の深度値を求める
		nearPos.z = (nearPos.z - A) / B;
		farPos.z = (farPos.z - A) / B;

		// ----------------------------------------
		// クリップ座標系からビュー座標系に変換
		// ----------------------------------------

		/* 頂点シェーダで、
		Xをアスペクト比で割った後、X,YをFOVで割ることで、
		アスペクト比とFOVを反映させたので、
		その逆を実行してビュー座標系に変換する

		ただし、余計な除算を省くために、逆数で乗算していたので、
		逆数で割る必要がある */

		// アスペクト比
		const float aspectRatio = Window::GetAspectRatio(WindowID::Main);

		// 余分な除算を省くため、
		// 逆数になっているFOVを逆数にして元に戻す
		const float invFovScale = 1.0f / GetFovScale();

		// FOVを掛けてビュー座標系に変換する
		// Xにはアスペクト比も掛ける
		nearPos.x *= invFovScale * aspectRatio;
		nearPos.y *= invFovScale;

		farPos.x *= invFovScale * aspectRatio;
		farPos.y *= invFovScale;

		// ----------------------------------------
		// ビュー座標系からワールド座標系に変換
		// ----------------------------------------

		/* ワールド座標系からビュー座標系への変換では
		「カメラの回転の逆」の回転をした
		その逆をするので、カメラの回転を適用することで変換する

		また、ワールド座標系からビュー座標系への変換では
		「カメラ座標を減算してから回転」をしていた
		その逆をするので、「回転してからカメラの座標を加算」する */

		// メインカメラ
		TransformPtr mainCamera = currentScene->GetMainCamera().transform;

		// カメラのY軸回転用sin
		const float cameraSinY = std::sin(mainCamera->rotation.y);

		// カメラのY軸回転用cos
		const float cameraCosY = std::cos(mainCamera->rotation.y);

		// カメラの回転を適用
		// 発射点は一番手前の座標なので、Z座標はnear
		nearPos = {
			nearPos.x * cameraCosY - near * cameraSinY,
			nearPos.y,
			nearPos.x * -cameraSinY - near * cameraCosY };

		// カメラの座標を加算して変換完了
		nearPos += mainCamera->position;

		// 終着点は一番奥の座標なので、Z座標はfar
		farPos = {
			farPos.x * cameraCosY - far * cameraSinY,
			farPos.y,
			farPos.x * -cameraSinY - far * cameraCosY };

		// カメラの座標を加算して変換完了
		farPos += mainCamera->position;

		// -----------------------------------
		// 近平面の座標と遠平面の座標から
		// 光線の向きベクトルを求める
		// -----------------------------------

		// 光線の向きベクトル
		Vec3 direction = farPos - nearPos;

		// 正規化するために、ベクトルの長さを求める
		const float length = sqrt(
			direction.x * direction.x +
			direction.y * direction.y +
			direction.z * direction.z);

		// 正規化
		direction *= 1.0f / length;

		return Ray{ nearPos, direction };
	}

	/// <summary>
	/// 光線とコライダーの交差判定
	/// </summary>
	/// <param name="[in] ray"> 光線 </param>
	/// <param name="[out] hitInfo"> 光線と最初に交差したコライダーの情報 </param>
	/// <param name="pred"> 交差判定を行うコライダーを選別する述語 </param>
	/// <returns>
	/// <para> true : コライダーと交差した </para>
	/// <para> false : コライダーと交差しなかった </para>
	/// </returns>
	bool Engine::Raycast(const Ray& ray, RaycastHit& hitInfo,
		const RaycastPredicate& pred) const
	{
		// 交点の情報を初期化
		hitInfo.collider = nullptr;
		hitInfo.distance = FLT_MAX;

		// [ゲームオブジェクト識別番号, ゲームオブジェクト]
		for (auto& gameObject : currentScene->GetGameObjectAll())
		{
			// コライダー
			for (auto& collider : gameObject->colliders)
			{
				// コライダーをワールド座標系に変換
				const auto worldCollider = collider->GetTransformedCollider(
					gameObject->transform->GetTransformMatrix());

				// --------------------
				// 光線との交差判定
				// --------------------

				// 光線がAABBと最初に交差するまでの距離
				float distance = 0;

				// 交差判定結果
				bool hit = false;

				// コライダーの図形種類によって処理を分岐
				switch (collider->GetType())
				{
				case Collider::Type::AABB:

					AABB aabb =
						static_cast<AabbCollider&>(*worldCollider).aabb;

					// 交差判定
					hit = Intersect(aabb, ray, distance);

					break;

				case Collider::Type::Sphere:

					Sphere sphere =
						static_cast<SphereCollider&>(*worldCollider).sphere;

					// 交差判定
					hit = Intersect(sphere, ray, distance);

					break;
				}

				// 光線とコライダーが
				// 交差しなければ次のコライダーへ
				if (!hit)
				{
					continue;
				}

				// 交差判定の対象でなければ次のコライダーへ
				if (!pred(collider, distance))
				{
					continue;
				}

				// より発射点に近い交点を持つコライダーを選ぶ
				if (distance < hitInfo.distance)
				{
					// 情報を更新
					hitInfo.collider = collider;
					hitInfo.distance = distance;
				}

			} // for colliders
		} // for gameObjectList

		// ------------------------------------
		// 交差するコライダーがあるか確認
		// ------------------------------------

		// 交差するコライダーがある
		if (hitInfo.collider)
		{
			// 交点の座標を計算
			hitInfo.point = ray.start + ray.direction * hitInfo.distance;

			return true;
		}

		// 交差するコライダーがない
		return false;
	}

#pragma endregion

} // namespace PokarinEngine