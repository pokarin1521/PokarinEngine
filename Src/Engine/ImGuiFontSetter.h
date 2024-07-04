/**
* @file ImGuiFontSetter.h
*/
#ifndef IMGUIFONTSETTER_H_INCLUDED
#define IMGUIFONTSETTER_H_INCLUDED

#include "ImGui/imgui.h"
#include "IconFont/IconFont.h"

#include "Configs/ImGuiConfig.h"

namespace PokarinEngine
{
	/// <summary>
	/// ImGui�̃t�H���g�ݒ�p
	/// </summary>
	namespace ImGuiFontSetter
	{
		/// <summary>
		/// ImGui�Ŏg���t�H���g��ݒ肷��
		/// </summary>
		/// <param name="[out] io"> �ݒ�Ώ� </param>
		inline void SetFont(ImGuiIO& io)
		{
			// ----------------------------------------------
			// �f�t�H���g�̃t�H���g���폜
			// ----------------------------------------------

			io.Fonts->Clear();

			// ------------------------------------------
			// �e�L�X�g�p�t�H���g��ǉ�
			// ------------------------------------------

			io.Fonts->AddFontFromFileTTF(ImGuiConfig::File::textFont, ImGuiConfig::fontSize);

			// ------------------------------------------
			// �A�C�R���p�t�H���g��ǉ�
			// ------------------------------------------

			// �ǉ�����͈�
			static const ImWchar icons_ranges[] = { ICON_MIN_FA, ICON_MAX_FA,0 };

			// �t�H���g�ǉ��Ɋւ���ݒ�
			// �t�H���g���}�[�W����悤�ɐݒ肷��
			ImFontConfig fontConfig;
			fontConfig.MergeMode = true;

			// �A�C�R���p�t�H���g��ǉ�
			io.Fonts->AddFontFromFileTTF(ImGuiConfig::File::iconFont,
				ImGuiConfig::fontSize, &fontConfig, icons_ranges);
		}
	}
}

#endif // !IMGUIFONTSETTER_H_INCLUDED
