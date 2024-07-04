/**
* @file ImGuiConfig.h
*/
#ifndef IMGUICONFIG_H_INCLUDED
#define IMGUICONFIG_H_INCLUDED

#include "ImGui/imgui.h"

namespace PokarinEngine
{
	/// <summary>
	/// ImGuiの設定に関する情報
	/// </summary>
	namespace ImGuiConfig
	{
		/// <summary>
		/// ファイル名
		/// </summary>
		namespace File
		{
			// ImGui情報の保存先ファイル
			inline constexpr const char* setting = "Settings/imgui.ini";

			// テキスト用フォントのファイル
			inline constexpr const char* textFont = "Res/Fonts/arial.ttf";

			// アイコン用フォントのファイル
			inline constexpr const char* iconFont = "Res/Fonts/fa-solid-900.ttf";
		}

		// ImGuiのフォントサイズ
		inline constexpr float fontSize = 20.0f;
	}
}

#endif // !IMGUICONFIG_H_INCLUDED
