/**
* @file ImGuiHelper.h
*/
#ifndef POKARINENGINE_IMGUIHELPER_H_INCLUDED
#define POKARINENGINE_IMGUIHELPER_H_INCLUDED

#include "ImGui/imgui.h"

#include "Color.h"
#include "Window.h"

namespace PokarinEngine
{
	namespace ImGuiHelper
	{
		/// <summary>
		/// ImGuiの初期化
		/// </summary>
		/// <param name="[in] windowID"> 使用するウィンドウの識別番号 </param>
		void Initialize(WindowID windowID);

		/// <summary>
		/// ImGuiの更新
		/// </summary>
		/// <param name="[in] context"> 使用するImGuiコンテキスト </param>
		void Update(ImGuiContext* context);

		/// <summary>
		/// ImGuiの描画
		/// </summary>
		/// <param name="[in] context"> 使用するImGuiコンテキスト </param>
		void Render(ImGuiContext* context);

		/// <summary>
		/// ImGuiの終了とコンテキストの削除
		/// </summary>
		/// <param name="[in] context"> 使用するImGuiコンテキスト </param>
		void Finalize(ImGuiContext* context);

		/// <summary>
		/// ImGuiの色設定を開始する
		/// </summary>
		/// <param name="[in] style"> 色を設定したい項目 </param>
		/// <param name="[in] styleColor"> 設定する色 </param>
		void PushColor(ImGuiCol style, const Color& styleColor);

		/// <summary>
		/// ImGuiの色設定を終了する
		/// </summary>
		void PopColor();

		/// <summary>
		/// ImGuiで使うフォントを設定する
		/// </summary>
		/// <param name="[out] io"> 設定対象 </param>
		void SetFont(ImGuiIO& io);

		/// <summary>
		/// ImGuiのボタンを表示する
		/// </summary>
		/// <param name="[in] label"> ボタンの識別名 </param>
		/// <param name="[in] color"> ボタンの色 </param>
		/// <param name="[in] size"> ボタンの大きさ </param>
		/// <returns>
		/// <para> true : ボタンが押された </para>
		/// <para> false : ボタンが押されていない </para>
		/// </returns>
		bool Button(const char* label, const Color& color, const ImVec2& size = { 0, 0 });
	}
}

#endif // !POKARINENGINE_IMGUIHELPER_H_INCLUDED
