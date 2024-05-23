/**
* @file ImGuiConfig.h
*/
#ifndef IMGUICONFIG_H_INCLUDED
#define IMGUICONFIG_H_INCLUDED

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

			// ImGuiで使用するフォントのファイル
			inline constexpr const char* font = "Res/Fonts/arial.ttf";
		}

		// ImGuiのフォントサイズ
		inline constexpr float fontSize = 20.0f;
	}
}

#endif // !IMGUICONFIG_H_INCLUDED
