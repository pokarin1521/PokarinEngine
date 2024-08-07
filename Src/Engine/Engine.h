/**
/**
* @file Engine.h
*/
#ifndef POKARINENGINE_ENGINE_H_INCLUDED
#define POKARINENGINE_ENGINE_H_INCLUDED

#include "glad/glad.h"

#include "MainEditor/MainEditor.h"

#include "Math/Vector.h"

#include "UsingNames/UsingScene.h"

#include <GLFW/glfw3.h>
#include <string>
#include <vector>
#include <memory>
#include <utility>
#include <functional>
#include <unordered_map>

namespace PokarinEngine
{
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

	public: // ----------------------- シーンの取得 -----------------------

		/// <summary>
		/// 現在のシーンを取得する
		/// </summary>
		/// <returns> 現在のシーン </returns>
		ScenePtr GetCurrentScene()
		{
			return currentScene;
		}

	public: // ------------ エンジン、ゲームオブジェクトの管理 ------------

		/// <summary>
		/// ゲームエンジンを実行する
		/// </summary>
		/// <returns>
		/// <para> 0 : 正常に実行が完了した </para>
		/// <para> 0以外 : エラーが発生した </para>
		/// </returns>
		int Run();

	public: // ----------------------- ゲームの再生 -----------------------

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

	private: // ---------------------- エンジンの制御 ---------------------

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

	private: // -------------------- シーンの識別番号 --------------------

		/// <summary>
		/// シーンの識別番号を作成する
		/// </summary>
		/// <returns> 作成した識別番号 </returns>
		int CreateSceneID();

	private: // ------------------------- シーン --------------------------

		// 現在のシーン
		ScenePtr currentScene;

		// シーン管理用配列
		std::vector<ScenePtr> sceneList;

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
