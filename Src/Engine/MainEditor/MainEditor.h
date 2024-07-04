/**
* @file MainEditor.h
*/
#ifndef EDITOR_H_INCLUDED
#define EDITOR_H_INCLUDED

#include "SceneView.h"
#include "GameView.h"
#include "Hierarchy.h"
#include "Inspector.h"

#include <memory>
#include <vector>

namespace PokarinEngine
{
	// -------------------
	// 前方宣言
	// -------------------

	class Engine;
	class SceneView;
	class GameView;

	/// <summary>
	/// メインエディタ管理用
	/// </summary>
	class MainEditor
	{
	public: // --------------- コンストラクタ・デストラクタ ----------------

		MainEditor() = default;
		~MainEditor() = default;

	public: // ---------------------------- 制御 ---------------------------

		/// <summary>
		/// 初期化
		/// </summary>
		/// <param name="[in] e"> エンジンクラスの参照 </param>
		void Initialize(Engine& e);

		/// <summary>
		/// 更新
		/// </summary>
		/// <param name="[out] isPlayGame"> ゲーム再生中ならtrue </param>
		void Update(bool& isPlayGame);

		/// <summary>
		/// 描画
		/// </summary>
		void Render();

		/// <summary>
		/// 終了
		/// </summary>
		void Finalize();

	public: // ------------------------ ビューの取得 -----------------------

		/// <summary>
		/// シーンビューを取得する
		/// </summary>
		/// <returns> シーンビュー </returns>
		const SceneView& GetSceneView()
		{
			return sceneView;
		}

		/// <summary>
		/// ゲームビューを取得する
		/// </summary>
		/// <returns> ゲームビュー </returns>
		const GameView& GetGameView()
		{
			return gameView;
		}

	private: // ------------------------ エディタ用 ------------------------

		/// <summary>
		/// メインメニュー
		/// </summary>
		void MainMenu();

	private: // ------------------------- 色設定用 -------------------------

		/// <summary>
		/// ImGuiの色設定を開始する
		/// </summary>
		/// <param name="[in] style"> 色を設定したい項目 </param>
		/// <param name="[in] styleColor"> 設定する色 </param>
		void PushColor(ImGuiCol style, const Color& styleColor);

		/// <summary>
		/// ImGuiの色設定を終了する
		/// </summary>
		void PopColor();

	private: // ------------------------ ウィンドウ ------------------------

		// シーンビュー
		SceneView sceneView;

		// ゲームビュー
		GameView gameView;

		// ヒエラルキー
		Hierarchy hierarchy;

		// インスペクター
		Inspector inspector;

	private: // ------------------------- バージョン -----------------------

		// GLSLのバージョン
		const char* glslVersion = "#version 450";

	private: // -------------------------- ImGui用 -------------------------

		// ImGui用コンテキスト
		ImGuiContext* imGuiContext = nullptr;

		// ImGuiの色設定の数
		int pushColorCount = 0;

	private: // ------------------------- エンジン用 -----------------------

		// エンジン
		Engine* engine = nullptr;
	};

} // namespace PokarinEngine

#endif // !EDITOR_H_INCLUDED
