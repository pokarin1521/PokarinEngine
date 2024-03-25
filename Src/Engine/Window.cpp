/**
* @file Window.cpp
*/
#include "Window.h"

#include "ImGui/imgui_impl_glfw.h"
#include "ImGui/imgui_impl_opengl3.h"

#include "Debug.h"

#include <string>
#include <vector>
#include <unordered_map>

namespace PokarinEngine
{
	namespace Window
	{
		// ------------------------------------
		// �E�B���h�E�I�u�W�F�N�g
		// ------------------------------------

		// �E�B���h�E�Ǘ��p�z��<�E�B���h�E���ʔԍ�, �E�B���h�E�I�u�W�F�N�g>
		std::unordered_map<WindowID, GLFWwindow*> windowList;

		// ------------------------------------
		// �֐�
		// ------------------------------------

		/// �����ł����g��Ȃ��̂ŁAcpp�݂̂ɏ���
		/// <summary>
		/// �E�B���h�E���쐬����Ă��邩���擾����
		/// </summary>
		/// <param name="windowID"> �E�B���h�E���ʔԍ� </param>
		/// <returns>
		/// <para> true : �w�肵���ԍ��̃E�B���h�E���쐬����Ă��� </para>
		/// <para> false : �w�肵���ԍ��̃E�B���h�E���쐬����Ă��Ȃ� </para>
		/// </returns>
		bool IsCreated(WindowID windowID)
		{
			return windowList.find(windowID) != windowList.end();
		}

		/// �����ł����g��Ȃ��̂ŁAcpp�݂̂ɏ���
		/// <summary>
		/// �E�B���h�E�I�u�W�F�N�g���쐬����
		/// </summary>
		/// <param name="windowID"> �E�B���h�E���ʔԍ� </param>
		/// <param name="windowTitle"> �E�B���h�E�^�C�g�� </param>
		/// <returns>
		/// <para> true : �E�B���h�E�I�u�W�F�N�g�̍쐬�ɐ����A�܂��͍쐬�ς� </para> 
		/// <para> false : �E�B���h�E�I�u�W�F�N�g�̍쐬�Ɏ��s </para> 
		/// </returns>
		bool CreateWindow(WindowID windowID)
		{
			// ���ɃE�B���h�E������Ȃ炻���Ԃ�
			if (IsCreated(windowID))
			{
				return true;
			}

			// -----------------------------
			// �f�o�b�O�R���e�L�X�g�̍쐬
			// -----------------------------

			glfwWindowHint(GLFW_OPENGL_DEBUG_CONTEXT, GLFW_TRUE);

			// ----------------------------------------
			// �E�B���h�E���t���X�N���[���ɐݒ�
			// ----------------------------------------

			glfwWindowHint(GLFW_MAXIMIZED, GL_TRUE);

			// ----------------------------------------
			// �E�B���h�E���\���ɐݒ�
			// ----------------------------------------

			// �K�v�ɉ����ĕ\�����������̂�
			// �ŏ��͔�\���ɂ��Ă���
			glfwWindowHint(GLFW_VISIBLE, GL_FALSE);

			// ------------------------
			// �`��E�B���h�E�̍쐬
			// ------------------------

			/* GLFW�E�B���h�E�I�u�W�F�N�g�́A
			GLFW���쐬�����E�B���h�E�̃f�[�^���Ǘ�����I�u�W�F�N�g

			���̃I�u�W�F�N�g��ʂ��āA
			�E�B���h�E�̈ʒu��T�C�Y�A�L�[���́A�}�E�X���͂��擾���� */

			// ���C�����j�^�[�̏��
			const GLFWvidmode mainMonitor = GetMainMonitor();

			// �E�B���h�E�I�u�W�F�N�g
			GLFWwindow* window = nullptr;

			// GLFW�E�B���h�E���쐬
			// �E�B���h�E�T�C�Y�ɂ̓��C�����j�^�[�̃T�C�Y���w�肵�Ă���
			window = glfwCreateWindow(mainMonitor.width, mainMonitor.height,
				"OpenGL Window", nullptr, nullptr);

			// �E�B���h�E�̍쐬�Ɏ��s
			if (!window)
			{
				glfwTerminate();
				return false;
			}

			// �E�B���h�E�Ǘ��p�z��ɒǉ�
			windowList.emplace(windowID, window);

			return true;
		}

