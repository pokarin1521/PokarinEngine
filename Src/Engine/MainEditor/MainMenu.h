/**
* @file MainMenu.h
*/
#ifndef POKARINENGINE_MAINMENU_H_INCLUDED
#define POKARINENGINE_MAINMENU_H_INCLUDED

namespace PokarinEngine
{
	// -------------------
	// 前方宣言
	// -------------------

	class Scene;

	/// <summary>
	/// メインエディタ用メインメニュー
	/// </summary>
	class MainMenu
	{
	public: // ------------- コンストラクタ・デストラクタ -------------

		MainMenu() = default;
		~MainMenu() = default;

	public: // ------------------------ 禁止事項 ----------------------

		/* エディタ管理がおかしくなるので禁止する */

		// コピーコンストラクタの禁止
		MainMenu(const MainMenu&) = delete;

		// 代入の禁止
		MainMenu& operator=(const MainMenu&) = delete;

	public: // -------------------------- 制御 ------------------------

		/// <summary>
		/// 更新
		/// </summary>
		/// <param name="[in,out] currentScene"> 現在のシーン </param>
		void Update(Scene& currentScene);
	};
}

#endif // !POKARINENGINE_MAINMENU_H_INCLUDED
