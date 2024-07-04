/**
* @file SceneView.cpp
*/
#include "SceneView.h"

#include "ImGui/ImGuizmo.h"

#include "../Window.h"
#include "../InputManager.h"
#include "../Time.h"

namespace PokarinEngine
{
	/// <summary>
	/// カメラの移動操作
	/// </summary>
	void SceneView::CameraMoveControl()
	{
		// 回転速度
		float moveSpeed = cameraMoveSpeed * Time::DeltaTime();

		// カメラの回転角度
		Vector3 cameraRotation = sceneCamera.rotation;

		// カメラの回転角度のSin
		// Z軸は回転させないので、XY軸のみ
		const Vector2 cameraSin = { sin(cameraRotation.x),sin(cameraRotation.y) };

		// カメラの回転角度のCos
		// Z軸は回転させないので、XY軸のみ
		const Vector2 cameraCos = { cos(cameraRotation.x),cos(cameraRotation.y) };

		// ------------------------------------
		// 前に移動
		// ------------------------------------

		// 正面ベクトル
		Vector3 front = sceneCamera.Front();

		if (Input::GetKey(KeyCode::W))
		{
			sceneCamera.position += moveSpeed * front;
		}

		// ------------------------------------
		// 後ろに移動
		// ------------------------------------

		if (Input::GetKey(KeyCode::S))
		{
			sceneCamera.position -= moveSpeed * front;
		}

		// ------------------------------------
		// 右に移動
		// ------------------------------------

		if (Input::GetKey(KeyCode::D))
		{
			sceneCamera.position.x += moveSpeed * cameraCos.y;
			sceneCamera.position.z += moveSpeed * cameraSin.y;
		}

		// ------------------------------------
		// 左に移動
		// ------------------------------------

		if (Input::GetKey(KeyCode::A))
		{
			sceneCamera.position.x -= moveSpeed * cameraCos.y;
			sceneCamera.position.z -= moveSpeed * cameraSin.y;
		}

		// ------------------------------------
		// 上に移動
		// ------------------------------------

		if (Input::GetKey(KeyCode::E))
		{
			sceneCamera.position.y += moveSpeed;
		}

		// ------------------------------------
		// 下に移動
		// ------------------------------------

		if (Input::GetKey(KeyCode::Q))
		{
			sceneCamera.position.y -= moveSpeed;
		}
	}

	/// <summary>
	/// カメラの回転操作
	/// </summary>
	void SceneView::CameraRotateControl()
	{
		// 回転速度
		float rotateSpeed = cameraRotateSpeed * Time::DeltaTime();

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
		sceneCamera.rotation.x -= mouseMove.y * rotateSpeed;
		sceneCamera.rotation.y -= mouseMove.x * rotateSpeed;
	}

	/// <summary>
	/// 更新
	/// </summary>
	void SceneView::Update()
	{
		// -------------------------------
		// FBOからテクスチャを取得する
		// -------------------------------

		texture = ImTextureID(*fbo->GetTexture());

		// -------------------------------
		// ウィンドウを作成
		// -------------------------------

		// ウィンドウの丸みを無くす
		ImGui::PushStyleVar(ImGuiStyleVar_::ImGuiStyleVar_WindowPadding, ImVec2(0, 0));

		// シーンビュー用ウィンドウ
		ImGui::Begin("Scene", nullptr,
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

		// ----------------------------------
		// カラーバッファをクリア
		// ----------------------------------

		fbo->ClearColor(backGround);

		// ----------------------------------
		// ギズモを無効にする
		// ----------------------------------

		ImGuizmo::Enable(false);
	}

} // namespace PokarinEngine