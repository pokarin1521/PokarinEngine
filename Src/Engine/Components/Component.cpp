/**
* @file Component.cpp
*/
#include "Component.h"

#include "IconFont/IconFont.h"
#include "Json/Json.h"

#include "../GameObject.h"
#include "../Scene.h"

namespace PokarinEngine
{
	/// <summary>
	/// コンポーネント操作用ポップアップの処理
	/// </summary>
	void Component::ControlPopup()
	{
		// transformは削除しないようにする
		// 現状削除しかないので何もしない
		if (ownerObject->transform.get() == this)
		{
			return;
		}

		// メインカメラは現状削除しないようにする
		if (&ownerObject->GetOwnerScene().GetMainCameraInfo() == this)
		{
			return;
		}

		// ポップアップ展開用ボタンをヘッダーの右端に表示
		const float posX = ImGui::GetItemRectSize().x - 20.0f;
		ImGui::SameLine(posX);

		// ポップアップ名
		// 区別用に識別番号を追加
		const std::string popupName = "ComponentControl##" + std::to_string(id);

		// ポップアップ展開用ボタンの名前
		// 歯車のアイコンを表示(区別用に識別番号を追加)
		const std::string popupButton = ICON_FA_GEAR"##" + std::to_string(id);

		// 色が重なって目立つので、ボタンの背景色を透明にする
		ImGui::PushStyleColor(ImGuiCol_::ImGuiCol_Button, ImVec4(0, 0, 0, 0));

		// ポップアップ展開用ボタン
		// 歯車で表示
		if (ImGui::Button(popupButton.c_str()))
		{
			ImGui::OpenPopup(popupName.c_str());
		}

		// ボタンの色設定を終了
		ImGui::PopStyleColor();

		// ポップアップ展開中にボタンを表示する
		if (ImGui::BeginPopup(popupName.c_str()))
		{
			// コンポーネント削除用ボタン
			if (ImGui::Button("Delete Component"))
			{
				Destroy();
			}

			ImGui::EndPopup();
		}
	}

	/// <summary>
	/// エディタに情報を表示する
	/// </summary>
	void Component::RenderInfo()
	{
		// 折り畳み可能なヘッダーで表示
		bool isOpen = ImGui::CollapsingHeader(title.c_str(),
			ImGuiTreeNodeFlags_AllowItemOverlap | ImGuiTreeNodeFlags_DefaultOpen);

		// 操作用ポップアップ
		ControlPopup();

		// ヘッダー展開中にコンポーネントの情報を表示する
		if (isOpen)
		{
			InfoEditor();
		}
	}
}