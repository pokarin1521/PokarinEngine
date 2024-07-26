/**
* @file Scene.cpp
*/
#include "Scene.h"

#include "Json/Json.h"

#include "Engine.h"
#include "Random.h"
#include "Debug.h"

#include "Components/Light.h"
#include "Components/Colliders/BoxCollider.h"

#include "Configs/ShaderConfig.h"

#include "Collision/Collision.h"

#include <algorithm>
#include <filesystem>

namespace PokarinEngine
{
#pragma region Create

	/// <summary>
	/// シーン作成用コンストラクタ
	/// </summary>
	/// <param name="[in] e"> エンジンクラスの参照 </param>
	/// <param name="[in] sceneID"> シーン識別番号 </param>
	/// <param name="[in] sceneName"> シーン名 </param>
	Scene::Scene(Engine& e, int sceneID, const char* sceneName)
		:engine(&e), id(sceneID), name(sceneName)
	{
		// メインカメラ作成
		// オブジェクト生成時に見えるように少し後ろに配置する
		mainCamera = CreateGameObject("MainCamera", Vector3(0, 0, -5));
		mainCameraInfo = mainCamera->AddComponent<Camera>();

		// 平行光源を作成
		auto directionalLight = CreateGameObject("Directional Light");
		std::shared_ptr<Light> lightComponent = directionalLight->AddComponent<Light>();
		lightComponent->SetType(LightParameter::Type::directional);
	}

	/// <summary>
	/// ゲームオブジェクトを作成する
	/// </summary>
	/// <typeparam name="T"> ゲームオブジェクトクラスまたはその派生 </typeparam>
	/// <param name="[in] name"> オブジェクトの名前 </param>
	/// <param name="[in] position"> オブジェクトを配置する位置 </param>
	/// <param name="[in] rotation"> オブジェクトの回転角度 </param>
	/// <param name="[in] staticMeshFile"> スタティックメッシュのファイル名 </param>
	/// <returns> 追加したゲームオブジェクトのポインタ </returns>
	GameObjectPtr Scene::CreateGameObject(const std::string& name,
		const Vector3& position, const Vector3& rotation, const char* staticMeshFile)
	{
		// シーン内のゲームオブジェクト数が最大値に達したら作成しない
		if (gameObjectList.size() >= gameObjectMax)
		{
			LOG_WARNING("ゲームオブジェクト数が最大値に達したため、ゲームオブジェクトを作成出来ません");
			return nullptr;
		}

		// ゲームオブジェクト作成
		GameObjectPtr object = std::make_shared<GameObject>();

		// ゲームオブジェクトの初期化
		object->Initialize(*this, GetSingleObjectID(),
			staticMeshFile, name,
			position, rotation);

		// ゲームオブジェクト管理用配列に追加
		gameObjectList.push_back(object);

		return object;
	}

	/// <summary>
	/// ゲームオブジェクトを複製する
	/// </summary>
	/// <param name="[in] object"> 複製元のゲームオブジェクト </param>
	void Scene::CopyGameObject(const GameObjectPtr& object)
	{
		// メインカメラは複製しない
		if (object == mainCamera)
		{
			return;
		}

		// 複製元の情報を格納するJson型
		Json data;

		// 複製元の情報を格納する
		object->ToJson(data);

		// ゲームオブジェクトを作成
		GameObjectPtr copyObject = CreateGameObject("Copy Object");

		// 複製元の情報を取得する
		copyObject->FromJson(data);
	}

#pragma endregion

#pragma region ID

	/// <summary>
	/// オブジェクトの識別番号を取得する
	/// </summary>
	/// <returns> 重複しない識別番号 </returns>
	int Scene::GetSingleObjectID()
	{
		// オブジェクト数がint型の最大値まで達したらIDを生成しない
		// INT_MIN ～ INT_MAXまで作れるが余裕を持つために半分にする
		if (objectIDList.size() >= INT_MAX)
		{
			LOG_WARNING("オブジェクト数が最大値に達したため、IDを生成出来ません");
			return 0;
		}

		// オブジェクト識別番号
		int objectID = Random::Range(INT_MIN, INT_MAX);

		// 他のオブジェクトと番号が重複しているなら乱数を取得し直す
		while (!objectIDList.emplace(objectID).second)
		{
			objectID = Random::Range(INT_MIN, INT_MAX);
		}

		return objectID;
	}

#pragma endregion

#pragma region StaticMesh

