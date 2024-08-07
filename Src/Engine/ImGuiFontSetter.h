/**
* @file ImGuiFontSetter.h
*/
#ifndef POKARINENGINE_IMGUIFONTSETTER_H_INCLUDED
#define POKARINENGINE_IMGUIFONTSETTER_H_INCLUDED

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
			// デフォルトのフォントを削除する
			// ----------------------------------------------

			io.Fonts->Clear();

			// ------------------------------------------
			// テキスト用フォントを追加する
			// ------------------------------------------

			// フォントサイズ
			static const float fontSize = 20.0f;

			// テキスト用フォントのファイル名
			static const char* textFontFile = "Res/Fonts/arial.ttf";

			// テキスト用フォントを追加
			io.Fonts->AddFontFromFileTTF(textFontFile, fontSize);

			// ------------------------------------------
			// アイコン用フォントを追加する
			// ------------------------------------------

			// 追加する範囲
			static const ImWchar icons_ranges[] = { ICON_MIN_FA, ICON_MAX_FA,0 };

			// フォント追加に関する設定
			// フォントをマージするように設定する
			ImFontConfig fontConfig;
			fontConfig.MergeMode = true;

			// アイコン用フォントのファイル名
			static const char* iconFontFile = "Res/Fonts/fa-solid-900.ttf";

			// アイコン用フォントを追加
			io.Fonts->AddFontFromFileTTF(iconFontFile, fontSize, &fontConfig, icons_ranges);
		}
	}
}

#endif // !POKARINENGINE_IMGUIFONTSETTER_H_INCLUDED
