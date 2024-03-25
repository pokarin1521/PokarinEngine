/**
* @file Scene.cpp
*/
#include "Scene.h"

#include "Engine.h"
#include "Random.h"
#include "Debug.h"

#include "Settings/ShaderSettings.h"

#include <algorithm>

namespace PokarinEngine
{
#pragma region Create

	/// <summary>
	/// シーン作成用コンストラクタ
	/// </summary>
	/// <param name="e"> エンジンクラスの参照 </param>
	/// <param name="sceneName"> シーン名 </param>
	Scene::Scene(Engine& e, const char* sceneName)
		:engine(&e), name(sceneName)
	{
		// メインカメラ作成
		mainCamera = CreateGameObject("MainCamera");
	}

	/// <summary>
	/// ゲームオブジェクトを作成する
	/// </summary>
	/// <typeparam name="T"> ゲームオブジェクトクラスまたはその派生 </typeparam>
	/// <param name="name"> オブジェクトの名前 </param>
	/// <param name="position"> オブジェクトを配置する座標 </param>
	/// <param name="rotation"> オブジェクトの回転角度 </param>
	/// <param name="staticMeshFile"> スタティックメッシュのファイル名 </param>
	/// <returns> 追加したゲームオブジェクトのポインタ </returns>
	GameObjectPtr Scene::CreateGameObject(const std::string& name,
		const Vec3& position, const Vec3& rotation, const char* staticMeshFile)
	{
		// シーン内のゲームオブジェクト数が最大値に達したら作成しない
		if (gameObjectList.size() >= gameObjectMax)
		{
			LOG_WARNING("ゲームオブジェクト数が最大値に達したため、ゲームオブジェクトを作成出来ません");
			return nullptr;
		}

		// ゲームオブジェクト作成
		GameObjectPtr object = std::make_shared<GameObject>();

		// エンジンを設定
		object->engine = engine;

		// 作成時の名前を種類名として設定
		object->typeName = name;

		// 名前を設定
		object->name = name;

		// オブジェクト種類名を追加
		// 重複している場合は、名前を再設定する
		if (!objectTypeNameList.emplace(object->typeName, SetNumberNameList()).second)
		{
			// 重複しないように名前を設定
			object->name = GetSingleObjectName(object->typeName);

			// 重複しているので、数字付きになっている名前を追加する
			objectTypeNameList[object->typeName].emplace(object->name);
		}

		// 重複しないように識別番号を設定
		object->id = GetSingleObjectID();

		// ゲームオブジェクトの初期化
		object->Initialize();

		// 位置と角度を設定
		object->transform->position = position;
		object->transform->rotation = rotation;

		// スタティックメッシュ
		object->staticMesh = engine->GetStaticMesh(staticMeshFile);

		// 現在のシーンに登録
		AddGameObject(object);

		return object;
	}

	/// <summary>
	/// ゲームオブジェクトを複製する
	/// </summary>
	/// <param name="object"> 複製元のゲームオブジェクト </param>
	/// <returns> 複製したゲームオブジェクト </returns>
	GameObjectPtr Scene::CopyGameObject(GameObjectPtr object)
	{
		// 複製元と同じパラメータのオブジェクトを作成
		// 名前の重複確認が出来るように種類名を使う
		GameObjectPtr copyObject = CreateGameObject(object->typeName,
			object->transform->position, object->transform->rotation,
			object->staticMesh->filename.c_str());

		// 複製したゲームオブジェクトを返す
		return copyObject;
	}

	/// <summary>
	/// ゲームオブジェクトを追加する
	/// </summary>
	/// <param name="object"> 追加するゲームオブジェクト </param>
	void Scene::AddGameObject(GameObjectPtr object)
	{
		// ゲームオブジェクトを追加
		gameObjectList.push_back(object);

		// オブジェクト識別番号を追加
		objectIDList.emplace(object->id);
	}

#pragma endregion

#pragma region Name,ID

