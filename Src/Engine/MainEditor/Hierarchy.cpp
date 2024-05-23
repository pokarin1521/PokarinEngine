/**
* @file Hierarchy.cpp
*/
#include "Hierarchy.h"

#include "ImGui/imgui.h"

#include "../Engine.h"
#include "../Scene.h"

#include "../InputManager.h"

#include "../Configs/MeshConfig.h"

namespace PokarinEngine
{
	/// <summary>
	/// 変数
	/// </summary>
	namespace
	{
		// 非選択時の要素番号
		const int unselected = -1;

		// 選択中オブジェクトの要素番号
		int selectObjectIndex = unselected;

		// 選択中のオブジェクト
		GameObjectPtr selectObject;

		// エンジンクラスのポインタ
		Engine* engine = nullptr;
	}

	/// <summary>
	/// オブジェクト作成用
	/// </summary>
	namespace
	{
		/// <summary>
		/// オブジェクト作成用クラス
		/// </summary>
		class CreateObject
		{
		public: // -------------------- ポップアップ制御 --------------------

			/// <summary>
			/// ゲームオブジェクト作成用ポップアップを開く
			/// </summary>
			void OpenPopup()
			{
				ImGui::OpenPopup(popupName);
			}

			/// <summary>
			/// ゲームオブジェクト作成用ポップアップの処理
			/// </summary>
			void Popup()
			{
				// -----------------------------------
				// ポップアップ開始
				// -----------------------------------

				// 開いていなければ何もせずに終了
				if (!ImGui::BeginPopup(popupName))
				{
					return;
				}

				// -----------------------------------
				// ポップアップ内の処理
				// -----------------------------------

				// 3Dオブジェクト作成用メニュー
				if (ImGui::BeginMenu("3D Object"))
				{
					// 球体生成用ボタン
					Button("Sphere", StaticMeshFile_OBJ::sphere);

					// ロボット生成用ボタン
					Button("Robot", StaticMeshFile_OBJ::robot);

					ImGui::EndMenu();
				}

				// --------------------------------
				// ポップアップの終了
				// --------------------------------

				ImGui::EndPopup();
			}

		private: // --------------------- 作成用ボタン ----------------------

			/// <summary>
			/// ゲームオブジェクト作成用ボタンの処理
			/// </summary>
			/// <param name="kindName"> 作成するゲームオブジェクトの種類 </param>
			/// <param name="staticMeshFile"> スタティックメッシュのファイル名 </param>
			void Button(const char* typeName, const char* staticMeshFile)
			{
				// ボタンの色を無色に設定
				ImGui::PushStyleColor(ImGuiCol_::ImGuiCol_Button, ImVec4(0, 0, 0, 0));

				// オブジェクト作成用ボタン
				if (ImGui::Button(typeName))
				{
					// ボタンが押されたので
					// 現在のシーンにオブジェクトを作成
					GameObjectPtr object = engine->GetCurrentScene().CreateGameObject(
						typeName, Vector3(0), Vector3(0), staticMeshFile);
				}

				// ボタン色の設定を終了
				ImGui::PopStyleColor();
			}

		private: // --------------------- ポップアップ ----------------------

			// オブジェクト作成用ポップアップの名前
			const char* popupName = "CreateObject";
		};

		// オブジェクト作成用
		CreateObject createObject;
	}

	/// <summary>
	/// ヒエラルキー(シーン内のオブジェクト管理用ウィンドウ)
	/// </summary>
	namespace Hierarchy
	{
		/// ここでしか使わないので、cppのみに書く
		/// <summary>
		/// ヒエラルキーウィンドウ内での操作
		/// </summary>
		void HierarchyControl()
		{
			// ウィンドウ内で右クリックした時に
			// オブジェクト作成用ポップアップを展開する
			if (ImGui::IsWindowHovered() &&
				Input::GetKeyUp(KeyCode::MouseRight))
			{
				createObject.OpenPopup();
			}
		}

		/// ここでしか使わないので、cppのみに書く
		/// <summary>
		/// メニューの処理
		/// </summary>
		void Mene()
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
					createObject.OpenPopup();
				}

				// ボタンの色設定を終了
				ImGui::PopStyleColor();

				// ゲームオブジェクト作成用ポップアップの処理
				createObject.Popup();

				ImGui::EndMenuBar();
			}
		}

		/// ここでしか使わないので、cppのみに書く
		/// <summary>
		/// ゲームオブジェクト表示用ツリーノードの処理
		/// </summary>
		void ObjectTree()
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
						Window::OpenWindow(WindowID::NodeScript, "NodeScript");
						gameObject.OpenNodeEditor();
					}
				}

				ImGui::TreePop();
			}
		}

		/// ここでしか使わないので、cppのみに書く
		/// <summary>
		/// 選択中のオブジェクトに対する操作をまとめた関数
		/// </summary>
		void ScelectObjectControl()
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

		/// <summary>
		/// 初期化
		/// </summary>
		void Initialize(Engine& e)
		{
			engine = &e;
		}

		/// <summary>
		/// 更新
		/// </summary>
		void Update()
		{
			// 現在のシーン
			Scene& currentScene = engine->GetCurrentScene();

			// シーン内のゲームオブジェクト
			GameObjectList gameObjectList = currentScene.GetGameObjectAll();

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
				createObject.Popup();

				// シーン内のオブジェクトツリー
				ObjectTree();

				// ヒエラルキーウィンドウを終了
				ImGui::End();
			}
		}

		/// <summary>
		/// 選択中のオブジェクトの要素番号を取得する
		/// </summary>
		/// <returns> ウィンドウ内で選択中のオブジェクトの要素番号 </returns>
		GameObjectPtr GetSelectObject()
		{
			return selectObject;
		}

	} // namespace Hierarchy

} // namespace PokarinEngine
