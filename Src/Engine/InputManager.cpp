/**
* @file InputManager.cpp
*/
#include "InputManager.h"

namespace PokarinEngine
{
	/// <summary>
	/// ���͂Ɋւ��閼�O���
	/// </summary>
	namespace Input
	{
		/// <summary>
		/// �L�[�������Ă��邩�擾����
		/// </summary>
		/// <param name="key"> �L�[ </param>
		/// <returns> �L�[�������Ă����true�ɂȂ� </returns>
		bool GetKey(KeyCode key)
		{
			return ImGui::IsKeyDown(ImGuiKey(key));
		}

		/// <summary>
		/// �L�[�����������擾����
		/// </summary>
		/// <param name="key"> �L�[ </param>
		/// <returns> �L�[���������u�Ԃ���true�ɂȂ� </returns>
		bool GetKeyDown(KeyCode key)
		{
			return ImGui::IsKeyPressed(ImGuiKey(key), false);
		}

		/// <summary>
		/// �L�[�𗣂������擾����
		/// </summary>
		/// <param name="key"> �L�[ </param>
		/// <returns> �L�[�𗣂����u�Ԃ���true�ɂȂ� </returns>
		bool GetKeyUp(KeyCode key)
		{
			return ImGui::IsKeyReleased(ImGuiKey(key));
		}

		/// <summary>
		/// �}�E�X�֌W
		/// </summary>
		namespace Mouse
		{
			/// <summary>
			/// �_�u���N���b�N�������擾����
			/// </summary>
			/// <param name="mouseButton"> �}�E�X�{�^�� </param>
			/// <returns>
			/// <para> true : �_�u���N���b�N���� </para>
			/// <para> false : �_�u���N���b�N���ĂȂ� </para>
			/// </returns>
			bool IsDoubleClick(MouseButton mouseButton)
			{
				return ImGui::IsMouseDoubleClicked(ImGuiMouseButton(mouseButton));
			}

			/// <summary>
			/// �}�E�X�J�[�\������ʒ[�ɂ��邩�擾����
			/// </summary>
			/// <param name="windowID"> �E�B���h�E���ʔԍ� </param>
			/// <returns>
			/// <para> true : �}�E�X�J�[�\������ʒ[�ɂ��� </para>
			/// <para> false : �}�E�X�J�[�\������ʒ[�ɂ��Ȃ� </para>
			/// </returns>
			bool IsScreenEdge(WindowID windowID)
			{
				// ���C�����j�^�[�̏��
				const GLFWvidmode mainMonitor = Window::GetMainMonitor();

				// �}�E�X�J�[�\���̈ʒu
				Vector2 mousePos = GetScreenPos(windowID);

				// �}�E�X�J�[�\���̑傫��
				Vector2 cursorSize = { 2, 35 };

				// �}�E�X�J�[�\���̑傫�����l��������ŁA
				// �}�E�X�J�[�\�������C�����j�^�[�̒[�ɂ��邩��Ԃ�
				return mousePos.x <= 0 || mousePos.x + cursorSize.x >= mainMonitor.width ||
					mousePos.y <= 0 || mousePos.y + cursorSize.y >= mainMonitor.height;
			}

			/// <summary>
			/// �}�E�X�J�[�\���̍��W���擾����
			/// </summary>
			/// <param name="windowID"> �E�B���h�E���ʔԍ� </param>
			/// <returns> �X�N���[�����W�n�̃}�E�X�J�[�\�����W </returns>
			Vector2 GetScreenPos(WindowID windowID)
			{
				double w = 0, h = 0;
				glfwGetCursorPos(&Window::GetWindow(windowID), &w, &h);
				return Vector2(static_cast<float>(w), static_cast<float>(h));
			}

			/// <summary>
			/// �}�E�X�J�[�\���̍��W���擾����
			/// </summary>
			/// <param name="windowID"> �E�B���h�E���ʔԍ� </param>
			/// <returns> �r���[���W�n�̃J�[�\�����W </returns>
			Vector2 GetViewPos(WindowID windowID)
			{
				// -----------------------------------------
				// �X�N���[�����W�n�̃J�[�\�����W���擾
				// -----------------------------------------

				// �߂�l�̌^�ɍ��킹�邽�߂ɁAfloat�ɃL���X�g
				const Vector2 pos = { GetScreenPos(windowID) };

				// ------------------------------------------------------
				// UILayer�̍��W�n�ɍ��킹�邽�߂ɁA
				// �X�N���[�����W�n����r���[���W�n(Z = -1)�ɕϊ����� 
				// ------------------------------------------------------

				// �E�B���h�E�T�C�Y���擾
				int w = 0, h = 0;

				// �^�����킹�邽�߂ɁA�L���X�g
				const Vector2 framebufferSize = { static_cast<float>(w),static_cast<float>(h) };

				// �A�X�y�N�g����擾
				const float aspectRatio = framebufferSize.x / framebufferSize.y;

				// �r���[���W�n�̃J�[�\�����W��Ԃ�
				// (Y���W���t�ɂȂ�̂ŁA-1���|����)
				return { (pos.x / framebufferSize.x * 2 - 1) * aspectRatio,
						 (pos.y / framebufferSize.y * 2 - 1) * -1 };
			}

			/// <summary>
			/// �X�N���[�����W�n�̃}�E�X�J�[�\�����W��ݒ肷��
			/// </summary>
			/// <param name="windowID"> �E�B���h�E���ʔԍ� </param>
			/// <param name="mousePos"> �}�E�X�J�[�\���̍��W </param>
			void SetScreenPos(WindowID windowID, Vector2 mousePos)
			{
				glfwSetCursorPos(&Window::GetWindow(windowID), mousePos.x, mousePos.y);
			}

		} // namespace Mouse

	} // namespace Input

} // namespace PokarinEngine