/**
/**
* @file Engine.h
*/
#ifndef POKARINENGINE_ENGINE_H_INCLUDED
#define POKARINENGINE_ENGINE_H_INCLUDED

#include "glad/glad.h"

#include "MainEditor/MainEditor.h"

#include "Scene.h"
#include "GameObject.h"

#include "Math/Vector.h"

#include "Shader/Shader.h"

#include "Configs/MeshConfig.h"

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

	public: // ------------------------ シーン作成 ------------------------

		/// <summary>
		/// シーンを作成する
		/// </summary>
		/// <param name="[in] name"> シーン名 </param>
		/// <returns> 作成したシーンのポインタ </returns>
		ScenePtr CreateScene(const char* name);

	public: // ------------------------ シーン取得 ------------------------

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
		int Run();

	public: // ------------------------- ゲームの再生 -------------------------

		/// <summary>
		/// 作成中のゲームが再生中か取得する
		/// </summary>
		/// <returns>
		/// <para> true : ゲームが再生中 </para>
		/// <para> false : ゲームが再生されていない </para>
		/// </returns>
		bool IsPlayGame()
		{
			return isPlayGame;
		}

	private: // ------------------------ エンジンの制御 -----------------------

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

	private: // ---------------------- シーンの識別番号 -----------------------

		/// <summary>
		/// シーンの識別番号を作成する
		/// </summary>
		/// <returns> 作成した識別番号 </returns>
		int CreateSceneID();

	private: // ------------------------- 描画用ビュー ------------------------

		/// <summary>
		/// 描画用ビューにカメラからの描画情報を保持させる
		/// </summary>
		/// <param name="[in] camera"> 使用するカメラ </param>
		/// <param name="[in] renderView"> 描画情報を保持する描画用ビュー </param>
		void DrawRenderView(const Transform& camera, const RenderView& renderView);


	private: // -------------------- スカイスフィアの描画 ---------------------

		/// <summary>
		/// スカイスフィアを描画する
		/// </summary>
		void DrawSkySphere(const Transform& camera);

	private: // ------------------------- 図形データ --------------------------

		// 図形データ管理用バッファ
		MeshBufferPtr meshBuffer;

	private: // ----------------------- スカイスフィア ------------------------

		// スカイスフィア用モデル
		StaticMeshPtr skySphere;

	private: // --------------------------- シーン ----------------------------

		// 現在のシーン
		ScenePtr currentScene;

		// シーン管理用配列
		SceneList sceneList;

		// シーン識別番号の管理用配列
		std::unordered_set<int> sceneIDList;

		// シーンの最大数
		const int sceneMax = 1000;

	private: // ------------------------ メインエディタ -----------------------

		// メインエディタ
		MainEditor mainEditor;

	private: // ----------------------- ゲーム再生確認用 ----------------------

		// 作成中のゲームが再生中ならtrue
		bool isPlayGame = false;
	};

} // namespace PokarinEngine

#endif // !POKARINENGINE_ENGINE_H_INCLUDED
