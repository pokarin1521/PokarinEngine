/**
* @file SceneView.cpp
*/
#include "SceneView.h"

#include "../Window.h"
#include "../Input.h"
#include "../Time.h"

#include "../Components/Transform.h"

namespace PokarinEngine
{
	/// <summary>
	/// �J�����̈ړ�����
	/// </summary>
	void SceneView::CameraMoveControl()
	{
		// ��]���x
		float moveSpeed = cameraMoveSpeed * Time::DeltaTime();

		// �J�����̉�]�p�x
		Vector3 cameraRotation = camera.transform.rotation;

		// �J�����̉�]�p�x��Sin
		// Z���͉�]�����Ȃ��̂ŁAXY���̂�
		const Vector2 cameraSin = { sin(cameraRotation.x),sin(cameraRotation.y) };

		// �J�����̉�]�p�x��Cos
		// Z���͉�]�����Ȃ��̂ŁAXY���̂�
		const Vector2 cameraCos = { cos(cameraRotation.x),cos(cameraRotation.y) };

		// ------------------------------------
		// �O�Ɉړ�
		// ------------------------------------

		// ���ʃx�N�g��
		Vector3 front = camera.transform.Front();

		if (Input::GetKey(KeyCode::W))
		{
			camera.transform.position += moveSpeed * front;
		}

		// ------------------------------------
		// ���Ɉړ�
		// ------------------------------------

		if (Input::GetKey(KeyCode::S))
		{
			camera.transform.position -= moveSpeed * front;
		}

		// ------------------------------------
		// �E�Ɉړ�
		// ------------------------------------

		if (Input::GetKey(KeyCode::D))
		{
			camera.transform.position.x += moveSpeed * cameraCos.y;
			camera.transform.position.z += moveSpeed * cameraSin.y;
		}

		// ------------------------------------
		// ���Ɉړ�
		// ------------------------------------

		if (Input::GetKey(KeyCode::A))
		{
			camera.transform.position.x -= moveSpeed * cameraCos.y;
			camera.transform.position.z -= moveSpeed * cameraSin.y;
		}

		// ------------------------------------
		// ��Ɉړ�
		// ------------------------------------

		if (Input::GetKey(KeyCode::E))
		{
			camera.transform.position.y += moveSpeed;
		}

		// ------------------------------------
		// ���Ɉړ�
		// ------------------------------------

		if (Input::GetKey(KeyCode::Q))
		{
			camera.transform.position.y -= moveSpeed;
		}
	}

	/// <summary>
	/// �J�����̉�]����
	/// </summary>
	void SceneView::CameraRotateControl()
	{
		// ��]���x
		float rotateSpeed = cameraRotateSpeed * Time::DeltaTime();

		// �}�E�X�J�[�\����ImGui�E�B���h�E����o���ꍇ
		if (!ImGui::IsWindowHovered() ||
			Input::Mouse::IsScreenEdge(WindowID::Main))
		{
			// �}�E�X�J�[�\���������ʒu�ɖ߂�
			Input::Mouse::SetScreenPos(WindowID::Main, startMousePos);

			// �ʒu���X�V
			mousePos = startMousePos;
		}

		// ���݂̃}�E�X�J�[�\���̈ʒu
		Vector2 currentMousePos = Input::Mouse::GetScreenPos(WindowID::Main);

		// �}�E�X�J�[�\���̈ړ���
		Vector2 mouseMove = currentMousePos - mousePos;

		// �}�E�X�J�[�\���̈ʒu���X�V
		mousePos = currentMousePos;

		// �}�E�X�ŃJ�����𑀍삷��
		camera.transform.rotation.x -= mouseMove.y * rotateSpeed;
		camera.transform.rotation.y -= mouseMove.x * rotateSpeed;
	}

	/// <summary>
	/// �X�V
	/// </summary>
	void SceneView::Update()
	{
		// -------------------------------
		// �E�B���h�E���쐬
		// -------------------------------

		// �E�B���h�E�̊ۂ݂𖳂���
		ImGui::PushStyleVar(ImGuiStyleVar_::ImGuiStyleVar_WindowPadding, ImVec2(0, 0));

		// �V�[���r���[�p�E�B���h�E
		ImGui::Begin("Scene", nullptr,
			ImGuiWindowFlags_::ImGuiWindowFlags_NoScrollbar |
			ImGuiWindowFlags_::ImGuiWindowFlags_NoScrollWithMouse);
		{
			// �E�B���h�E���ŉE�N���b�N������
			// �J����������J�n����
			if (ImGui::IsWindowHovered() &&
				ImGui::IsKeyPressed(ImGuiKey_MouseRight, false))
			{
				isControlCamera = true;
				startMousePos = Input::Mouse::GetScreenPos(WindowID::Main);
				mousePos = startMousePos;
			}

			// �J����������
			if (isControlCamera)
			{
				// �}�E�X�J�[�\�����\���ɂ���
				ImGui::SetMouseCursor(ImGuiMouseCursor_::ImGuiMouseCursor_None);

				// �J�����̈ʒu�𑀍�
				CameraMoveControl();

				// �J�����̉�]�𑀍�
				CameraRotateControl();

				// �E�{�^���𗣂����瑀�����������
				isControlCamera = !ImGui::IsKeyReleased(ImGuiKey_MouseRight);
			}

			// FBO�̃e�N�X�`�����ʔԍ�
			ImTextureID texture = ImTextureID(camera.GetTextureID());

			// �V�[���r���[�p�E�B���h�E�̑傫��
			// �`�掞�ɐݒ肷��
			ImVec2 size = ImGui::GetWindowSize();

			// �V�[���r���[�p�E�B���h�E�̑傫���ŕ`��
			// ���̂܂܂��Ɖ摜�����]���Ă��܂��̂ŁA
			// ��������E��ɕ`�悷��悤�ɐݒ�
			ImGui::Image(texture, size, ImVec2(0, 1), ImVec2(1, 0));

			// �E�B���h�E�̊ۂݐݒ���I��
			ImGui::PopStyleVar();

			// �E�B���h�E���I��
			ImGui::End();
		}

		// ----------------------------------
		// �J���[�o�b�t�@���N���A
		// ----------------------------------

		fbo->ClearColor(backGround);
	}

} // namespace PokarinEngine