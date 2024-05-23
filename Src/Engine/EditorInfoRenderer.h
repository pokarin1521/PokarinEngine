/**
* @file EditorInforSetter.h
*/
#ifndef EDITORINFORSETTER_H_INCLUDED
#define EDITORINFORSETTER_H_INCLUDED

#include "Math/Vector.h"

#include <string>

namespace PokarinEngine
{
	/// <summary>
	/// エディタに情報を表示するための機能
	/// </summary>
	namespace EditorInfoRenderer
	{
		/// <summary>
		///	Vector3型の情報をドラッグ操作用スライダーで表示する
		/// </summary>
		/// <param name="info"> 表示する情報 </param>
		/// <param name="width"> 表示するスライダーの幅 </param>
		/// <param name="infoName"> 表示する情報の名前 </param>
		/// <param name="startX"> 表示の開始位置(X座標) </param>
		void DragVector3(Vector3& info, const float width, const std::string& infoName, const float startX = 0);
	}
}

#endif // !EDITORINFORSETTER_H_INCLUDED

