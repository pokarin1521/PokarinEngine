/**
* @file Window.h
*/
#ifndef WINDOW_H_INCLUDED
#define WINDOW_H_INCLUDED

#include "glad/glad.h"
#include "Math/Vector.h"

#include <GLFW/glfw3.h>

namespace PokarinEngine
{
	/// <summary>
	/// �E�B���h�E���ʔԍ�
	/// </summary>
	enum class WindowID
	{
		Main,	     // ���C��
		NodeScript,  // �m�[�h�X�N���v�g�p
		Max,         // �E�B���h�E�̐�
	};

	/// <summary>
	/// �E�B���h�E�Ǘ��p
	/// </summary>
	namespace Window
	{
		/// <summary>
		/// �S�ẴE�B���h�E�I�u�W�F�N�g���쐬����
		/// </summary>
		/// <returns>
		/// <para> true : �S�ẴE�B���h�E�I�u�W�F�N�g�̍쐬�ɐ��� </para> 
		/// <para> false : �����ꂩ�̃E�B���h�E�I�u�W�F�N�g�̍쐬�Ɏ��s </para> 
		/// </returns>
		bool CreateAllWindow();

		/// <summary>
		/// �E�B���h�E���J��
		/// </summary>
		/// <param name="windowID"> �E�B���h�E���ʔԍ� </param>
		/// <param name="windowTitle"> �E�B���h�E�^�C�g�� </param>
		void OpenWindow(WindowID windowID, const char* windowTitle);

		/// <summary>
		/// �E�B���h�E�����
		/// </summary>
		/// <param name="windowID"> �E�B���h�E���ʔԍ� </param>
		void CloseWindow(WindowID windowID);

		/// <summary>
		/// �E�B���h�E�̎g�p���J�n����
		/// </summary>
		/// <param name="windowID"> �g�p����E�B���h�E���ʔԍ� </param>
		/// <returns>
		/// <para> true : �w�肵���E�B���h�E���A�N�e�B�u </para>
		/// <para> false : �w�肵���E�B���h�E����A�N�e�B�u </para>
		/// </returns>
		bool Begin(WindowID windowID);

		/// <summary>
		/// �E�B���h�E�̎g�p���I������
		/// </summary>
		void End();

		/// <summary>
		/// �E�B���h�E�����Ă��邩���擾����
		/// </summary>
		/// <param name="windowID"> �E�B���h�E���ʔԍ� </param>
		/// <returns>
		/// <para> true : �E�B���h�E�����Ă��� </para>
		/// <para> false : �E�B���h�E�����Ă��Ȃ� </para>
		/// </returns>
		bool IsClosed(WindowID windowID);

		/// <summary>
		/// �E�B���h�E���擾����
		/// </summary>
		/// <param name="windowID"> �E�B���h�E���ʔԍ� </param>
		/// <returns> �w�肵���E�B���h�E�̃E�B���h�E�I�u�W�F�N�g </returns>
		GLFWwindow& GetWindow(WindowID windowID);

		/// <summary>
		/// GLFW�E�B���h�E�̑傫�����擾����
		/// </summary>
		/// <param name="windowID"> �E�B���h�E���ʔԍ� </param>
		/// <returns> �w�肵���E�B���h�E�̑傫�� </returns>
		Vector2 GetWindowSize(WindowID windowID);

		/// <summary>
		/// GLFW�E�B���h�E�̑傫�����擾����
		/// </summary>
		/// <param name="[in] windowID"> �E�B���h�E���ʔԍ� </param>
		/// <param name="[out] w"> �E�B���h�E�̕� </param>
		/// <param name="[out] h"> �E�B���h�E�̍��� </param>
		void GetWindowSize(WindowID windowID, GLsizei& w, GLsizei& h);

		/// <summary>
		/// �E�B���h�E�̃A�X�y�N�g����擾����
		/// </summary>
		/// <param name="windowID"> �E�B���h�E���ʔԍ� </param>
		/// <returns> �E�B���h�E�̃A�X�y�N�g��</returns>
		float GetAspectRatio(WindowID windowID);

		/// <summary>
		/// ���C�����j�^�[�̏����擾����
		/// </summary>
		/// <returns> ���C�����j�^�[�̏�� </returns>
		const GLFWvidmode& GetMainMonitor();

	} // namespace Window

} // namespace PokarinEngine

#endif // !WINDOW_H_INCLUDED