	/// ここでしか使わないので、cppのみに書く
	/// <summary>
	/// オブジェクト名の後ろに付いてる数字を取得する
	/// </summary>
	/// <param name="objectName"> オブジェクト名 </param>
	/// <returns> ()の中にある数字 </returns>
	int GetBackNumber(const std::string& objectName)
	{
		/* 数字は()の中にあるので
		()の位置から数字の位置と数を求める */

		// 数字の始め
		// 「(」の次から数字が入るので+1
		size_t first = objectName.find_last_of("(") + 1;

		// 数字の終わり
		// 「)」が終わりの合図なので「)」の位置を求める
		size_t last = objectName.find_last_of(")");

		// ()がない場合、0を返す
		if (first == objectName.npos ||
			last == objectName.npos)
		{
			return 0;
		}

		// 数字の数
		// 「終わりの位置」と「始めの位置」の差から数を求める
		size_t numSize = last - first;

		// 名前の後ろに付いてる数字(文字列)
		std::string backNum_str = objectName.substr(first, numSize);

		// int型に変換して返す
		return std::stoi(backNum_str);
	}

	/// <summary>
	/// ゲームオブジェクトの名前を変更する
	/// </summary>
	/// <param name="object"> 名前を変更するオブジェクト </param>
	/// <param name="afterName"> 変更後の名前 </param>
	void Scene::ChangeObjectName(GameObjectPtr object, const std::string& afterName)
	{
		// 変更前の種類名
		std::string beforName = object->typeName;

		// 名前を変更
		object->name = afterName;

		// 種類名を変更
		object->typeName = afterName;

		// 数字付きのオブジェクト名配列
		auto& setNumberNameList = objectTypeNameList[object->typeName];

		// 数字付きの名前として登録されているならそれを削除
		if (setNumberNameList.size() > 0)
		{
			setNumberNameList.erase(object->name);
		}
		// 登録されていないなら種類名を削除
		else
		{
			objectTypeNameList.erase(object->typeName);
		}

		// 変更後の種類名を追加する
		objectTypeNameList.emplace(afterName, SetNumberNameList());
	}

	/// <summary>
	/// ゲームオブジェクトの名前が重複しないように変更する
	/// </summary>
	/// <param name="objectName"> オブジェクト名 </param>
	/// <returns> 
	/// <para> 重複しないように変更した名前 </para>
	/// <para> (重複していないならそのままの名前) </para>
	/// </returns>
	std::string Scene::GetSingleObjectName(const std::string& typeName) const
	{
		// 新しいオブジェクト名
		std::string newObjectName = typeName;

		// 数字付きの名前の数
		size_t setNumberNameCount = objectTypeNameList.at(typeName).size();

		// 数字付きの名前の数が0なら
		// (1)を付けて返す
		if (setNumberNameCount == 0)
		{
			return newObjectName += " (1)";
		}

		/* set型は自動でソートするので、
		最後の要素から現在使われている最大の数字を取得できる */

		// 最後の数字付きの名前
		std::string lastName = *objectTypeNameList.at(typeName).rbegin();

		// オブジェクト名の後ろに付ける数字
		// 「現在使われている最大の数字 + 1」の数字を付ける
		int backNumber = GetBackNumber(lastName) + 1;

		// 名前の後ろに番号を付ける
		newObjectName += " (" + std::to_string(backNumber) + ")";

		return newObjectName;
	}

	/// <summary>
	/// <para> 他のオブジェクトと重複しない識別番号を取得する </para>
	/// </summary>
	/// <returns> 
	/// <para> 重複しない識別番号(乱数) </para>
	/// </returns>
	int Scene::GetSingleObjectID() const
	{
		// オブジェクト数が最大値まで達したらIDを生成しない
		if (objectIDList.size() >= INT_MAX)
		{
			LOG_WARNING("オブジェクト数が最大値に達したため、IDを生成出来ません");
			return 0;
		}

		// オブジェクト識別番号
		// 負数が入るとヒエラルキーの処理で困るので0以上の乱数
		int objectID = Random::Range(0, INT_MAX);

		// 他のオブジェクトと番号が重複しているなら乱数を取得し直す
		while (objectIDList.find(objectID) != objectIDList.end())
		{
			objectID = Random::Range(0, INT_MAX);
		}

		return objectID;
	}

#pragma endregion

#pragma region Update

