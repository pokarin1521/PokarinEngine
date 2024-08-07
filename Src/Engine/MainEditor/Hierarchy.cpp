/**
* @file Hierarchy.cpp
*/
#include "Hierarchy.h"

#include "ImGui/imgui.h"

#include "../Scene.h"
#include "../GameObject.h"

#include "../Input.h"

#include "../Mesh/Mesh.h"

#include "../Configs/MeshConfig.h"

namespace PokarinEngine
{
#pragma region Hierarchy

	/// <summary>
	/// 更新
	/// </summary>
	/// <param name="currentScene"> 現在のシーン </param>
	void Hierarchy::Update(const ScenePtr& currentScene)
	{
		// シーン内のゲームオブジェクト
		GameObjectList gameObjectList = currentScene->GetGameObjectAll();

		// 選択中のオブジェクトに対する操作を可能にする
		ScelectObjectControl();

		// ヒエラルキーウィンドウ
		ImGui::Begin("Hierarchy", nullptr,
			ImGuiWindowFlags_::ImGuiWindowFlags_MenuBar);
		{
			// ヒエラルキーウィンドウ内の操作
			HierarchyControl();

			// メニュー表示
			Mene();

			// オブジェクト作成用ポップアップの処理
			CreateObjectPopup();

			// シーン内のオブジェクトツリー
			ObjectTree();

			// ヒエラルキーウィンドウを終了
			ImGui::End();
		}
	}

	/// <summary>
	/// ヒエラルキーウィンドウ内での操作
	/// </summary>
	void Hierarchy::HierarchyControl()
	{
		// ウィンドウ内で右クリックした時に
		// オブジェクト作成用ポップアップを展開する
		if (ImGui::IsWindowHovered() &&
			Input::GetKeyUp(KeyCode::MouseRight))
		{
			ImGui::OpenPopup(createObjectPopupName);
		}
	}

	/// <summary>
	/// メニューの処理
	/// </summary>
	void Hierarchy::Mene()
	{
		// メニューバーの中に
		// ゲームオブジェクト作成用ボタンを配置する
		ImGui::BeginMenuBar();
		{
			// ゲームオブジェクト作成ポップアップ用ボタン
			// 黒色で表示
			if (ImGui::Button("Object +", ImVec4(0,0,0,0)))
			{
				// ボタンを押した時に
				// ゲームオブジェクト作成用ポップアップを展開する
				ImGui::OpenPopup(createObjectPopupName);
			}

			// ゲームオブジェクト作成用ポップアップの処理
			CreateObjectPopup();

			ImGui::EndMenuBar();
		}
	}

	/// <summary>
	/// ゲームオブジェクト表示用ツリーノードの処理
	/// </summary>
	void Hierarchy::ObjectTree()
	{
		// シーン内のゲームオブジェクト
		GameObjectList gameObjectList = currentScene->GetGameObjectAll();

		// ツリーノードの機能
		ImGuiTreeNodeFlags treeFlags = ImGuiTreeNodeFlags_::ImGuiTreeNodeFlags_DefaultOpen;

		// シーン内のオブジェクトを表示するためのツリーノード
		if (ImGui::TreeNodeEx(currentScene->GetName(), treeFlags))
		{
			// シーン内のオブジェクトを表示
			for (int i = 0; i < gameObjectList.size(); ++i)
			{
				// ゲームオブジェクト
				const GameObject& gameObject = *gameObjectList[i];

				// ゲームオブジェクトの名前
				std::string objectName = gameObject.name;

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
					Window::OpenWindow(WindowID::NodeScript, "NodeScript");
					gameObject.OpenNodeEditor();
				}
			}

			ImGui::TreePop();
		}
	}

	/// <summary>
	/// 選択中のオブジェクトに対する操作をまとめた関数
	/// </summary>
	void Hierarchy::ScelectObjectControl()
	{
		// 選択中のオブジェクトが無い場合は何もしない
		if (!selectObject)
		{
			return;
		}

		// ------------------------------------------------------
		// 「Ctrl + D」で選択中のオブジェクトを複製する
		// ------------------------------------------------------

		if (Input::GetKey(KeyCode::LeftCtrl) &&
			Input::GetKeyDown(KeyCode::D))
		{
			// 選択中のオブジェクトを複製する
			currentScene->CopyGameObject(selectObject);
		}

		// -------------------------------------------------------
		// 「Delete」で選択中のオブジェクトを削除する
		// -------------------------------------------------------

		if (Input::GetKey(KeyCode::Delete))
		{
			// 選択中のオブジェクトを削除
			currentScene->DestroyObject(selectObject);

			// 非選択状態にする
			selectObjectIndex = unselected;
			selectObject.reset();
		}
	}

	/// <summary>
	/// 選択中のオブジェクトを取得する
	/// </summary>
	/// <returns> ウィンドウ内で選択中のオブジェクト </returns>
	GameObjectPtr& Hierarchy::GetSelectObject()
	{
		return selectObject;
	}

#pragma endregion

#pragma region CreateObject

	/// <summary>
	/// ゲームオブジェクト作成用ポップアップの処理
	/// </summary>
	void Hierarchy::CreateObjectPopup()
	{
		// -----------------------------------
		// ポップアップ開始
		// -----------------------------------

		// 開いていなければ何もせずに終了
		if (!ImGui::BeginPopup(createObjectPopupName))
		{
			return;
		}

		// -----------------------------------
		// ポップアップ内の処理
		// -----------------------------------

		// 3Dオブジェクト作成用メニュー
		if (ImGui::BeginMenu("3D Object"))
		{
			// 直方体生成用ボタン
			CreateObjectButton("Cube", StaticMeshFile::cube);

			// 球体生成用ボタン
			CreateObjectButton("Sphere", StaticMeshFile::sphere);

			// 板生成用ボタン
			CreateObjectButton("Plane", StaticMeshFile::plane);

			// ロボット生成用ボタン
			CreateObjectButton("Robot", StaticMeshFile::robot);

			ImGui::EndMenu();
		}

		// --------------------------------
		// ポップアップの終了
		// --------------------------------

		ImGui::EndPopup();
	}

	/// <summary>
	/// ゲームオブジェクト作成用ボタンの処理
	/// </summary>
	/// <param name="[in] kindName"> 作成するゲームオブジェクトの種類 </param>
	/// <param name="[in] staticMeshFile"> スタティックメッシュのファイル名 </param>
	void Hierarchy::CreateObjectButton(const char* typeName, const char* staticMeshFile)
	{
		// オブジェクト作成用ボタン
		if (ImGui::Button(typeName, Color::black))
		{
			// ボタンが押されたので
			// 現在のシーンにオブジェクトを作成
			GameObjectPtr object = currentScene->CreateGameObject(
				typeName, Vector3(0), Vector3(0));

			// スタティックメッシュを設定
			object->staticMesh = Mesh::GetStaticMesh(staticMeshFile);
		}
	}

#pragma endregion

} // namespace PokarinEngine
