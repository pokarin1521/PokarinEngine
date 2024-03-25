/**
/**
* @file Engine.h
*/
#ifndef ENGINE_H_INCLUDED
#define ENGINE_H_INCLUDED

#include "glad/glad.h"

#include "Scene.h"
#include "GameObject.h"
#include "Mesh.h"
#include "VecMath.h"

#include "Shader/Shader.h"

#include "MainEditor/MainEditor.h"

#include "Settings/MeshSettings.h"

#include <GLFW/glfw3.h>
#include <string>
#include <vector>
#include <memory>
#include <utility>
#include <functional>
#include <unordered_map>

namespace PokarinEngine
{
	// -------------------------
	// 前方宣言
	// -------------------------

	class RenderView;

	/// <summary>
	/// ゲームエンジン
	/// </summary>
	class Engine
	{
	public: // --------------- コンストラクタ・デストラクタ ---------------

		Engine() = default;
		~Engine() = default;

	public: // -------------------- UIオブジェクト作成 --------------------

		/// <summary>
		/// UIオブジェクトを作成する
		/// </summary>
		/// <typeparam name="T"> 
		/// ゲームオブジェクトに割り当てるUILayoutまたはその派生
		/// </typeparam>
		/// <param name="filename"> UIオブジェクトに表示する画像 </param>
		/// <param name="position"> UIオブジェクトの座標 </param>
		/// <param name="scale"> UIオブジェクトの大きさ </param>
		/// <returns> 作成したUIオブジェクト </returns>
		template<typename T>
		std::pair<GameObjectPtr, std::shared_ptr<T>> CreateUIObject(
			const char* filename, const Vec2& position, float scale)
		{
			// テキストオブジェクトを作成
			auto object = currentScene->CreateGameObject(
				filename, { position.x, position.y, 0 });

			// 描画順を設定
			object->renderQueue = RenderQueue::overlay;

			// UI用スタティックメッシュを設定
			object->staticMesh = GetStaticMesh(StaticMeshFile_OBJ::ui);

			// テクスチャ
			auto texBaseColor = GetTexture(filename);

			// 固有マテリアルを作成し、テクスチャを差し替える
			object->materials = CloneMaterialList(object->staticMesh);
			object->materials[0]->texBaseColor = texBaseColor;

			// 画像のアスペクト比に応じて拡大率を調整
			const float aspectRatio = texBaseColor->GetAspectRatio();
			object->transform->scale = { scale * aspectRatio, scale, 1 };

			// コンポーネントを追加
			auto component = object->AddComponent<T>();

			// オブジェクトとコンポーネントを返す
			return { object, component };
		}

	public: // ---------------------- シーン作成 ------------------------

		/// <summary>
		/// シーンを作成する
		/// </summary>
		/// <param name="name"> シーン名 </param>
		/// <returns> 作成したシーンのポインタ </returns>
		ScenePtr CreateScene(const char* name);

	public: // ---------------------- シーン取得 ------------------------

		/// <summary>
		/// 現在のシーンを取得する
		/// </summary>
		/// <returns> 現在のシーンの参照 </returns>
		Scene& GetCurrentScene()
		{
			return *currentScene;
		}

		/// <summary>
		/// 現在のシーンを取得する(読み取り専用)
		/// </summary>
		/// <returns> 現在のシーンの参照(読み取り専用) </returns>
		const Scene& GetCurrentScene() const
		{
			return *currentScene;
		}

	public: // -------------- エンジン、ゲームオブジェクトの管理 ----------------

		/// <summary>
		/// ゲームエンジンを実行する
		/// </summary>
		/// <returns>
		/// <para> 0 : 正常に実行が完了した </para>
		/// <para> 0以外 : エラーが発生した </para>
		/// </returns>
		bool Run();

	public: // ---------------------- スタティックメッシュ ----------------------

		/// <summary>
		/// スタティックメッシュを取得する
		/// </summary>
		/// <param name="name"> ファイル名 </param>
		/// <returns> ファイル名前が一致するスタティックメッシュ </returns>
		StaticMeshPtr GetStaticMesh(const char* filename)
		{
			return meshBuffer->GetStaticMesh(filename);
		}

		GLuint GetVAO()
		{
			return *meshBuffer->GetVAO();
		}

	public: // -------------------------- テクスチャ -----------------------------

		/// <summary>
		/// <para> テクスチャを取得する </para>
		/// <para> 作成する場合、サイズを指定する </para>
		/// </summary>
		/// <param name="name"> テクスチャファイル名 </param>
		///	<param name="width"> 作成する場合、テクスチャ幅を指定する </param>
		///	<param name="height"> 作成する場合、テクスチャの高さを指定する </param>
		/// <returns> 名前がnameと一致するテクスチャ </returns>
		TexturePtr GetTexture(const char* name);

		/// <summary>
		/// テクスチャを取得する
		/// </summary>
		/// <param name="width"> 幅 </param>
		/// <param name="height"> 高さ </param>
		/// <returns> 指定した大きさのテクスチャ </returns>
		TexturePtr GetTexture(GLsizei width, GLsizei height);

	public: // -------------------------- 視野角の設定 ---------------------------

		/// <summary>
		/// 垂直視野角を設定する
		/// </summary>
		/// <param name="fovY"> 設定する垂直視野角(度数法) </param>
		void SetFovY(float fovY);

	public: // -------------------------- 視野角の取得 ---------------------------

		/// <summary>
		/// 垂直視野角を取得
		/// </summary>
		/// <returns> 垂直視野角(度数法) </returns>
		float GetFovY() const { return degFovY; }

