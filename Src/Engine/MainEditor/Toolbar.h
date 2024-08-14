/**
* @file Toolbar.h
*/
#ifndef POKARINENGINE_TOOLBAR_H_INCLUDED
#define POKARINENGINE_TOOLBAR_H_INCLUDED

namespace PokarinEngine
{
	// ----------------------
	// 前方宣言
	// ----------------------

	class Engine;

	/// <summary>
	/// ツールバー
	/// </summary>
	namespace Toolbar
	{
		/// <summary>
		/// 初期化
		/// </summary>
		void Initialize();

		/// <summary>
		/// 更新
		/// </summary>
		/// <param name="[out] isPlayGame"> ゲームが再生中ならtrue </param>
		void Update(bool& isPlayGame);
	}
}

#endif // !POKARINENGINE_TOOLBAR_H_INCLUDED
