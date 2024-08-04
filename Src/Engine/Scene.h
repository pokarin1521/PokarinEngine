/**
* @file Scene.h
*/
#ifndef POKARINENGINE_SCENE_H_INCLUDED
#define POKARINENGINE_SCENE_H_INCLUDED

#include "Shader/Shader.h"
#include "Configs/MeshConfig.h"

#include "UsingNames/UsingScene.h"
#include "UsingNames/UsingGameObject.h"

#include "GameObject.h"

#include "Components/Camera.h"

#include <string>
#include <memory>
#include <vector>
#include <set>
#include <unordered_set>
#include <unordered_map>

namespace PokarinEngine
{
	// -------------------
	// 前方宣言
	// -------------------

	class Engine;
	class Camera;

	// ------------------------------------
	// 型の別名を定義
	// ------------------------------------

	using CameraPtr = std::shared_ptr<Camera>;

	/// <summary>
	/// シーン管理クラス
	/// </summary>
	class Scene
	{
	public: // --------------- コンストラクタデストラクタ ------------------

		/// <summary>
		/// シーン作成用コンストラクタ
		/// </summary>
		/// <param name="[in] sceneID"> シーン識別番号 </param>
		/// <param name="[in] sceneName"> シーン名 </param>
		Scene(int sceneID, const char* sceneName);

		~Scene() = default;

	public: // ------------------------- 禁止事項 ----------------------------

		// コピーコンストラクタの禁止
		Scene(const Scene&) = delete;

		// 代入の禁止
		Scene& operator=(const Scene&) = delete;

	public: // ------------------- ゲームオブジェクト作成 --------------------

		/// <summary>
		/// ゲームオブジェクトを作成する
		/// </summary>
		/// <typeparam name="T"> ゲームオブジェクトクラスまたはその派生 </typeparam>
		/// <param name="[in] name"> オブジェクトの名前 </param>
		/// <param name="[in] position"> オブジェクトを配置する位置 </param>
		/// <param name="[in] rotation"> オブジェクトの回転角度 </param>
		/// <param name="[in] staticMeshFile"> スタティックメッシュのファイル名 </param>
		/// <returns> 追加したゲームオブジェクトのポインタ </returns>
		GameObjectPtr CreateGameObject(const std::string& name,
			const Vector3& position = Vector3::zero, const Vector3& rotation = Vector3::zero,
			const char* staticMeshFile = "");

		/// <summary>
		/// ゲームオブジェクトをコピーする
		/// </summary>
		/// <param name="[in] copyObject"> コピー元のゲームオブジェクト </param>
		void CopyGameObject(const GameObjectPtr& object);

	public: // ------------------- ゲームオブジェクト制御 -------------------

		/// <summary>
		/// ゲームオブジェクトを削除する
		/// </summary>
		/// <param name="[in] object"> 削除するゲームオブジェクト </param>
		void DestroyObject(GameObjectPtr& object);

		/// <summary>
		/// ゲームエンジンから全てのゲームオブジェクトを破棄する
		/// </summary>
		void ClearGameObject();

		/// <summary>
		/// ゲームオブジェクトの状態を更新する
		/// </summary>
		/// <param name="[in] isPlayGame"> ゲーム再生中ならtrue </param>
		void UpdateGameObject(bool isPlayGame);

		/// <summary>
		/// シーン内の全てのゲームオブジェクトを描画する
		/// </summary>
		void DrawGameObjectAll();

		/// <summary>
		/// 削除するゲームオブジェクトを完全に削除する
		/// </summary>
		void RemoveDestroyedGameObject();

	public: // ------------------------- 情報の取得 --------------------------

		/// <summary>
		/// シーン内の全てのゲームオブジェクトを取得する
		/// </summary>
		/// <returns> ゲームオブジェクト配列 </returns>
		const GameObjectList& GetGameObjectAll() const
		{
			return gameObjectList;
		}

		/// <summary>
		/// メインカメラを取得する
		/// </summary>
		/// <returns> メインカメラ </returns>
		const Camera& GetMainCamera() const
		{
			return *mainCamera;
		}

		/// <summary>
		/// シーン名を取得する
		/// </summary>
		/// <returns> シーンの名前 </returns>
		const char* GetName() const
		{
			return name.c_str();
		}

	public: // ------------------------ カメラの追加 -------------------------

		/// <summary>
		/// カメラを追加する
		/// </summary>
		/// <param name="camera"> 追加するカメラ </param>
		void AddCamera(const CameraPtr& camera)
		{
			// 今はメインカメラを変更する形で
			mainCamera = camera;
		}

	public: // ---------------------------- 保存 -----------------------------

		/// <summary>
		/// シーンの情報をファイルに保存する
		/// </summary>
		void SaveScene() const;

	public: // -------------------------- 読み込み ---------------------------

		/// <summary>
		/// シーンの情報をファイルから読み込む
		/// </summary>
		void LoadScene();

	private: // ----------------------- 型の別名を定義 -----------------------

		/* 検索をメインで使うので、検索が得意なset型を使う */

		// 識別番号は1つずつしかないので重複禁止
		using ObjectIDList = std::unordered_set<int>;

	private: // -------------------------- 識別番号 ----------------------------

		/// <summary>
		/// オブジェクトの識別番号を取得する
		/// </summary>
		/// <returns> 重複しない識別番号 </returns>
		int GetSingleObjectID();

	private: // ------------------------- ファイル名 ---------------------------

		/// <summary>
		/// 保存先のファイル名を取得する
		/// </summary>
		/// <returns> 保存先のファイル名 </returns>
		std::string GetFileName() const
		{
			return folderName + name + ".json";
		}

	private: // ---------------------- ゲームオブジェクト ----------------------

		// ゲームオブジェクト管理用配列
		GameObjectList gameObjectList;

		// オブジェクト識別番号の管理用配列
		ObjectIDList objectIDList;

		// ゲームオブジェクトの最大数
		const size_t gameObjectMax = 1000000;

	private: // ------------------------- メインカメラ ------------------------

		// メインカメラ
		CameraPtr mainCamera;

	private: // ----------------------------- 情報 ----------------------------

		// シーンの名前
		std::string name = "";

		// 識別番号
		int id = 0;

	private: // ---------------------------- 保存用 ---------------------------

		// 保存先のフォルダ
		const std::string folderName = "My project/Assets/Scenes/";
	};

} // namespace PokarinEngine

#endif // !POKARINENGINE_SCENE_H_INCLUDED
