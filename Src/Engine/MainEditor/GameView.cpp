/**
* @file GameView.cpp
*/
#include "GameView.h"

namespace PokarinEngine
{
	/// <summary>
	/// 更新
	/// </summary>
	void GameView::Update()
	{
		// FBOからテクスチャを取得
		texture = (void*)(std::intptr_t)fbo->GetTexture();

		// ウィンドウの丸みを無くす
		ImGui::PushStyleVar(ImGuiStyleVar_::ImGuiStyleVar_WindowPadding, ImVec2(0, 0));

		// シーンビュー用ウィンドウ
		ImGui::Begin("Game", NULL,
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

		// カラーバッファをクリア
		fbo->ClearColor(backGround);
	}

} // namespace PokarinEngine