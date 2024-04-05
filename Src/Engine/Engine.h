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
#include "Math/Vector.h"

#include "Shader/Shader.h"

#include "MainEditor/MainEditor.h"
#include "NodeScript/NodeScript.h"

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

	public: // ------------------------------ 光線 -------------------------------

		/// <summary>
		/// 光線の交差判定結果
		/// </summary>
		struct RaycastHit
		{
			// 最初に光線と交差したコライダー
			ColliderPtr collider;

			// 最初の交点の座標
			Vector3 point;

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
		Collision::Ray GetRayFromMousePosition() const;

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
		bool Raycast(const Collision::Ray& ray, RaycastHit& hitInfo,
			const RaycastPredicate& pred) const;

	private: // ------------------------- コライダー --------------------------

		// ワールド座標系のコライダーを表す構造体
		struct WorldCollider
		{
			/// <summary>
			/// ワールドコライダーの座標を変更する
			/// </summary>
			/// <param name="v"> 移動量 </param>
			void AddPosition(const Vector3& v)
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
			GameObject& gameObject, const Vector3& penetration);

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

		// シーン配列
		SceneList scenes;

	private: // --------------------------- 時間 ----------------------------

		// 前回更新時の時刻
		double previousTime = 0;

		// 前回更新からの経過時間
		float deltaTime = 0;

	private: // ------------------------- エディタ --------------------------

		// メインエディタ制御用
		MainEditor mainEditor;
	};

} // namespace PokarinEngine

#endif // !ENGINE_H_INCLUDED
