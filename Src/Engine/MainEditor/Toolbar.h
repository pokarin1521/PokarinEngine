/**
* @file Toolbar.h
*/
#ifndef TOOLBAR_H_INCLUDED
#define TOOLBAR_H_INCLUDED

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
		/// <param name="[in] engine"> エンジンクラス </param>
		void Initialize(Engine& engine);

		/// <summary>
		/// 更新
		/// </summary>
		/// <param name="[out] isPlayGame"> ゲームが再生中ならtrue </param>
		void Update(bool& isPlayGame);
	}
}

#endif // !TOOLBAR_H_INCLUDED
