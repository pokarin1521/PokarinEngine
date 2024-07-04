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
		///// <summary>
		///// チェックボックスを表示する
		///// </summary>
		///// <param name="name"> チェックボックスの名前 </param>
		///// <param name="isCheck">  </param>
		//void CheckBox(const std::string& name, bool& isCheck)
		//{
		//	//ImGui::Checkbox(name.c_str(), &isCheck);
		//}

		/// <summary>
		///	Vector3型の情報をドラッグ操作用スライダーで表示する
		/// </summary>
		/// <param name="[in,out] info"> 表示する情報 </param>
		/// <param name="[in] title"> 表示するタイトル </param>
		/// <param name="[in] id_string"> 識別番号の文字列 </param>
		/// <param name="[in] startX"> 表示の開始位置(X座標) </param>
		/// <param name="[in] width"> 表示するスライダーの幅 </param>
		void DragVector3(Vector3& info, const std::string& title, const std::string& id_string,
			const float width, const float startX = 0);
	}
}

#endif // !EDITORINFORSETTER_H_INCLUDED

