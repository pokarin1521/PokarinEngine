/**
* @file SceneView.cpp
*/
#include "RenderView.h"

#include "../Engine.h"
#include "../Window.h"

namespace PokarinEngine
{
	/// <summary>
	/// ������
	/// </summary>
	/// <param name="[in] engine"> �G���W���N���X�̎Q�� </param> 
	void RenderView::Initialize(Engine& engine)
	{
		// ---------------------------
		// FBO���쐬
		// ---------------------------

		// GLFW�E�B���h�E�̑傫�����w��
		GLsizei w = 0, h = 0;
		Window::GetWindowSize(WindowID::Main, w, h);

		// FBO���쐬
		fbo = FramebufferObject::Create(engine, WindowID::Main, w, h);
	}

} // namespace PokarinEngine