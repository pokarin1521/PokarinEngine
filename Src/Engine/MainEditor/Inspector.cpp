/**
* @file Inspector.cpp
*/
#include "Inspector.h"

#include "../Engine.h"

namespace PokarinEngine
{
	/// <summary>
	/// 初期化
	/// </summary>
	void Inspector::Initialize(Engine& e)
	{
		engine = &e;
	}

	/// <summary>
	/// 更新
	/// </summary>
	/// <param name="selectObject"> ヒエラルキーで選択中のオブジェクト </param>
	void Inspector::Update(GameObjectPtr selectObject)
	{
		// 現在のシーン
		Scene& currentScene = engine->GetCurrentScene();

		// シーン内のゲームオブジェクト
		GameObjectList gameObjectList = currentScene.GetGameObjectAll();

		// インスペクターウィンドウ
		ImGui::Begin("Inspector");
		{
			// ヒエラルキーでオブジェクトを選択していないなら
			// 何も表示しない
			if (!selectObject)
			{
				ImGui::End();
				return;
			}

			// 選択中のオブジェクトの名前を取得
			selectObjectName = selectObject->GetName();

			// 名前を書き換えられるように
			// オブジェクト名の最大文字数を設定
			selectObjectName.resize(selectObject->GetNameSize());

			// オブジェクト名を入力するテキスト欄
			// ##でテキストを非表示する
			if (ImGui::InputText("##ObjectName", selectObjectName.data(), selectObject->GetNameSize(),
				ImGuiInputTextFlags_::ImGuiInputTextFlags_EnterReturnsTrue | 
				ImGuiInputTextFlags_::ImGuiInputTextFlags_None))
			{
				// 新しい名前が入力されている
				if (selectObjectName[0] != '\0' &&
					selectObjectName != selectObject->GetName())
				{
					// 終端位置
					size_t endLine = selectObjectName.find_first_of('\0');

					// 名前を変更
					// 余計な\0が入ると困るので、最初の\0までを渡す
					currentScene.ChangeObjectName(selectObject, selectObjectName.substr(0, endLine));
				}
			}

			// 区切り線を入れる
			ImGui::Separator();

			// コンポーネントを表示
			for (ComponentPtr component : selectObject->components)
			{
				component->RenderEditor();
			}

			ImGui::End();
		}
	}

} // namespace PokarinEngine