		/// <summary>
		/// 視野角による拡大率を取得
		/// </summary>
		/// <returns> 視野角による拡大率の逆数 </returns>
		float GetFovScale() const { return fovScale; }

	public: // ------------------------------ 光線 -------------------------------

		/// <summary>
		/// 光線の交差判定結果
		/// </summary>
		struct RaycastHit
		{
			// 最初に光線と交差したコライダー
			ColliderPtr collider;

			// 最初の交点の座標
			Vec3 point;

			// 最初の交点までの距離
			float distance;
		};

		// 交差判定の対象になるかどうか調べる述語型
		// 戻り値は、交差判定の対象になるかどうか
		using RaycastPredicate = std::function<bool(
			const ColliderPtr& collider, float distance)>;

		/// <summary>
		/// マウス座標から発射される光線を取得する
		/// </summary>
		/// <returns> マウス座標から発射される光線 </returns>
		Ray GetRayFromMousePosition() const;

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
		bool Raycast(const Ray& ray, RaycastHit& hitInfo,
			const RaycastPredicate& pred) const;

	public: // --------------------- シェーダ番号の取得 -----------------------

		/// <summary>
		/// シェーダプログラムの管理番号を取得する
		/// </summary>
		/// <param name="progNum"> 取得するシェーダの種類 </param>
		/// <returns> 指定したシェーダプログラムの管理番号 </returns>
		GLuint GetShaderProgram(Shader::ProgType progNum) const
		{
			return shaderProgList.at(progNum);
		}

	private: // ------------------------- コライダー --------------------------

		// ワールド座標系のコライダーを表す構造体
		struct WorldCollider
		{
			/// <summary>
			/// ワールドコライダーの座標を変更する
			/// </summary>
			/// <param name="v"> 移動量 </param>
			void AddPosition(const Vec3& v)
			{
				// コライダーの持ち主の座標を変更
				origin->GetOwner().transform->position += v;

				// コライダーの座標を変更
				world->AddPosition(v);
			}

			// オリジナルのコライダー
			ColliderPtr origin;

			// コライダー
			ColliderPtr world;
		};

		// ワールド座標系コライダーの配列
		using WorldColliderList = std::vector<WorldCollider>;

		/// <summary>
		/// 貫通ベクトルをゲームオブジェクトに反映する
		/// </summary>
		/// <param name="worldColliders"> ワールド座標系のコライダー配列 </param>
		/// <param name="gameObject"> ゲームオブジェクト </param>
		/// <param name="penetration"> 貫通ベクトル </param>
		void ApplyPenetration(WorldColliderList* worldColliders,
			GameObject& gameObject, const Vec3& penetration);

	private: // ---------------------- エンジンの制御 -------------------------

		/// <summary>
		/// ゲームエンジンを初期化する
		/// </summary>
		/// <returns>
		/// <para> 0 : 正常に初期化された </para>
		/// <para> 0以外 : エラーが発生した </para>
		/// </returns>
		bool Initialize();

		/// <summary>
		/// ゲームエンジンの状態を更新する
		/// </summary>
		void Update();

		/// <summary>
		/// ゲームエンジンの状態を描画する
		/// </summary>
		void Render();

	private: // ---------------- ゲームオブジェクトの衝突 ------------------

		/// <summary>
		/// ゲームオブジェクトの衝突処理
		/// </summary>
		void HandleGameObjectCollision();

		/// <summary>
		/// コライダー単位の衝突判定
		/// </summary>
		/// <param name="colliderA"> 
		/// 判定対象のワールドコライダー配列(衝突する側) </param>
		/// <param name="colliderB"> 
		/// 判定対象のワールドコライダー配列(衝突される側) </param>
		void HandleWorldColliderCollision(
			WorldColliderList* collidersA, WorldColliderList* collidersB);

	private: // ---------------------- 描画用ビュー ---------------------

		/// <summary>
		/// 描画用ビューにカメラからの描画情報を保持させる
		/// </summary>
		/// <param name="camera"> 使用するカメラ </param>
		/// <param name="renderView"> 描画情報を保持する描画用ビュー </param>
		void DrawRenderView(const Transform& camera, const RenderView& view);

	private: // ------------------------ 視野角 --------------------------

		// 垂直視野角(度数法)
		float degFovY = 60;

		// 垂直視野角(弧度法)
		float radFovY = degFovY * pi / 180;

		// 視野角による拡大率の逆数
		// (視野角による拡大率は常にこの形で使うので、
		//  あらかじめ逆数にしておく)
		float fovScale = 1 / tan(radFovY / 2);

	private: // ------------------ シェーダプログラム --------------------

		// シェーダプログラムの管理番号配列
		// <シェーダプログラムの種類, シェーダプログラムの管理番号>
		Shader::ProgList shaderProgList;

	private: // --------------------- 図形データ -------------------------

		// 図形データ管理オブジェクト
		MeshBufferPtr meshBuffer;

	private: // --------------------- キャッシュ -------------------------

		// テクスチャ用キャッシュ
		// <ファイル名, テクスチャのポインタ>
		std::unordered_map<std::string, TexturePtr> textureCache;

	private: // ----------------------- シーン ---------------------------

		// 現在のシーン
		ScenePtr currentScene;

		// 次のシーン(待機中のシーン)
		ScenePtr nextScene;

		// シーン配列
		SceneList scenes;

	private: // --------------------------- 時間 ----------------------------

		// 前回更新時の時刻
		double previousTime = 0;

		// 前回更新からの経過時間
		float deltaTime = 0;

	private: // ------------------------- エディタ --------------------------

		// エディタ制御用
		MainEditor editor;
	};

} // namespace PokarinEngine

#endif // !ENGINE_H_INCLUDED
