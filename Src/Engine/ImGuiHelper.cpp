/**
* @file ImGuiHelper.cpp
*/
#include "ImGuiHelper.h"

#include "ImGui/imgui_impl_glfw.h"
#include "ImGui/imgui_impl_opengl3.h"

#include "IconFont/IconFont.h"

namespace PokarinEngine
{
	namespace ImGuiHelper
	{
		// -----------------------
		// 変数
		// -----------------------

		// ImGuiの色設定の回数
		int pushColorCount = 0;

		// -----------------------
		// 関数
		// -----------------------

		/// <summary>
		/// ImGuiの初期化
		/// </summary>
		/// <param name="[in] windowID"> 使用するウィンドウの識別番号 </param>
		void Initialize(WindowID windowID)
		{
			// GLFWに対応するための初期化
			// GLFWコールバックは自動作成にする
			ImGui_ImplGlfw_InitForOpenGL(&Window::GetWindow(windowID), true);

			// OpenGL3.x以上に対応するための初期化
			// GLSLのバージョンを指定
			ImGui_ImplOpenGL3_Init("#version 450");
		}

		/// <summary>
		/// ImGuiの更新
		/// </summary>
		/// <param name="[in] context"> 使用するImGuiコンテキスト </param>
		void Update(ImGuiContext* context)
		{
			// コンテキストを指定
			ImGui::SetCurrentContext(context);

			// 更新
			ImGui_ImplGlfw_NewFrame();
			ImGui_ImplOpenGL3_NewFrame();
			ImGui::NewFrame();
		}

		/// <summary>
		/// ImGuiの描画
		/// </summary>
		/// <param name="[in] context"> 使用するImGuiコンテキスト </param>
		void Render(ImGuiContext* context)
		{
			// コンテキストの指定
			ImGui::SetCurrentContext(context);

			// ImGuiの描画
			ImGui::Render();
			ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
		}

		/// <summary>
		/// ImGuiの終了とコンテキストの削除
		/// </summary>
		/// <param name="[in] context"> 使用するImGuiコンテキスト </param>
		void Finalize(ImGuiContext* context)
		{
			// コンテキストを指定
			ImGui::SetCurrentContext(context);

			// ImGuiを終了する
			ImGui_ImplGlfw_Shutdown();
			ImGui_ImplOpenGL3_Shutdown();

			// コンテキストを削除する
			ImGui::DestroyContext(context);
		}

		/// <summary>
		/// ImGuiの色設定を開始する
		/// </summary>
		/// <param name="[in] style"> 色を設定したい項目 </param>
		/// <param name="[in] styleColor"> 設定する色 </param>
		void PushColor(ImGuiCol style, const Color& styleColor)
		{
			// 色を設定
			ImGui::PushStyleColor(style, styleColor);

			// 設定数をカウント
			pushColorCount++;
		}

		/// <summary>
		/// ImGuiの色設定を終了する
		/// </summary>
		void PopColor()
		{
			ImGui::PopStyleColor(pushColorCount);
			pushColorCount = 0;
		}

		/// <summary>
		/// ImGuiで使うフォントを設定する
		/// </summary>
		/// <param name="[out] io"> 設定対象 </param>
		void SetFont(ImGuiIO& io)
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
		bool Button(const char* label, const Color& color, const ImVec2& size)
		{
			// PushColorとPopColorの間に呼ばれる可能性があるので、
			// 意図しない色設定の終了を避けるためにライブラリの関数を使う

			// ボタンの色を設定
			ImGui::PushStyleColor(ImGuiCol_::ImGuiCol_Button, color);

			// ボタンを表示
			// ボタンが押されたらtrue
			bool isPressed = ImGui::Button(label, size);

			ImGui::PopStyleColor();

			return isPressed;
		}
	}
}