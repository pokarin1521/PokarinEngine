/**
* @file MainEditor.h
*/
#ifndef EDITOR_H_INCLUDED
#define EDITOR_H_INCLUDED

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
	namespace MainEditor
	{
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

		/// <summary>
		/// シーンビューを取得する
		/// </summary>
		/// <returns> シーンビュー </returns>
		const SceneView& GetSceneView();

		/// <summary>
		/// ゲームビューを取得する
		/// </summary>
		/// <returns> ゲームビュー </returns>
		const GameView& GetGameView();
	}

} // namespace PokarinEngine

#endif // !EDITOR_H_INCLUDED
