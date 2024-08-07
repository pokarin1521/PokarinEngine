/**
* @file GameView.h
*/
#ifndef POKARINENGINE_GAMEVIEW_H_INCLUDED
#define POKARINENGINE_GAMEVIEW_H_INCLUDED

#include "../UsingNames/UsingScene.h"
#include "../UsingNames/UsingFramebufferObject.h"

namespace PokarinEngine
{
	// -------------------
	// 前方宣言
	// -------------------

	class Scene;

	/// <summary>
	/// ゲームビュー(ゲーム画面を表示するビュー)
	/// </summary>
	class GameView
	{
	public: // -------------- コンストラクタ・デストラクタ ----------------

		GameView() = default;
		~GameView() = default;

	public: // -------------------------- 禁止事項 -------------------------

		/* エディタ管理がおかしくなるので禁止する */

		// コピーコンストラクタの禁止
		GameView(const GameView&) = delete;

		// 代入の禁止
		GameView& operator=(const GameView&) = delete;

	public: // ---------------------------- 更新 ---------------------------

		/// <summary>
		/// 更新
		/// </summary>
		/// <param name="currentScene"> 現在のシーン </param>
		void Update(const ScenePtr& currentScene);
	};

} // namespace PokarinEngine

#endif // !POKARINENGINE_GAMEVIEW_H_INCLUDED
