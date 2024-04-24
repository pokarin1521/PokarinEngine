/**
* @file Inspector.h
*/
#ifndef INSPECTOR_H_INCLUDED
#define INSPECTOR_H_INCLUDED

#include "ImGui/imgui.h"

#include "../UsingNames/UsingGameObject.h"

#include <string>
#include <memory>

namespace PokarinEngine
{
	/// <summary>
	/// インスペクター(シーン内のオブジェクト制御用ウィンドウ)
	/// </summary>
	namespace Inspector
	{
		/// <summary>
		/// 更新
		/// </summary>
		/// <param name="selectObject"> ヒエラルキーで選択中のオブジェクト </param>
		void Update(GameObjectPtr selectObject);
	}

} // namespace PokarinEngine

#endif // !INSPECTOR_H_INCLUDED

