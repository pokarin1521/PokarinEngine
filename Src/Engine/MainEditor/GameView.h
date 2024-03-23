/**
* @file GameView.h
*/
#ifndef GAMEVIEW_H_INCLUDED
#define GAMEVIEW_H_INCLUDED

#include "RenderView.h"

#include "../Color.h"

namespace PokarinEngine
{
	/// <summary>
	/// ゲームビュー(ゲーム画面を表示するビュー)
	/// </summary>
	class GameView : public RenderView
	{
	public: // --------------- コンストラクタ・デストラクタ ----------------

		GameView() = default;
		~GameView() = default;

	public:

		/// <summary>
		/// 更新
		/// </summary>
		void Update() override;

	private:

		// 背景色
		Color backGround = { 0.3f, 0.3f, 0.3f, 1.0f };
	};

} // namespace PokarinEngine

#endif // !GAMEVIEW_H_INCLUDED
