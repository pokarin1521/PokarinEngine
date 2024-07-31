/**
* @file GameView.h
*/
#ifndef POKARINENGINE_GAMEVIEW_H_INCLUDED
#define POKARINENGINE_GAMEVIEW_H_INCLUDED

#include "RenderView.h"

#include "../Color.h"

namespace PokarinEngine
{
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
		void Update();

	private:

		// 背景色
		Color backGround = { 0.3f, 0.3f, 0.3f, 1.0f };
	};

} // namespace PokarinEngine

#endif // !POKARINENGINE_GAMEVIEW_H_INCLUDED
