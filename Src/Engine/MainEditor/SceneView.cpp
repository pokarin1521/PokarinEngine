/**
* @file SceneView.cpp
*/
#include "SceneView.h"

#include "../Window.h"
#include "../InputManager.h"

namespace PokarinEngine
{
	/// <summary>
	/// カメラの移動操作
	/// </summary>
	void SceneView::CameraMoveControl()
	{
		Vector3 cameraRotation = sceneCamera.rotation;
		const Vector3 cameraSin = { sin(cameraRotation.x),sin(cameraRotation.y),sin(cameraRotation.z) };
		const Vector3 cameraCos = { cos(cameraRotation.x),cos(cameraRotation.y),cos(cameraRotation.z) };

		if (ImGui::IsKeyDown(ImGuiKey_W))
		{
			sceneCamera.position.x -= cameraMoveSpeed * cameraSin.y;
			sceneCamera.position.z -= cameraMoveSpeed * cameraCos.y;
		}

		if (ImGui::IsKeyDown(ImGuiKey_S))
		{
			sceneCamera.position.x += cameraMoveSpeed * cameraSin.y;
			sceneCamera.position.z += cameraMoveSpeed * cameraCos.y;
		}

		if (ImGui::IsKeyDown(ImGuiKey_A))
		{
			sceneCamera.position.x -= cameraMoveSpeed * cameraCos.y;
			sceneCamera.position.z -= cameraMoveSpeed * -cameraSin.y;
		}

		if (ImGui::IsKeyDown(ImGuiKey_D))
		{
			sceneCamera.position.x += cameraMoveSpeed * cameraCos.y;
			sceneCamera.position.z += cameraMoveSpeed * -cameraSin.y;
		}
	}

	/// <summary>
	/// カメラの回転操作
	/// </summary>
	void SceneView::CameraRotateControl()
	{
		// マウスカーソルがImGuiウィンドウから出た場合
		if (!ImGui::IsWindowHovered() ||
			Input::Mouse::IsScreenEdge(WindowID::Main))
		{
			// マウスカーソルを初期位置に戻す
			Input::Mouse::SetScreenPos(WindowID::Main, startMousePos);

			// 位置を更新
			mousePos = startMousePos;
		}

		// 現在のマウスカーソルの位置
		Vector2 currentMousePos = Input::Mouse::GetScreenPos(WindowID::Main);

		// マウスカーソルの移動量
		Vector2 mouseMove = currentMousePos - mousePos;

		// マウスカーソルの位置を更新
		mousePos = currentMousePos;

		// マウスでカメラを操作する
		//sceneCamera.transform->rotation.x -= mouseMove.y * cameraRotateSpeed;
		sceneCamera.rotation.y -= mouseMove.x * cameraRotateSpeed;
	}

	/// <summary>
	/// 更新
	/// </summary>
	void SceneView::Update()
	{
		// FBOからテクスチャを取得
		texture = (void*)(std::intptr_t)fbo->GetTexture();

		// ウィンドウの丸みを無くす
		ImGui::PushStyleVar(ImGuiStyleVar_::ImGuiStyleVar_WindowPadding, ImVec2(0, 0));

		// シーンビュー用ウィンドウ
		ImGui::Begin("Scene", NULL,
			ImGuiWindowFlags_::ImGuiWindowFlags_NoScrollbar |
			ImGuiWindowFlags_::ImGuiWindowFlags_NoScrollWithMouse);
		{
			// ウィンドウ内で右クリックしたら
			// カメラ操作を開始する
			if (ImGui::IsWindowHovered() &&
				ImGui::IsKeyPressed(ImGuiKey_MouseRight, false))
			{
				isControlCamera = true;
				startMousePos = Input::Mouse::GetScreenPos(WindowID::Main);
				mousePos = startMousePos;
			}

			// カメラ操作状態
			if (isControlCamera)
			{
				// マウスカーソルを非表示にする
				ImGui::SetMouseCursor(ImGuiMouseCursor_::ImGuiMouseCursor_None);

				// カメラの位置を操作
				CameraMoveControl();

				// カメラの回転を操作
				CameraRotateControl();

				// 右ボタンを離したら操作を解除する
				isControlCamera = !ImGui::IsKeyReleased(ImGuiKey_MouseRight);
			}

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