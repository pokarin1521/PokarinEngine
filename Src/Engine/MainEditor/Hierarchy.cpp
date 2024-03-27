/**
* @file Hierarchy.cpp
*/
#include "Hierarchy.h"

#include "ImGui/imgui.h"

#include "../Engine.h"
#include "../GameObject.h"
#include "../Scene.h"

#include "../InputManager.h"

#include "../Settings/MeshSettings.h"

namespace PokarinEngine
{
#pragma region CreateGameObject

	/// <summary>
	/// ゲームオブジェクト作成用ボタンの処理
	/// </summary>
	/// <param name="kindName"> 作成するゲームオブジェクトの種類 </param>
	/// <param name="staticMeshFile"> スタティックメッシュのファイル名 </param>
	void Hierarchy::CreateObjectButton(
		const char* typeName, const char* staticMeshFile)
	{
		// ボタンの色を無色に設定
		ImGui::PushStyleColor(ImGuiCol_::ImGuiCol_Button, ImVec4(0, 0, 0, 0));

		// オブジェクト作成用ボタン
		if (ImGui::Button(typeName))
		{
			// ボタンが押されたので
			// 現在のシーンにオブジェクトを作成
			GameObjectPtr object = engine->GetCurrentScene().CreateGameObject(
				typeName, Vec3(0), Vec3(0), staticMeshFile);
		}

		// ボタン色の設定を終了
		ImGui::PopStyleColor();
	}

	/// <summary>
	/// ゲームオブジェクト作成用ポップアップを展開する
	/// </summary>
	void Hierarchy::CreateObjectPopup_Open()
	{
		ImGui::OpenPopup("CreateObject");
	}

	/// <summary>
	/// ゲームオブジェクト作成用ポップアップを展開した時の処理
	/// </summary>
	void Hierarchy::CreateObjectPopup_Run()
	{
		// ゲームオブジェクト作成用ポップアップ
		if (ImGui::BeginPopup("CreateObject"))
		{
			// 3Dオブジェクト作成用メニュー
			if (ImGui::BeginMenu("3D Object"))
			{
				// 球体生成用ボタン
				CreateObjectButton("Sphere", StaticMeshFile_OBJ::sphere);

				// ロボット生成用ボタン
				CreateObjectButton("Robot", StaticMeshFile_OBJ::robot);

				ImGui::EndMenu();
			}

			ImGui::EndPopup();
		}
	}

#pragma endregion

#pragma region Event

	/// <summary>
	/// 初期化
	/// </summary>
	void Hierarchy::Initialize(Engine& e)
	{
		engine = &e;
	}

	/// <summary>
	/// 更新
	/// </summary>
	void Hierarchy::Update()
	{
		// 現在のシーン
		Scene& currentScene = engine->GetCurrentScene();

		// シーン内のゲームオブジェクト
		GameObjectList gameObjectList = currentScene.GetGameObjectAll();

		// 選択中のオブジェクトに対する操作を可能にする
		ScelectObjectControl();

		// ヒエラルキーウィンドウ
		ImGui::Begin("Hierarchy", NULL,
			ImGuiWindowFlags_::ImGuiWindowFlags_MenuBar);
		{
			// ウィンドウ内で右クリックした時に
			// オブジェクト作成用ポップアップを展開する
			if (ImGui::IsWindowHovered() &&
				Input::GetKeyUp(KeyCode::MouseRight))
			{
				CreateObjectPopup_Open();
			}

			// オブジェクト作成用ポップアップを
			// 展開した時の処理
			CreateObjectPopup_Run();

			// メニュー表示
			Mene();

			// シーン内のオブジェクトツリー
			ObjectTree();

			// ヒエラルキーウィンドウを終了
			ImGui::End();
		}
	}

#pragma endregion

#pragma region HierarchyWindow