	/// <summary>
	/// スタティックメッシュを取得する
	/// </summary>
	/// <param name="[in] fileName"> ファイル名 </param>
	/// <returns> ファイル名が一致するスタティックメッシュ </returns>
	StaticMeshPtr Scene::GetStaticMesh(const std::string& fileName)
	{
		return engine->GetStaticMesh(fileName);
	}

#pragma endregion

#pragma region Update

	/// <summary>
	/// ゲームオブジェクトの状態を更新する
	/// </summary>
	void Scene::UpdateGameObject()
	{
		// ゲームオブジェクトを更新
		for (const auto& gameObject : gameObjectList)
		{
			if (!gameObject->IsDestroyed())
			{
				gameObject->Update(engine->IsPlayGame());
			}
		}

		if (engine->IsPlayGame())
		{
			Collision::GameObjectCollision(gameObjectList);
		}

	} // UpdateGameObject

#pragma endregion

#pragma region Destroy

	/// <summary>
	/// シーンから全てのゲームオブジェクトを破棄する
	/// </summary>
	void Scene::ClearGameObject()
	{
		// ゲームオブジェクトの削除イベントを実行
		for (auto& gameObject : gameObjectList)
		{
			gameObject->OnDestroy();
		}

		// ゲームオブジェクトを全削除
		gameObjectList.clear();

		// オブジェクトの識別番号を全削除
		objectIDList.clear();
	}

	/// <summary>
	/// ゲームオブジェクトを削除する
	/// </summary>
	/// <param name="[in] object"> 削除するゲームオブジェクト </param>
	void Scene::DestroyObject(GameObjectPtr& object)
	{
		// メインカメラは削除しない
		if (object == mainCamera)
		{
			return;
		}

		// -----------------------------------
		// ゲームオブジェクトを削除
		// -----------------------------------

		object->Destroy();

		// -------------------------------------
		// オブジェクト識別番号を削除
		// -------------------------------------

		objectIDList.erase(object->GetID());
	}

	/// <summary>
	/// 削除するゲームオブジェクトを完全に削除する
	/// </summary>
	void Scene::RemoveDestroyedGameObject()
	{
		// ゲームオブジェクトを持っていなければ何もしない
		if (gameObjectList.empty())
		{
			return;
		}

		/* 処理がしやすいように削除の有無で振り分け
		削除しないゲームオブジェクトを配列の前側に、
		削除するゲームオブジェクトを後ろ側に集める

		std::stable_partition(範囲の先頭, 範囲の終端, 振り分け条件)
		位置関係を保ちながら振り分ける */

		// 削除の有無で振り分けた後の、
		// 削除するオブジェクトの先頭イテレータ
		const auto destoryBegin = std::stable_partition(
			gameObjectList.begin(), gameObjectList.end(),
			[](const GameObjectPtr& e) {return !e->IsDestroyed(); });

		// 削除するオブジェクト配列
		GameObjectList destroyList(
			std::move_iterator(destoryBegin),
			std::move_iterator(gameObjectList.end()));

		// 配列から移動済みオブジェクトを削除
		gameObjectList.erase(destoryBegin, gameObjectList.end());

		// 削除するオブジェクトの削除イベントを実行
		for (auto& destoryObject : destroyList)
		{
			destoryObject->OnDestroy();
		}

		// ここで実際にゲームオブジェクトが削除される(destroyListの寿命が終わるため)
	}

#pragma endregion

#pragma region Draw

	/// ここでしか使わないので、cppのみに書く
	/// <summary>
	/// ゲームオブジェクトのパラメータをGPUにコピーする
	/// </summary>
	/// <param name="[in] prog"> シェーダプログラムの管理番号 </param>
	/// <param name="[in] gameObject"> パラメータをコピーするゲームオブジェクト </param>
	void CopyGameObjectParameters(GLuint prog, const GameObjectPtr& gameObject)
	{
		// 座標変換行列
		Matrix4x4 transformMatrix = gameObject->transform->GetTransformMatrix();

		// 法線変換行列
		Matrix3x3 normalMatrix = gameObject->transform->GetNormalMatrix();

		// ------------------------------------
		// パラメータをGPUにコピー
		// ------------------------------------

		// オブジェクトの色
		glProgramUniform4fv(prog,
			UniformLocation::color, 1, &gameObject->color.r);

		// 座標変換行列
		glProgramUniformMatrix4fv(
			prog, UniformLocation::transformMatrix,
			1, GL_FALSE, &transformMatrix[0].x);

		// 法線変換行列
		if (prog == Shader::GetProgram(Shader::ProgType::Standard))
		{
			glProgramUniformMatrix3fv(
				prog, UniformLocation::normalMatrix,
				1, GL_FALSE, &normalMatrix[0].x);
		}
	}

