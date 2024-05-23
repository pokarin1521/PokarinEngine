/**
* @file NodeScript.h
*/
#ifndef NODESCRIPT_H_INCLUDED
#define NODESCRIPT_H_INCLUDED

#include "../UsingNames/UsingNodeEditor.h"

#include <memory>

namespace PokarinEngine
{
	/// <summary>
	/// ノードエディタを管理する機能
	/// </summary>
	namespace NodeScript
	{
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
		void Render();

		/// <summary>
		/// 終了
		/// </summary>
		void Finalize();

		/// <summary>
		/// ノードエディタを開く
		/// </summary>
		/// <param name="nodeEditor"> ノードエディタ </param>
		void OpenNodeEditor(const NodeEditorPtr nodeEditor);

		/// <summary>
		/// ノードエディタを閉じる
		/// </summary>
		/// <param name="nodeEditor"> ノードエディタ </param>
		void CloseNodeEditor(const NodeEditorPtr nodeEditor);
	} 

} // namespace PokarinEngine

#endif // !NODESCRIPT_H_INCLUDED
