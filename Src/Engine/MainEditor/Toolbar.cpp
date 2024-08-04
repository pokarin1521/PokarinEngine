/**
* @file Toolbar.cpp
*/
#include "Toolbar.h"

#include "ImGui/imgui.h"

#include "../Engine.h"
#include "../TextureGetter.h"

#include <unordered_map>

namespace PokarinEngine
{
	/// <summary>
	/// ツールバー
	/// </summary>
	namespace Toolbar
	{
		/// <summary>
		/// ツールバーで使用する画像のファイル名
		/// </summary>
		namespace ImageName
		{
			// 再生ボタン
			constexpr const char* playButton = "Res/Toolbar/PlayButton.tga";

			// 停止ボタン
			constexpr const char* stopButton = "Res/Toolbar/StopButton.tga";
		}

		/// <summary>
		/// ツールバーで使用する画像の識別番号
		/// </summary>
		namespace ImageID
		{
			// 再生・停止ボタン
			// [ゲームが再生中ならtrue, テクスチャ識別番号]
			std::unordered_map<bool, ImTextureID> playButton;
		}

		/// <summary>
		/// 初期化
		/// </summary>
		void Initialize()
		{
			// 再生ボタンのテクスチャ識別番号を追加
			ImageID::playButton.emplace(false, ImTextureID(*TextureGetter::Get(ImageName::playButton)));

			// 停止ボタンのテクスチャ識別番号を追加
			ImageID::playButton.emplace(true, ImTextureID(*TextureGetter::Get(ImageName::stopButton)));
		}

		/// <summary>
		/// 更新
		/// </summary>
		/// <param name="[out] isPlayGame"> ゲームが再生中ならtrue </param>
		void Update(bool& isPlayGame)
		{
			ImGui::Begin("Toolbar", nullptr,
				ImGuiWindowFlags_::ImGuiWindowFlags_NoScrollbar |
				ImGuiWindowFlags_::ImGuiWindowFlags_NoScrollWithMouse);
			{
				// 再生ボタンのX座標
				// ウィンドウの真ん中あたりに設定する
				float playButtonPosX = ImGui::GetWindowWidth() / 2.0f;

				// 再生・停止ボタンの位置を設定
				ImGui::SetCursorPosX(playButtonPosX);

				// ボタンのサイズ
				static const ImVec2 buttonSize = { 30, 30 };

				// 再生中なら停止ボタンの画像を、停止中なら再生ボタンの画像を表示する
				if (ImGui::ImageButton(ImageID::playButton[isPlayGame], buttonSize, ImVec2(0, 1), ImVec2(1, 0)))
				{
					// 再生状況を切り替える
					isPlayGame = !isPlayGame;
				}

				ImGui::End();
			}
		}
	}
}