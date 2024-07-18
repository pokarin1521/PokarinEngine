/**
* @file Inspector.cpp
*/
#include "Inspector.h"

#include "../Engine.h"
#include "../GameObject.h"
#include "../Color.h"

#include "../Components/ComponentAdder.h"

namespace PokarinEngine
{
#pragma region Inspector

	/// <summary>
	/// オブジェクト名を表示する
	/// </summary>
	void Inspector::RenderName()
	{
		// 選択中オブジェクトの名前
		std::string selectObjectName = selectObject->name;

		// オブジェクト名を入力するテキスト欄
		// ##でテキストを非表示する
		if (ImGui::InputText("##ObjectName", selectObjectName.data(), selectObject->nameSize,
			ImGuiInputTextFlags_::ImGuiInputTextFlags_EnterReturnsTrue))
		{
			// 新しい名前が入力されている
			if (selectObjectName[0] != '\0' &&
				selectObjectName != selectObject->name)
			{
				// 終端位置
				size_t endLine = selectObjectName.find_first_of('\0');

				// 名前を変更
				// 余計な空白が入ると困るので、最初の\0までを渡す
				selectObject->name = selectObjectName.substr(0, endLine);
			}
		}
	}

	/// <summary>
	/// 更新
	/// </summary>
	/// <param name="[in,out] hierarchySelect"> ヒエラルキーで選択中のオブジェクト </param>
	void Inspector::Update(GameObjectPtr& hierarchySelect)
	{
		// インスペクターウィンドウ
		ImGui::Begin("Inspector");
		{
			// 選択中のオブジェクトを更新
			selectObject = hierarchySelect;

			// ヒエラルキーでオブジェクトを選択していないなら
			// 何も表示しない
			if (!selectObject)
			{
				ImGui::End();
				return;
			}

			// オブジェクト名を表示
			RenderName();

			// 区切り線を入れる
			ImGui::Separator();

			// コンポーネントを表示
			selectObject->RenderComponent();

			// コンポーネント追加用ポップアップ
			AddComponentPopup();

			ImGui::End();
		}
	}

#pragma endregion

#pragma region AddComponent

	/// ここでしか使わないので、cppのみに書く
	/// <summary>
	/// ポップアップ展開用ボタンの設定
	/// </summary>
	void PopupButtonSetting()
	{
		// ボタンのX位置
		const float posX = 5.0f;

		// ボタンの大きさ
		const ImVec2 size = { ImGui::GetWindowWidth() , 25.0f };

		// そのままだと位置がずれてしまうので、
		// X位置を設定
		ImGui::SetCursorPosX(posX);
	}

	/// ここでしか使わないので、cppのみに書く
	/// <summary>
	/// ポップアップの設定
	/// </summary>
	void PopupSetting()
	{
		// ポップアップの幅
		// ウィンドウ幅に合わせる
		const float width = ImGui::GetWindowWidth();

		// ポップアップの大きさを設定
		// 高さは表示する数に応じて変わるようにするために、0に設定する
		ImGui::SetNextWindowSize(ImVec2(width, 0));

		// ポップアップの位置
		// ポップアップ展開用ボタンの下に表示する
		const ImVec2 pos = { ImGui::GetItemRectMin().x, ImGui::GetItemRectMax().y };

		// ポップアップの位置を設定
		ImGui::SetNextWindowPos(pos);
	}

	/// <summary>
	/// コンポーネント追加用ポップアップの処理
	/// </summary>
	void Inspector::AddComponentPopup()
	{
		// ポップアップ名
		static const char* popupName = "AddComponent Popup";

		// ポップアップ展開用ボタンの設定
		PopupButtonSetting();

		// ポップアップ展開用ボタン
		if (ImGui::Button("Add Component"))
		{
			ImGui::OpenPopup(popupName);
		}

		// ポップアップの設定
		PopupSetting();

		// ポップアップ展開中
		// コンポーネント追加用リストを表示
		if (ImGui::BeginPopup(popupName, ImGuiWindowFlags_::ImGuiWindowFlags_NoMove))
		{
			ComponentAdder::RenderList(selectObject);

			ImGui::EndPopup();
		}
	}

#pragma endregion

} // namespace PokarinEngine