/**
* @file ImGuiConfig.h
*/
#ifndef POKARINENGINE_IMGUICONFIG_H_INCLUDED
#define POKARINENGINE_IMGUICONFIG_H_INCLUDED

#include "ImGui/imgui.h"

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

			// �e�L�X�g�p�t�H���g�̃t�@�C��
			inline constexpr const char* textFont = "Res/Fonts/arial.ttf";

			// �A�C�R���p�t�H���g�̃t�@�C��
			inline constexpr const char* iconFont = "Res/Fonts/fa-solid-900.ttf";
		}

		// ImGui�̃t�H���g�T�C�Y
		inline constexpr float fontSize = 20.0f;
	}
}

#endif // !POKARINENGINE_IMGUICONFIG_H_INCLUDED
