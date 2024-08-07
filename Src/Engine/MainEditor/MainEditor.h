/**
* @file MainEditor.h
*/
#ifndef POKARINENGINE_EDITOR_H_INCLUDED
#define POKARINENGINE_EDITOR_H_INCLUDED

#include "SceneView.h"
#include "GameView.h"
#include "Hierarchy.h"
#include "Inspector.h"

#include <memory>
#include <vector>

namespace PokarinEngine
{
	/// <summary>
	/// メインエディタ管理用クラス
	/// </summary>
	class MainEditor
	{
	public: // --------------- コンストラクタ・デストラクタ ----------------

		MainEditor() = default;
		~MainEditor() = default;

	public: // -------------------------- 禁止事項 -------------------------

		/* ImGuiコンテキストの管理がおかしくなるので、禁止する */

		// コピーコンストラクタの禁止
		MainEditor(const MainEditor&) = delete;

		// 代入の禁止
		MainEditor& operator=(const MainEditor&) = delete;

	public: // ---------------------------- 制御 ---------------------------

		/// <summary>
		/// 初期化
		/// </summary>
		void Initialize();

		/// <summary>
		/// 更新
		/// </summary>
		/// <param name="[in] currentScene"> 現在のシーン </param>
		/// <param name="[out] isPlayGame"> ゲーム再生中ならtrue </param>
		void Update(const ScenePtr& currentScene, bool& isPlayGame);

		/// <summary>
		/// 描画
		/// </summary>
		void Render();

		/// <summary>
		/// 終了
		/// </summary>
		void Finalize();

	private: // ------------------------ エディタ用 ------------------------

		/// <summary>
		/// メインメニュー
		/// </summary>
		/// <param name="[in] currentScene"> 現在のシーン </param>
		void MainMenu(const ScenePtr& currentScene);

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

	private: // ------------------------- ウィンドウ -----------------------

		SceneView sceneView;
		GameView gameView;
		Hierarchy hierarchy;
		Inspector inspector;

	private: // ------------------------- バージョン -----------------------

		// GLSLのバージョン
		const char* glslVersion = "#version 450";

	private: // -------------------------- ImGui用 -------------------------

		// ImGui用コンテキスト
		ImGuiContext* imGuiContext = nullptr;

		// ImGuiの色設定の数
		int pushColorCount = 0;
	};

} // namespace PokarinEngine

#endif // !POKARINENGINE_EDITOR_H_INCLUDED
