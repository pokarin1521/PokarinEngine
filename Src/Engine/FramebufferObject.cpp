/**
* FramebufferObject.cpp
*/
#include "FramebufferObject.h"

#include "Engine.h"
#include "Window.h"
#include "Texture.h"
#include "RenderbufferObject.h"

namespace PokarinEngine
{
	/// <summary>
	/// FBOを作成するコンストラクタ
	/// </summary>
	/// <param name="engine"> エンジンクラスの参照 </param>
	/// <param name="_windowID"> ウィンドウ番号 </param>
	/// <param name="fboWidth"> FBOの幅 </param>
	/// <param name="fboHeight"> FBOの高さ </param>
	FramebufferObject::FramebufferObject(
		Engine& engine, WindowID _windowID, GLsizei fboWidth, GLsizei fboHeight)
	{
		// -------------------------------
		// ウィンドウ番号を設定
		// -------------------------------

		windowID = _windowID;

		// -------------------------------
		// FBOの大きさを設定
		// -------------------------------

		width = fboWidth;
		height = fboHeight;

		// -------------------------------------------
		// カラーバッファ用テクスチャを作成
		// -------------------------------------------

		texture = engine.GetTexture(width, height);

		// -------------------------------
		// 深度バッファ用RBOを作成
		// -------------------------------

		// どのGPUでもそれなりの性能になることを期待して
		// 深度バッファはGL_DEPTH_COMPONENT32にする
		rbo = RenderbufferObject::Create(
			width, height, GL_DEPTH_COMPONENT32);

		// -------------------------
		// FBOを作成
		// -------------------------

		// FBOを作成
		glCreateFramebuffers(1, &id);

		// カラーバッファ用テクスチャを割り当てる
		glNamedFramebufferTexture(id, GL_COLOR_ATTACHMENT0, *texture, 0);

		// 深度バッファ用テクスチャを割り当てる
		glNamedFramebufferRenderbuffer(
			id, GL_DEPTH_ATTACHMENT, GL_RENDERBUFFER, *rbo);

		// FBOが正常に作成できたか確認する
		if (glCheckNamedFramebufferStatus(
			id, GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
		{
			LOG_WARNING("FBOの作成に失敗しました");

			// FBO削除
			glDeleteFramebuffers(1, &id);

			// 管理番号を初期化
			id = 0;
		}
	}

	/// <summary>
	/// FBOを削除するデストラクタ
	/// </summary>
	FramebufferObject::~FramebufferObject()
	{
		glDeleteFramebuffers(1, &id);
	}

	/// <summary>
	/// FBOをバインドする
	/// </summary>
	void FramebufferObject::Bind() const
	{
		// FBOをバインド
		glBindFramebuffer(GL_FRAMEBUFFER, id);

		// ビューポートをFBOの大きさで設定
		glViewport(0, 0, width, height);

		// 深度バッファをクリア
		glClear(GL_DEPTH_BUFFER_BIT);
	}

	/// <summary>
	/// FBOのバインドを解除する
	/// </summary>
	void FramebufferObject::UnBind() const
	{
		// FBOのバインド解除
		glBindFramebuffer(GL_FRAMEBUFFER, 0);

		// OpenGLウィンドウの大きさを取得
		GLsizei windowWidth = 0, windowHeight = 0;
		Window::GetWindowSize(windowID, windowWidth, windowHeight);

		// ビューポートをOpenGLウィンドウの大きさに戻す
		glViewport(0, 0, windowWidth, windowHeight);
	}

	/// <summary>
	/// カラーバッファ用テクスチャをクリア
	/// </summary>
	void FramebufferObject::ClearColor(const Color& color)
	{
		glBindFramebuffer(GL_FRAMEBUFFER, id);

		// バックバッファをクリアするときの色を指定
		glClearColor(color.r, color.g, color.b, color.a);

		// バックバッファをクリア
		// 今回はカラーバッファと深度バッファを指定
		glClear(GL_COLOR_BUFFER_BIT);

		glBindFramebuffer(GL_FRAMEBUFFER, 0);
	}

} // namespace PokarinEngine