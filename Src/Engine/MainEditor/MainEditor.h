/**
* @file MainEditor.h
*/
#ifndef EDITOR_H_INCLUDED
#define EDITOR_H_INCLUDED

#include "ImGui/imgui.h"

#include "Hierarchy.h"
#include "Inspector.h"
#include "SceneView.h"
#include "GameView.h"

#include "../UsingNames/UsingMesh.h"

#include <memory>
#include <vector>

namespace PokarinEngine
{
	// -------------------
	// 前方宣言
	// -------------------

	class Engine;

	/// <summary>
	/// エディタ管理クラス
	/// </summary>
	class MainEditor
	{
	public: // --------------- コンストラクタ・デストラクタ -----------------

		MainEditor() = default;
		~MainEditor() = default;

	public: // ------------------------ 禁止事項 ----------------------------

		// コピーコンストラクタの禁止
		MainEditor(const MainEditor&) = delete;

		// 代入の禁止
		MainEditor& operator=(const MainEditor&) = delete;

	public: // -------------------------- 制御 ------------------------------

		/// <summary>
		/// 初期化
		/// </summary>
		/// <param name="engine"> エンジンクラスの参照 </param>
		void Initialize(Engine& engine);

		/// <summary>
		/// 更新
		/// </summary>
		void Update();

		/// <summary>
		/// 描画
		/// </summary>
		void Render();

		/// <summary>
		/// 終了
		/// </summary>
		void Finalize();

	public: // --------------------- ビューの取得(お試し) -----------------------

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

	private: // ------------------------ ImGui設定管理 --------------------------

		/// <summary>
		/// ImGuiの色設定を開始する
		/// </summary>
		/// <param name="target"> 設定対象 </param>
		/// <param name="styleColor"> 色 </param>
		void PushStyleColor(ImGuiCol target, const ImVec4& styleColor)
		{
			// 色設定を開始
			ImGui::PushStyleColor(target, styleColor);

			// 設定数をカウント
			pushColorCount++;
		}

		/// <summary>
		/// ImGuiの色設定を終了する
		/// </summary>
		void PopStyleColor()
		{
			// 色設定を終了
			ImGui::PopStyleColor(pushColorCount);

			// 設定数をリセット
			pushColorCount = 0;
		}

	private: // -------------------------- バージョン ---------------------------

		// GLSLのバージョン
		const char* glslVersion = "#version 450";

	private: // --------------------------- 描画用 -------------------------------

		// 灰色
		const ImVec4 gray = { 0.2f, 0.2f, 0.2f, 1.0f };

		// 黒色
		const ImVec4 black = { 0, 0, 0, 1.0f };

	private: // ------------------------- 描画用ビュー ---------------------------

		// シーンビュー
		SceneView sceneView;

		// ゲームビュー
		GameView gameView;

	private: // -------------------------- ウィンドウ ----------------------------

		// ヒエラルキー
		Hierarchy hierarchy;

		// インスペクター
		Inspector inspector;

	private: // ---------------------------- ImGui -------------------------------

		// ImGui用コンテキスト
		ImGuiContext* imGuiContext = nullptr;

		// ImGuiの色設定の数
		int pushColorCount = 0;
	};

} // namespace PokarinEngine

#endif // !EDITOR_H_INCLUDED
