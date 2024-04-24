/**
* @file Hierarchy.h
*/
#ifndef HIERARCHY_H_INCLUDED
#define HIERARCHY_H_INCLUDED

#include "../UsingNames/UsingGameObject.h"

#include <memory>
#include <vector>

namespace PokarinEngine
{
	// ---------------------
	// 前方宣言
	// ---------------------

	class Engine;

	/// <summary>
	/// ヒエラルキー(シーン内のオブジェクト管理用ウィンドウ)
	/// </summary>
	namespace Hierarchy
	{
		/// <summary>
		/// 初期化
		/// </summary>
		void Initialize(Engine& e);

		/// <summary>
		/// 更新
		/// </summary>
		void Update();

		/// <summary>
		/// 選択中のオブジェクトの要素番号を取得する
		/// </summary>
		/// <returns> ウィンドウ内で選択中のオブジェクトの要素番号 </returns>
		GameObjectPtr GetSelectObject();
	} 

} // namespace PokarinEngine

#endif // !HIERARCHY_H_INCLUDED