	/// <summary>
	/// ゲームオブジェクトの状態を更新する
	/// </summary>
	/// <param name="deltaTime"> 前回の更新からの経過時間(秒) </param>
	void Scene::UpdateGameObject(float deltaTime)
	{
		for (const auto& gameObject : gameObjectList)
		{
			if (!gameObject->IsDestroyed())
			{
				gameObject->Update(deltaTime);
			}
		}

		// ----------------------------------
		// ローカル座標変換行列を計算
		// ----------------------------------

		for (const auto& gameObject : gameObjectList)
		{
			// オブジェクトの座標・回転角度・拡大率
			TransformPtr transform = gameObject->transform;

			// オブジェクトに設定する座標変換行列
			Mat4 transformMatrix = Mat4_Function::GetTransformMatrix(
				transform->scale, transform->rotation, transform->position);

			// オブジェクトに設定する法線変換行列
			Mat3 normalMatrix = Mat3_Function::GetRotationMatrix(transform->rotation);

			// 座標変換行列を設定
			transform->SetTransformMatrix(transformMatrix);

			// 法線変換行列を設定
			transform->SetNormalMatrix(normalMatrix);
		}

		// ------------------------------------
		// ワールド座標変換行列を計算
		// ------------------------------------

		// ワールド座標変換行列
		std::vector<Mat4> worldTransforms(gameObjectList.size());

		// ワールド法線変換行列
		std::vector<Mat3> worldNormals(gameObjectList.size());

		// 全ての親の座標変換行列を掛け合わせる
		for (size_t i = 0; i < gameObjectList.size(); ++i)
		{
			// オブジェクトの座標・回転角度・拡大率
			Transform* transform = gameObjectList[i]->transform.get();

			// 自身の座標変換行列
			Mat4 transformMatrix = transform->GetTransformMatrix();

			// 自身の法線変換行列
			Mat3 normalMatrix = transform->GetNormalMatrix();

			// 親をたどっていく
			for (Transform* parent = transform->GetParent(); parent; parent = parent->GetParent())
			{
				// 親の座標変換行列
				Mat4 parentTransformMatrix = parent->GetTransformMatrix();

				// 親の法線変換行列
				Mat3 parentNormalMatrix = parent->GetNormalMatrix();

				// 親の変換行列を掛け合わせる
				transformMatrix = parentTransformMatrix * transformMatrix;
				normalMatrix = parentNormalMatrix * normalMatrix;
			}

			// 計算したワールド座標変換行列を代入
			worldTransforms[i] = transformMatrix;
			worldNormals[i] = normalMatrix;
		}

		// -----------------------------------
		// ワールド座標変換行列を
		// ゲームオブジェクトに設定
		// -----------------------------------

		for (size_t i = 0; i < gameObjectList.size(); ++i)
		{
			gameObjectList[i]->transform->SetTransformMatrix(worldTransforms[i]);

			gameObjectList[i]->transform->SetNormalMatrix(worldNormals[i]);
		}

	} // UpdateGameObject

#pragma endregion

#pragma region Destroy

/// <summary>
/// シーンから全てのゲームオブジェクトを破棄する
/// </summary>
	void Scene::ClearGameObjectAll()
	{
		// ゲームオブジェクトの削除イベントを実行
		for (auto& gameObject : gameObjectList)
		{
			gameObject->OnDestroy();
		}

		gameObjectList.clear();
		objectTypeNameList.clear();
		objectIDList.clear();
	}