		/// <summary>
		/// �S�ẴE�B���h�E�I�u�W�F�N�g���쐬����
		/// </summary>
		/// <returns>
		/// <para> true : �S�ẴE�B���h�E�I�u�W�F�N�g�̍쐬�ɐ��� </para> 
		/// <para> false : �����ꂩ�̃E�B���h�E�I�u�W�F�N�g�̍쐬�Ɏ��s </para> 
		/// </returns>
		bool CreateAllWindow()
		{
			// �쐬�\�ȃE�B���h�E�̐�
			int windowMax = static_cast<int>(WindowID::Max);

			// �S�ẴE�B���h�E�I�u�W�F�N�g���쐬
			for (int i = 0; i < windowMax; ++i)
			{
				WindowID windowID = WindowID(i);

				// �쐬�Ɏ��s�����̂Œ��f
				if (!CreateWindow(windowID))
				{
					LOG_ERROR("%d�Ԃ̃E�B���h�E�I�u�W�F�N�g���쐬�ł��܂���ł���", i);
					return false;
				}
			}

			// �쐬�ɐ���
			return true;
		}

		/// <summary>
		/// �E�B���h�E���J��
		/// </summary>
		/// <param name="windowID"> �E�B���h�E���ʔԍ� </param>
		/// <param name="windowTitle"> �E�B���h�E�^�C�g�� </param>
		void OpenWindow(WindowID windowID, const char* windowTitle)
		{
			// �E�B���h�E�I�u�W�F�N�g���Ȃ��̂Œ��f
			if (!IsCreated(windowID))
			{
				return;
			}

			// �E�B���h�E�I�u�W�F�N�g
			GLFWwindow* window = &GetWindow(windowID);

			// �E�B���h�E�^�C�g����ݒ�
			glfwSetWindowTitle(window, windowTitle);

			// �E�B���h�E��\��
			glfwShowWindow(window);

			// �E�B���h�E���ŏ�������Ă���Ȃ�
			// ���̑傫���ɖ߂�
			if (glfwGetWindowAttrib(window, GLFW_ICONIFIED))
			{
				glfwRestoreWindow(window);
			}

			// �E�B���h�E�����Ȃ��悤�ɐݒ�
			glfwSetWindowShouldClose(window, false);
		}

		/// <summary>
		/// �E�B���h�E�����
		/// </summary>
		/// <param name="windowID"> �E�B���h�E���ʔԍ� </param>
		void CloseWindow(WindowID windowID)
		{
			// �w�肵���E�B���h�E�������ꍇ�͉������Ȃ�
			if (!IsCreated(windowID))
			{
				return;
			}

			glfwSetWindowShouldClose(windowList[windowID], true);
		}

		/// <summary>
		/// �E�B���h�E�̎g�p���J�n����
		/// </summary>
		/// <param name="windowID"> �g�p����E�B���h�E���ʔԍ� </param>
		/// <returns>
		/// <para> true : �w�肵���E�B���h�E���A�N�e�B�u </para>
		/// <para> false : �w�肵���E�B���h�E����A�N�e�B�u </para>
		/// </returns>
		bool Begin(WindowID windowID)
		{
			// �E�B���h�E���Ȃ��̂Œ��f
			if (!IsCreated(windowID))
			{
				return false;
			}

			// �E�B���h�E
			GLFWwindow* window = windowList[windowID];

			// �g�p����E�B���h�E��ݒ�
			glfwMakeContextCurrent(windowList[windowID]);

			// OS����̗v������������
			// �L�[�{�[�h��}�E�X�Ȃǂ̏�Ԃ��擾����ɂ́A����I�ɌĂяo���K�v������
			glfwPollEvents();

			// �E�B���h�E���A�N�e�B�u�ɂȂ�����true
			return glfwGetWindowAttrib(window, GLFW_FOCUSED);
		}

		/// <summary>
		/// �E�B���h�E�̎g�p���I������
		/// </summary>
		void End()
		{
			/* �t���[���o�b�t�@�͊G��`�悷�邽�߂̃�����
			�\�����ƕ`�摤��2��p�ӂ��A�`�悪�I�������\�����ƌ����Ƃ����̂��J��Ԃ�
			���ŕ`�悷�邱�Ƃŕ`�撆�̊G��\������̂�h���ł���
			�\�������t�����g�o�b�t�@�A�`�摤���o�b�N�o�b�t�@�Ƃ��� */

			// ���ݎg�p���̃E�B���h�E
			GLFWwindow* window = glfwGetCurrentContext();

			// �g�p���̃E�B���h�E���Ȃ���Β��f
			if (!window)
			{
				return;
			}

			// �t�����g�o�b�t�@�ƃo�b�N�o�b�t�@�̓���ւ�
			glfwSwapBuffers(window);

			// �E�B���h�E������ꂽ���\���ɂ���
			if (glfwWindowShouldClose(window))
			{
				glfwHideWindow(window);
			}

			// �E�B���h�E�̎g�p���I��
			glfwMakeContextCurrent(nullptr);
		}