	/// ここでしか使わないので、cppのみに書く
	/// <summary>
	/// ゲームオブジェクトを描画する
	/// </summary>
	/// <param name="[in] prog"> シェーダプログラムの管理番号 </param>
	/// <param name="[in] begin"> 描画するゲームオブジェクト配列の先頭イテレータ </param>
	/// <param name="[in] end"> 描画するゲームオブジェクト配列の末尾イテレータ </param>
	void DrawGameObject(GLuint prog,
		GameObjectList::const_iterator begin, GameObjectList::const_iterator end)
	{
		// ------------------------
		// オブジェクトの描画
		// ------------------------

		// ゲームオブジェクトを描画
		for (auto& itr = begin; itr != end; ++itr)
		{
			// ゲームオブジェクト
			GameObjectPtr gameObject = *itr;

			// 削除済みのゲームオブジェクトは描画しない
			if (gameObject->IsDestroyed())
			{
				continue;
			}

			// スタティックメッシュがない場合
			if (!gameObject->staticMesh)
			{
				// 描画せず次のオブジェクトへ
				continue;
			}

			// -------- ユニフォーム変数にデータをコピー -----------

			// 描画に使うシェーダを指定
			glUseProgram(prog);

			// パラメータをコピー
			CopyGameObjectParameters(prog, gameObject);

			// ------------- 図形を描画 --------------

			// 固有マテリアルがない
			if (gameObject->materials.empty())
			{
				// 共有マテリアルを使って
				// スタティックメッシュを描画
				DrawMesh(gameObject->staticMesh, prog, gameObject->staticMesh->materials);
			}
			// 固有マテリアルがある
			else
			{
				// 固有マテリアルを使って
				// スタティックメッシュを描画
				DrawMesh(gameObject->staticMesh, prog, gameObject->materials);
			}
		}
	}

	/// <summary>
	/// シーン内の全てのゲームオブジェクトを描画する
	/// </summary>
	void Scene::DrawGameObjectAll()
	{
		// ------------------------------
		// 描画優先度で並べ替え
		// ------------------------------

		// 描画するゲームオブジェクト配列
		// 本体の順番が変わると困るのでコピー
		GameObjectList drawObjectList = gameObjectList;

		// ゲームオブジェクトを描画順に並べ替える
		// (描画優先度が低いものから順に並べる)
		std::stable_sort(drawObjectList.begin(), drawObjectList.end(),
			[](const GameObjectPtr& a, const GameObjectPtr& b) {
				return a->renderQueue < b->renderQueue; });

		/* ソート済みの範囲に対して検索を行うには、lower_bound関数を使う

		lower_bound(範囲の先頭, 範囲の終端, 検索する値, 検索条件)

		検索条件を満たさない最初の要素の位置を返す

		検索する値にRenderQueue_overlay、
		検索条件を「renderQueue_overlay < 検索する値」とすると、
		「RenderQueue_overlay以上の最初の要素の位置」を返す

		該当する要素が見つからない場合は、範囲の終端を返す */

		// 半透明な図形の先頭を検索
		const auto transparentBegin = std::lower_bound(
			drawObjectList.begin(), drawObjectList.end(), renderQueue_transparent,
			[](const GameObjectPtr& e, int value) {
				return e->renderQueue < value; });

		// UI・エフェクトなどの先頭を検索
		const auto overlayBegin = std::lower_bound(
			transparentBegin, drawObjectList.end(), renderQueue_overlay,
			[](const GameObjectPtr& e, int value) {
				return e->renderQueue < value; });

		// ---------------------------------
		// 優先度順に描画
		// ---------------------------------

		// 標準シェーダプログラムの管理番号
		GLuint progStandard = Shader::GetProgram(Shader::ProgType::Standard);

		// ライティング無しシェーダプログラムの管理番号
		GLuint progUnlit = Shader::GetProgram(Shader::ProgType::Unlit);

		// ---------- transparent以前のキューを描画  -----------
		// ---------- 通常のオブジェクト		     -----------

		// 描画
		DrawGameObject(progUnlit, drawObjectList.begin(), transparentBegin);

		// ------ transparentからoverlayまでのキューを描画 ------
		// ------ 半透明なオブジェクト					   ------

		// 半透明なオブジェクトなので後ろのオブジェクトが見えるように
		// 深度バッファへの書き込みを禁止
		glDepthMask(GL_FALSE);

		// 描画
		DrawGameObject(progStandard, transparentBegin, overlayBegin);

		// 後ろのオブジェクトが見えるのは半透明なオブジェクトだけなので
		// 深度バッファへの書き込みを許可
		glDepthMask(GL_TRUE);

		// ---------------- overlay以降のキューを描画 ----------------
		// ---------------- UI,全画面エフェクトなど   ----------------

		// overlay以降は
		// UI系の深度値関係なく描画して欲しいオブジェクトなので、
		// 深度テストを無効化する
		glDisable(GL_DEPTH_TEST);

		// 描画
		// UIにライティングはいらないのでアンリットシェーダを使う
		DrawGameObject(progUnlit, overlayBegin, drawObjectList.end());

		// ゲームオブジェクトのコライダーを描画
		// コライダーは深度値関係なく描画しないと不便なので、ここで描画する
		for (const auto& drawObject : drawObjectList)
		{
			drawObject->DrawCollider();
		}
	}

#pragma endregion

#pragma region Save,Load

