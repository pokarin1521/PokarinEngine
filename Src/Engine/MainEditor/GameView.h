/**
* @file GameView.h
*/
#ifndef POKARINENGINE_GAMEVIEW_H_INCLUDED
#define POKARINENGINE_GAMEVIEW_H_INCLUDED

#include "../UsingNames/UsingScene.h"
#include "../UsingNames/UsingFramebufferObject.h"

namespace PokarinEngine
{
	/// <summary>
	/// ゲームビュー(ゲーム画面を描画するウィンドウ)
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

	public: // ---------------------------- 制御 ---------------------------

		/// <summary>
		/// 初期化
		/// </summary>
		void Initialize();

		/// <summary>
		/// 更新
		/// </summary>
		void Update();

		/// <summary>
		/// 描画
		/// </summary>
		/// <param name="[in] currentScene"> 現在のシーン </param>
		void Render(const ScenePtr& currentScene);

	private: // -------------------------- 描画用 --------------------------

		// 描画用FBO
		FramebufferObjectPtr fbo;
	};

} // namespace PokarinEngine

#endif // !POKARINENGINE_GAMEVIEW_H_INCLUDED