		/// <summary>
		/// �E�B���h�E�����Ă��邩���擾����
		/// </summary>
		/// <param name="windowID"> �E�B���h�E���ʔԍ� </param>
		/// <returns>
		/// <para> true : �E�B���h�E�����Ă��� </para>
		/// <para> false : �E�B���h�E�����Ă��Ȃ� </para>
		/// </returns>
		bool IsClosed(WindowID windowID)
		{
			// �E�B���h�E���쐬����Ă��Ȃ��Ȃ�true
			if (!IsCreated(windowID))
			{
				return true;
			}

			// �E�B���h�E����\���Ȃ�true
			return !glfwGetWindowAttrib(&GetWindow(windowID), GLFW_VISIBLE);
		}

		/// <summary>
		/// �E�B���h�E���擾����
		/// </summary>
		/// <param name="windowID"> �E�B���h�E���ʔԍ� </param>
		/// <returns> 
		/// <para> �w�肵���E�B���h�E�̃E�B���h�E�I�u�W�F�N�g </para>
		/// <para> �w�肵���ԍ��̃E�B���h�E�������ꍇ�́A���C���E�B���h�E��Ԃ� </para>
		/// </returns>
		GLFWwindow& GetWindow(WindowID windowID)
		{
			// �w�肵���ԍ��̃E�B���h�E�������ꍇ��
			// ���C���E�B���h�E��Ԃ�
			if (!IsCreated(windowID))
			{
				// ���C���E�B���h�E�������ꍇ�͍쐬����
				if (!IsCreated(WindowID::Main))
				{
					CreateWindow(WindowID::Main);
				}

				return *windowList[WindowID::Main];
			}

			// �w�肵���ԍ��̃E�B���h�E
			return *windowList[windowID];
		}

		/// <summary>
		/// GLFW�E�B���h�E�̑傫�����擾����
		/// </summary>
		/// <param name="windowID"> �E�B���h�E���ʔԍ� </param>
		/// <returns> �w�肵���E�B���h�E�̑傫�� </returns>
		Vec2 GetWindowSize(WindowID windowID)
		{
			// �w�肵���E�B���h�E�������ꍇ��0��Ԃ�
			if (!IsCreated(windowID))
			{
				return { 0, 0 };
			}

			// �t���[���o�b�t�@�̑傫�����擾
			int w, h;
			glfwGetFramebufferSize(windowList[windowID], &w, &h);

			// vec2�^�ŕԂ�
			return { static_cast<float>(w), static_cast<float>(h) };
		}

		/// <summary>
		/// GLFW�E�B���h�E�̑傫�����擾����
		/// </summary>
		/// <param name="[in] windowID"> �E�B���h�E���ʔԍ� </param>
		/// <param name="[out] w"> �E�B���h�E�̕� </param>
		/// <param name="[out] h"> �E�B���h�E�̍��� </param>
		void GetWindowSize(WindowID windowID, GLsizei& w, GLsizei& h)
		{
			if (IsCreated(windowID))
			{
				glfwGetFramebufferSize(windowList[windowID], &w, &h);
			}
		}

		/// <summary>
		/// �E�B���h�E�̃A�X�y�N�g����擾����
		/// </summary>
		/// <param name="windowID"> �E�B���h�E���ʔԍ� </param>
		/// <returns> �E�B���h�E�̃A�X�y�N�g��</returns>
		float GetAspectRatio(WindowID windowID)
		{
			// �t���[���o�b�t�@�̑傫�����擾
			const Vec2 size = GetWindowSize(windowID);

			// 0���Z�͔�����
			if (size.y == 0)
			{
				return 0;
			}

			// �A�X�y�N�g���Ԃ�
			return size.x / size.y;
		}

		/// <summary>
		/// ���C�����j�^�[�̏����擾����
		/// </summary>
		/// <returns> ���C�����j�^�[�̏�� </returns>
		const GLFWvidmode& GetMainMonitor()
		{
			return *glfwGetVideoMode(glfwGetPrimaryMonitor());
		}

	} // namespace Window

} // namespace PokarinEngine