	/// <summary>
	/// メニューの処理
	/// </summary>
	void Hierarchy::Mene()
	{
		// メニューバーの中に
		// ゲームオブジェクト作成用ボタンを配置する
		ImGui::BeginMenuBar();
		{
			// ボタンの色を設定
			ImGui::PushStyleColor(ImGuiCol_::ImGuiCol_Button, ImVec4(0, 0, 0, 0));

			// ゲームオブジェクト作成ポップアップ用ボタン
			if (ImGui::Button("Object +"))
			{
				// ボタンを押した時に
				// ゲームオブジェクト作成用ポップアップを展開する
				CreateObjectPopup_Open();
			}

			// ボタンの色設定を終了
			ImGui::PopStyleColor();

			// ゲームオブジェクト作成用ポップアップ
			CreateObjectPopup_Run();

			ImGui::EndMenuBar();
		}
	}

	/// <summary>
	/// ゲームオブジェクト表示用ツリーノードの処理
	/// </summary>
	void Hierarchy::ObjectTree()
	{
		// 現在のシーン
		Scene& currentScene = engine->GetCurrentScene();

		// シーン内のゲームオブジェクト
		GameObjectList gameObjectList = currentScene.GetGameObjectAll();

		// ツリーノードの機能
		ImGuiTreeNodeFlags treeFlags = ImGuiTreeNodeFlags_::ImGuiTreeNodeFlags_DefaultOpen;

		// シーン内のオブジェクトを表示するためのツリーノード
		if (ImGui::TreeNodeEx(currentScene.GetName(), treeFlags))
		{
			// シーン内のオブジェクトを表示
			for (int i = 0; i < gameObjectList.size(); ++i)
			{
				// ゲームオブジェクト
				const GameObject& gameObject = *gameObjectList[i];

				// ゲームオブジェクトの名前
				std::string objectName = gameObject.GetName();

				// 重複しないように識別番号を追加
				// ##で非表示にする
				objectName += "##" + std::to_string(gameObject.GetID());

				// 選択可能な項目として表示
				// 選択中なら強調表示するように引数で設定
				if (ImGui::Selectable(objectName.c_str(), selectObjectIndex == i))
				{
					// 選択中オブジェクトの要素番号を設定
					selectObjectIndex = i;

					// 選択中のオブジェクトを設定
					selectObject = gameObjectList[i];
				}

				// オブジェクトの項目をダブルクリックしたら
				// ウィンドウを表示
				if (ImGui::IsItemHovered() &&
					Input::Mouse::IsDoubleClick(MouseButton::Left))
				{
					// テスト用に
					// ダブルクリックしたオブジェクトの名前をタイトルにする
					Window::OpenWindow(WindowID::NodeScript, gameObject.GetName());
					gameObject.OpenNodeEditor();
				}
			}

			ImGui::TreePop();
		}
	}

	/// <summary>
	/// 選択中のオブジェクトに対する制御をまとめた関数
	/// </summary>
	void Hierarchy::ScelectObjectControl()
	{
		// 選択中のオブジェクトが無い場合は何もしない
		if (!selectObject)
		{
			return;
		}

		// 現在のシーン
		Scene& currentScene = engine->GetCurrentScene();

		// ------------------------------------------------------
		// 「Ctrl + D」で選択中のオブジェクトを複製する
		// ------------------------------------------------------

		if (Input::GetKey(KeyCode::LeftCtrl) &&
			Input::GetKeyDown(KeyCode::D))
		{
			// 選択中のオブジェクトを複製する
			currentScene.CopyGameObject(selectObject);
		}

		// -------------------------------------------------------
		// 「Delete」で選択中のオブジェクトを削除する
		// -------------------------------------------------------

		if (Input::GetKey(KeyCode::Delete))
		{
			// 選択中のオブジェクトを削除
			currentScene.DestroyObject(selectObject);

			// 非選択状態にする
			selectObjectIndex = unselected;
			selectObject.reset();
		}
	}

#pragma endregion

} // namespace PokarinEngine
