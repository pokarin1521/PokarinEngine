/**
* @file GameView.h
*/
#ifndef POKARINENGINE_GAMEVIEW_H_INCLUDED
#define POKARINENGINE_GAMEVIEW_H_INCLUDED

#include "../Color.h"

#include "../UsingNames/UsingScene.h"

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
	public: // --------------- コンストラクタ・デストラクタ ----------------

		GameView() = default;
		~GameView() = default;

	public:

		/// <summary>
		/// 更新
		/// </summary>
		/// <param name="currentScene"> 現在のシーン </param>
		void Update(const ScenePtr& currentScene);

	private:

		// 背景色
		Color backGround = { 0.3f, 0.3f, 0.3f, 1.0f };
	};

} // namespace PokarinEngine

#endif // !POKARINENGINE_GAMEVIEW_H_INCLUDED
