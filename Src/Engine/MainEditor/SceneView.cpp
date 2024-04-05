/**
* @file SceneView.cpp
*/
#include "SceneView.h"

#include "../Window.h"
#include "../InputManager.h"

namespace PokarinEngine
{
	/// <summary>
	/// �J�����̈ړ�����
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
	/// �J�����̉�]����
	/// </summary>
	void SceneView::CameraRotateControl()
	{
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
		//sceneCamera.transform->rotation.x -= mouseMove.y * cameraRotateSpeed;
		sceneCamera.rotation.y -= mouseMove.x * cameraRotateSpeed;
	}

	/// <summary>
	/// �X�V
	/// </summary>
	void SceneView::Update()
	{
		// FBO����e�N�X�`�����擾
		texture = (void*)(std::intptr_t)fbo->GetTexture();

		// �E�B���h�E�̊ۂ݂𖳂���
		ImGui::PushStyleVar(ImGuiStyleVar_::ImGuiStyleVar_WindowPadding, ImVec2(0, 0));

		// �V�[���r���[�p�E�B���h�E
		ImGui::Begin("Scene", NULL,
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

		// �J���[�o�b�t�@���N���A
		fbo->ClearColor(backGround);
	}

} // namespace PokarinEngine