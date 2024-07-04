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
	/// ImGuiのフォント設定用
	/// </summary>
	namespace ImGuiFontSetter
	{
		/// <summary>
		/// ImGuiで使うフォントを設定する
		/// </summary>
		/// <param name="[out] io"> 設定対象 </param>
		inline void SetFont(ImGuiIO& io)
		{
			// ----------------------------------------------
			// デフォルトのフォントを削除
			// ----------------------------------------------

			io.Fonts->Clear();

			// ------------------------------------------
			// テキスト用フォントを追加
			// ------------------------------------------

			io.Fonts->AddFontFromFileTTF(ImGuiConfig::File::textFont, ImGuiConfig::fontSize);

			// ------------------------------------------
			// アイコン用フォントを追加
			// ------------------------------------------

			// 追加する範囲
			static const ImWchar icons_ranges[] = { ICON_MIN_FA, ICON_MAX_FA,0 };

			// フォント追加に関する設定
			// フォントをマージするように設定する
			ImFontConfig fontConfig;
			fontConfig.MergeMode = true;

			// アイコン用フォントを追加
			io.Fonts->AddFontFromFileTTF(ImGuiConfig::File::iconFont,
				ImGuiConfig::fontSize, &fontConfig, icons_ranges);
		}
	}
}

#endif // !IMGUIFONTSETTER_H_INCLUDED
