/**
* @file Inspector.cpp
*/
#include "Inspector.h"

#include "../Engine.h"
#include "../Color.h"

#include "../Components/Rigidbody.h"

namespace PokarinEngine
{
	/// <summary>
	/// コンポーネント追加用
	/// </summary>
	namespace AddComponent
	{
		// -----------------------------
		// 変数
		// -----------------------------

		// ポップアップの名前
		const char* popupName = "AddComponent";

		// ----------------------------------
		// 関数
		// ----------------------------------

		/// <summary>
		/// コンポーネント追加用ボタン
		/// </summary>
		/// <typeparam name="T">  </typeparam>
		template<class T>
		void AddButton(GameObjectPtr object)
		{

		}

		/// <summary>
		/// ポップアップ展開用ボタン
		/// </summary>
		/// <returns>
		/// <para> true : 展開中 </para>
		/// <para> false : 展開していない </para>
		/// </returns>
		bool OpenPopupButton()
		{
			static bool isOpen = false;

			// ボタンのX位置
			const float posX = 5.0f;

			// ボタンの大きさ
			const ImVec2 size = { ImGui::GetWindowWidth() , 25.0f };

			// そのままだと位置がずれてしまうので、
			// X位置を設定
			ImGui::SetCursorPosX(posX);

			// コンポーネント追加用ボタン
			// 押されたらポップアップ展開
			if (ImGui::Button("Add Component", size))
			{
				//ImGui::OpenPopup(popupName);
				isOpen = true;
			}

			return isOpen;
		}

		/// <summary>
		/// ポップアップ展開中の処理
		/// </summary>
		/// <param name="object"> ゲームオブジェクト </param>
		void DownMenu(GameObjectPtr object)
		{

			if (ImGui::CollapsingHeader("Add Component", ImGuiTreeNodeFlags_Bullet))
			{
				ImGui::Text("Select Component");
			}

			//AddButton<Rigidbody>(object);
		}
	}

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
					selectObject->GetOwnerScene().ChangeObjectName(
						selectObject, selectObjectName.substr(0, endLine));
				}
			}
		}

		/// <summary>
		/// 更新
		/// </summary>
		/// <param name="selectObject"> ヒエラルキーで選択中のオブジェクト </param>
		void Update(GameObjectPtr selectObject)
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
				for (auto& component : selectObject->componentList)
				{
					// 表示するコンポーネント名
					// 区別できるように、オブジェクトの識別番号を付けておく(非表示)
					const std::string componentName = component->GetName() + "##" + std::to_string(selectObject->GetID());

					// 折り畳み可能なヘッダーで表示
					if (ImGui::CollapsingHeader(componentName.c_str(), ImGuiTreeNodeFlags_DefaultOpen))
					{
						// ヘッダーを展開中に情報の編集ができるように表示する
						component->InfoEditor();
					}
				}

				//AddComponent::DownMenu(selectObject);

				ImGui::End();
			}
		}
	}

} // namespace PokarinEngine