	/// <summary>
	/// ゲームオブジェクトを削除する
	/// </summary>
	/// <param name="object"> 削除するゲームオブジェクト </param>
	void Scene::DestroyObject(GameObjectPtr object)
	{
		// -----------------------------------
		// ゲームオブジェクトを削除
		// -----------------------------------

		object->Destroy();

		// -------------------------------------
		// オブジェクト識別番号を削除
		// -------------------------------------

		objectIDList.erase(object->id);

		// -----------------------------------
		// オブジェクト種類名を削除
		// -----------------------------------

		// 数字付きの名前の配列
		auto& setNumberNameList = objectTypeNameList[object->typeName];

		// 数字付きの名前として登録されているならそれを削除
		if (setNumberNameList.size() > 0)
		{
			setNumberNameList.erase(object->name);
		}
		// 登録されていないなら種類名を削除
		else
		{
			objectTypeNameList.erase(object->typeName);
		}
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
	/// <param name="prog"> シェーダプログラムの管理番号 </param>
	/// <param name="gameObject"> パラメータをコピーするゲームオブジェクト </param>
	void CopyGameObjectParameters(
		const Engine& engine, GLuint prog, const GameObject& gameObject)
	{
		// 座標変換行列
		Mat4 transformMatrix = gameObject.transform->GetTransformMatrix();

		// 法線変換行列
		Mat3 normalMatrix = gameObject.transform->GetNormalMatrix();

		// ------------------------------------
		// パラメータをGPUにコピー
		// ------------------------------------

		// オブジェクトの色
		glProgramUniform4fv(prog,
			LocationNum::color, 1, &gameObject.color.r);

		// 座標変換行列
		glProgramUniformMatrix4fv(
			prog, 0, 1, GL_FALSE, &transformMatrix[0].x);

		// 法線変換行列
		if (prog == engine.GetShaderProgram(Shader::ProgType::Standard))
		{
			glProgramUniformMatrix3fv(
				prog, 1, 1, GL_FALSE, &normalMatrix[0].x);
		}
	}

	/// ここでしか使わないので、cppのみに書く
	/// <summary>
	/// ゲームオブジェクトを描画する
	/// </summary>
	/// <param name="prog"> シェーダプログラムの管理番号 </param>
	/// <param name="begin"> 描画するゲームオブジェクト配列の先頭イテレータ </param>
	/// <param name="end"> 描画するゲームオブジェクト配列の末尾イテレータ </param>
	void DrawGameObject(const Engine& engine, GLuint prog,
		GameObjectList::iterator begin, GameObjectList::iterator end)
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
			CopyGameObjectParameters(engine, prog, *gameObject);

			// ------------- 図形を描画 --------------

			// 固有マテリアルがない
			if (gameObject->materials.empty())
			{
				// 共有マテリアルを使って
				// スタティックメッシュを描画
				Draw(*gameObject->staticMesh, prog, gameObject->staticMesh->materials);
			}
			// 固有マテリアルがある
			else
			{
				// 固有マテリアルを使って
				// スタティックメッシュを描画
				Draw(*gameObject->staticMesh, prog, gameObject->materials);
			}
		}
	}

	/// <summary>
	/// シーン内の全てのゲームオブジェクトを描画する
	/// </summary>
	/// <param name="shaderProgList"> シェーダプログラム配列 </param>
	void Scene::DrawGameObjectAll(const Shader::ProgList& shaderProgList)
	{
		// ------------------------------
		// 描画優先度で並べ替え
		// ------------------------------

		// 描画するゲームオブジェクト配列
		// 本体の順番が変わると困るのでコピー
		GameObjectList drawObjectList = gameObjectList;

		// ゲームオブジェクトを描画順に並べ替える
		// (レンダーキューが小さいものから順に並べる)
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
		GLuint progStandard = shaderProgList.at(Shader::ProgType::Standard);

		// ライティング無しシェーダプログラムの管理番号
		GLuint progUnlit = shaderProgList.at(Shader::ProgType::Unlit);

		// ---------- transparent以前のキューを描画  -----------
		// ---------- 通常のオブジェクト		     -----------

		// テスト用にライティング無しのシェーダを使う
		// 本来は「progStandard」を使う
		// 描画
		DrawGameObject(*engine, progUnlit, drawObjectList.begin(), transparentBegin);

		// ------ transparentからoverlayまでのキューを描画 ------
		// ------ 半透明なオブジェクト					   ------

		// 深度バッファへの書き込みを禁止
		glDepthMask(GL_FALSE);

		// テスト用にライティング無しのシェーダを使う
		// 本来は「progStandard」を使う
		// 描画
		DrawGameObject(*engine, progUnlit, transparentBegin, overlayBegin);

		// 深度バッファへの書き込みを許可
		glDepthMask(GL_TRUE);

		// ---------------- overlay以降のキューを描画 ----------------
		// ---------------- UI,全画面エフェクトなど   ----------------

		// overlay以降は
		// UI系の深度値関係なく描画して欲しいオブジェクトなので、
		// 深度テストを無効化する
		glDisable(GL_DEPTH_TEST);

		// 描画
		DrawGameObject(*engine, progUnlit, overlayBegin, drawObjectList.end());
	}

#pragma endregion

} // namespace PokarinEngine