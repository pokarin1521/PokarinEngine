/**
* @file SceneView.cpp
*/
#include "RenderView.h"

#include "../Engine.h"
#include "../Window.h"

namespace PokarinEngine
{
	/// <summary>
	/// 初期化
	/// </summary>
	/// <param name="[in] engine"> エンジンクラスの参照 </param> 
	void RenderView::Initialize(Engine& engine)
	{
		// ---------------------------
		// FBOを作成
		// ---------------------------

		// GLFWウィンドウの大きさを指定
		GLsizei w = 0, h = 0;
		Window::GetWindowSize(WindowID::Main, w, h);

		// FBOを作成
		fbo = FramebufferObject::Create(engine, WindowID::Main, w, h);
	}

} // namespace PokarinEngine