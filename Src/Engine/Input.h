/**
* @file InputManager.h
*/
#ifndef POKARINENGINE_INPUT_H_INCLUDED
#define POKARINENGINE_INPUT_H_INCLUDED

#include "glad/glad.h"
#include "ImGui/imgui.h"
#include "Math/Vector.h"
#include "Window.h"

#include <memory>

namespace PokarinEngine
{
	/// <summary>
	/// �L�[�w��p
	/// </summary>
	enum class KeyCode
	{
		// ------------------------
		// �L�[�{�[�h
		// ------------------------

		LeftCtrl = ImGuiKey_LeftCtrl,
		RightCtrl = ImGuiKey_RightCtrl,
		A = ImGuiKey_A,
		C = ImGuiKey_C,
		D = ImGuiKey_D,
		E = ImGuiKey_E,
		L = ImGuiKey_L,
		Q = ImGuiKey_Q,
		S = ImGuiKey_S,
		W = ImGuiKey_W,
		Delete = ImGuiKey_Delete,
		LeftAlt = ImGuiKey_LeftAlt,

		// ------------------------
		// �}�E�X
		// ------------------------

		MouseLeft = ImGuiKey_MouseLeft,
		MouseRight = ImGuiKey_MouseRight,
		MouseMiddle = ImGuiKey_MouseMiddle,
	};

	/// <summary>
	/// �}�E�X�{�^���w��p
	/// </summary>
	enum class MouseButton
	{
		Left = ImGuiMouseButton_Left,
		Right = ImGuiMouseButton_Right,
		Middle = ImGuiMouseButton_Middle,
	};

	/// <summary>
	/// ���͂Ɋւ��閼�O���
	/// </summary>
	namespace Input
	{
		/// <summary>
		/// �L�[�������Ă��邩�擾����
		/// </summary>
		/// <param name="[in] key"> �L�[ </param>
		/// <returns> �L�[�������Ă����true�ɂȂ� </returns>
		bool GetKey(KeyCode key);

		/// <summary>
		/// �L�[�����������擾����
		/// </summary>
		/// <param name="[in] key"> �L�[ </param>
		/// <returns> �L�[���������u�Ԃ���true�ɂȂ� </returns>
		bool GetKeyDown(KeyCode key);

		/// <summary>
		/// �L�[�𗣂������擾����
		/// </summary>
		/// <param name="[in] key"> �L�[ </param>
		/// <returns> �L�[�𗣂����u�Ԃ���true�ɂȂ� </returns>
		bool GetKeyUp(KeyCode key);

		/// <summary>
		/// �}�E�X�֌W
		/// </summary>
		namespace Mouse
		{
			/// <summary>
			/// �_�u���N���b�N�������擾����
			/// </summary>
			/// <param name="[in] mouseButton"> �}�E�X�{�^�� </param>
			/// <returns>
			/// <para> true : �_�u���N���b�N���� </para>
			/// <para> false : �_�u���N���b�N���Ă��Ȃ� </para>
			/// </returns>
			bool IsDoubleClick(MouseButton mouseButton);

			/// <summary>
			/// �}�E�X�J�[�\������ʒ[�ɂ��邩�擾����
			/// </summary>
			/// <param name="[in] windowID"> �E�B���h�E���ʔԍ� </param>
			/// <returns>
			/// <para> true : �}�E�X�J�[�\������ʒ[�ɂ��� </para>
			/// <para> false : �}�E�X�J�[�\������ʒ[�ɂ��Ȃ� </para>
			/// </returns>
			bool IsScreenEdge(WindowID windowID);

			/// <summary>
			/// �}�E�X�J�[�\���̍��W���擾����
			/// </summary>
			/// <param name="[in] windowID"> �E�B���h�E���ʔԍ� </param>
			/// <returns> �X�N���[�����W�n�̃}�E�X�J�[�\�����W </returns>
			Vector2 GetScreenPos(WindowID windowID);

			/// <summary>
			/// �}�E�X�J�[�\���̍��W���擾����
			/// </summary>
			/// <param name="[in] windowID"> �E�B���h�E���ʔԍ� </param>
			/// <returns> �r���[���W�n�̃J�[�\�����W </returns>
			Vector2 GetViewPos(WindowID windowID);

			/// <summary>
			/// �X�N���[�����W�n�̃}�E�X�J�[�\�����W��ݒ肷��
			/// </summary>
			/// <param name="[in] windowID"> �E�B���h�E���ʔԍ� </param>
			/// <param name="[in] mousePos"> �}�E�X�J�[�\���̍��W </param>
			void SetScreenPos(WindowID windowID, const Vector2& mousePos);

		} // namespace Mouse

	} // namespace Input

} // namespace PokarinEngine

#endif // !POKARINENGINE_INPUT_H_INCLUDED