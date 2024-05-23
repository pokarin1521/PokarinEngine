/**
* @file ImGuiConfig.h
*/
#ifndef IMGUICONFIG_H_INCLUDED
#define IMGUICONFIG_H_INCLUDED

namespace PokarinEngine
{
	/// <summary>
	/// ImGui�̐ݒ�Ɋւ�����
	/// </summary>
	namespace ImGuiConfig
	{
		/// <summary>
		/// �t�@�C����
		/// </summary>
		namespace File
		{
			// ImGui���̕ۑ���t�@�C��
			inline constexpr const char* setting = "Settings/imgui.ini";

			// ImGui�Ŏg�p����t�H���g�̃t�@�C��
			inline constexpr const char* font = "Res/Fonts/arial.ttf";
		}

		// ImGui�̃t�H���g�T�C�Y
		inline constexpr float fontSize = 20.0f;
	}
}

#endif // !IMGUICONFIG_H_INCLUDED
