/**
* @file MainMenu.cpp
*/
#include "MainMenu.h"

#include "ImGui/imgui.h"

#include "../Scene.h"

namespace PokarinEngine
{
	/// <summary>
	/// 更新
	/// </summary>
	/// <param name="[in,out] currentScene"> 現在のシーン </param>
	void MainMenu::Update(Scene& currentScene)
	{
		// メインメニュー作成
		ImGui::BeginMainMenuBar();
		{
			// ファイル関係のメニュー
			if (ImGui::BeginMenu("File"))
			{
				if (ImGui::MenuItem("Save"))
				{
					currentScene.SaveScene();
				}

				ImGui::EndMenu();
			}

			// ファイル関係のメニュー
			if (ImGui::BeginMenu("File"))
			{
				if (ImGui::MenuItem("Load"))
				{
					currentScene.LoadScene();
				}

				ImGui::EndMenu();
			}

			ImGui::EndMainMenuBar();
		}
	}
}