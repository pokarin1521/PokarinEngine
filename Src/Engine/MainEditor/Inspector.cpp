/**
* @file Inspector.cpp
*/
#include "Inspector.h"

#include "../Engine.h"

namespace PokarinEngine
{
	/// <summary>
	/// インスペクター(シーン内のオブジェクト制御用ウィンドウ)
	/// </summary>
	namespace Inspector
	{
		/// ここでしか使わないので、cppのみに書く
		/// <summary>
		/// オブジェクト名を表示する
		/// </summary>
		/// <param name="selectObject"> ヒエラルキーで選択中のオブジェクト </param>
		void RenderName(GameObjectPtr selectObject)
		{
			// 選択中オブジェクトの名前
			std::string selectObjectName = selectObject->GetName();

			// オブジェクト名を入力するテキスト欄
				// ##でテキストを非表示する
			if (ImGui::InputText("##ObjectName", selectObjectName.data(), selectObject->GetNameSize(),
				ImGuiInputTextFlags_::ImGuiInputTextFlags_EnterReturnsTrue))
			{
				// 新しい名前が入力されている
				if (selectObjectName[0] != '\0' &&
					selectObjectName != selectObject->GetName())
				{
					// 終端位置
					size_t endLine = selectObjectName.find_first_of('\0');

					// 名前を変更
					// 余計な空白が入ると困るので、最初の\0までを渡す
					selectObject->GetScene().ChangeObjectName(
						selectObject, selectObjectName.substr(0, endLine));
				}
			}
		}

		/// <summary>
		/// 更新
		/// </summary>
		/// <param name="selectObject"> ヒエラルキーで選択中のオブジェクト </param>
		void Inspector::Update(GameObjectPtr selectObject)
		{
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

				// オブジェクト名を表示
				RenderName(selectObject);

				// 区切り線を入れる
				ImGui::Separator();

				// コンポーネントを表示
				for (ComponentPtr component : selectObject->componentList)
				{
					component->RenderInfo();
				}

				ImGui::End();
			}
		}
	}

} // namespace PokarinEngine