	/// <summary>
	/// シーンの情報をファイルに保存する
	/// </summary>
	void Scene::SaveScene() const
	{
		// ---------------------------------------
		// シーンの情報をJson型に格納する
		// ---------------------------------------

		// 保存するデータ
		Json data;

		// シーンの名前
		data["Scene"] = name;


		// ----------------------------------------------------
		// ゲームオブジェクトの情報をJson型に格納する
		// ----------------------------------------------------

		// ゲームオブジェクト識別番号(文字列)の配列
		// リストの順番を維持するためにvector型にする
		std::vector<std::string> stringIDList;
		stringIDList.reserve(gameObjectList.size());

		// ゲームオブジェクトの情報を格納
		for (const auto& gameObject : gameObjectList)
		{
			gameObject->ToJson(data[gameObject->GetID_String()]);
			stringIDList.push_back(gameObject->GetID_String());
		}

		// ゲームオブジェクト識別番号の配列を格納
		data["ObjectIDList"] = stringIDList;

		// ----------------------------------------------
		// Jsonファイルに情報を保存する
		// ----------------------------------------------

		// 保存先のディレクトリがなければ作成する
		std::filesystem::create_directories(folderName);

		// 保存先のファイル名
		const std::string fileName = GetFileName();

		// 保存
		JsonFile::Save(fileName, data);
	}

	/// <summary>
	/// シーンの情報をファイルから読み込む
	/// </summary>
	void Scene::LoadScene()
	{
		// ---------------------------------------------
		// フォルダが存在しない場合は何もしない
		// ---------------------------------------------

		if (!std::filesystem::exists(folderName))
		{
			return;
		}

		// --------------------------------------------------------
		// 現在生成されているゲームオブジェクトを削除する
		// --------------------------------------------------------

		ClearGameObject();

		// ---------------------------------------
		// ファイルから情報を読み込む
		// ---------------------------------------

		// 情報を格納するJson型
		Json data;

		// シーンの情報を読み込む
		JsonFile::Load(GetFileName(), data);

		// ---------------------------------------------
		// ゲームオブジェクトの情報を読み込む
		// ---------------------------------------------

		// ゲームオブジェクトの識別番号(文字列)
		for (const std::string& objectID_string : data["ObjectIDList"])
		{
			// ゲームオブジェクトを作成
			GameObjectPtr gameObject = CreateGameObject("New Object");

			// 識別番号に対応した情報を読み込む
			gameObject->FromJson(data[objectID_string]);
		}
	}

#pragma endregion

} // namespace PokarinEngine