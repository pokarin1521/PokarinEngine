/**
* @file GameView.cpp
*/
#include "GameView.h"

#include "../Scene.h"

#include "../Color.h"
#include "../FramebufferObject.h"

namespace PokarinEngine
{
	/// <summary>
	/// 更新
	/// </summary>
	/// <param name="currentScene"> 現在のシーン </param>
	void GameView::Update(const ScenePtr& currentScene)
	{
		// ---------------------------
		// ウィンドウを作成
		// ---------------------------

		// FBOからテクスチャを取得
		ImTextureID texture = ImTextureID(currentScene->GetTextureID());

		// ウィンドウの丸みを無くす
		ImGui::PushStyleVar(ImGuiStyleVar_::ImGuiStyleVar_WindowPadding, ImVec2(0, 0));

		// シーンビュー用ウィンドウ
		ImGui::Begin("Game", nullptr,
			ImGuiWindowFlags_::ImGuiWindowFlags_NoScrollbar |
			ImGuiWindowFlags_::ImGuiWindowFlags_NoScrollWithMouse);
		{
			// シーンビュー用ウィンドウの大きさ
			// 描画時に設定する
			ImVec2 size = ImGui::GetWindowSize();

			// シーンビュー用ウィンドウの大きさで描画
			// そのままだと画像が反転してしまうので、
			// 左下から右上に描画するように設定
			ImGui::Image(texture, size, ImVec2(0, 1), ImVec2(1, 0));

			// ウィンドウの丸み設定を終了
			ImGui::PopStyleVar();

			// ウィンドウを終了
			ImGui::End();
		}

		// -------------------------------
		// カラーバッファをクリア
		// -------------------------------

		//fbo->ClearColor(Color::black);
	}

} // namespace PokarinEngine