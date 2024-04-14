/**
* @file FramebufferObject.h
*/
#ifndef FRAMEBUFFEROBJECT_H_INCLUDED
#define FRAMEBUFFEROBJECT_H_INCLUDED

#include "glad/glad.h"
#include "Window.h"
#include "Debug.h"
#include "Texture.h"

#include "Color.h"

#include <memory>

namespace PokarinEngine
{
	// -------------------
	// 前方宣言
	// -------------------

	class FramebufferObject;
	class RenderbufferObject;
	class Engine;

	// --------------------------
	// 型の別名を定義
	// --------------------------

	using FramebufferObjectPtr = std::shared_ptr<FramebufferObject>;
	using RenderbufferObjectPtr = std::shared_ptr<RenderbufferObject>;

	/// <summary>
	/// FBO(カラーバッファ・深度バッファなどを統括するバッファオブジェクト)
	/// </summary>
	class FramebufferObject
	{
	public: // ------------------ FBO作成 -------------------

		/// <summary>
		/// FBOを作成する
		/// </summary>
		/// <param name="engine"> エンジンクラスの参照 </param>
		/// <param name="_windowID"> ウィンドウ番号 </param>
		/// <param name="fboWidth"> FBOの幅 </param>
		/// <param name="fboHeight"> FBOの高さ </param>
		static FramebufferObjectPtr Create(
			Engine& engine, WindowID _windowID, GLsizei fboWidth, GLsizei fboHeight)
		{
			return std::make_shared<FramebufferObject>(
				engine, _windowID, fboWidth, fboHeight);
		}

	public: // -------- コンストラクタ・デストラクタ --------

		/// <summary>
		/// FBOを作成するコンストラクタ
		/// </summary>
		/// <param name="engine"> エンジンクラスの参照 </param>
		/// <param name="_windowID"> ウィンドウ番号 </param>
		/// <param name="fboWidth"> FBOの幅 </param>
		/// <param name="fboHeight"> FBOの高さ </param>
		FramebufferObject(
			Engine& engine, WindowID _windowID, GLsizei fboWidth, GLsizei fboHeight);

		/// <summary>
		/// FBOを削除するデストラクタ
		/// </summary>
		~FramebufferObject();

	public: // ----------------- 禁止事項 -------------------

		// コピーコンストラクタの禁止
		FramebufferObject(const FramebufferObject&) = delete;

		// 代入の禁止
		FramebufferObject operator=(const FramebufferObject&) = delete;

	public: // --------------- FBOのバインド ----------------

		/// <summary>
		/// FBOをバインドする
		/// </summary>
		void Bind() const;

		/// <summary>
		/// FBOのバインドを解除する
		/// </summary>
		void UnBind() const;

	public: // -------------- カラーバッファ ---------------

		/// <summary>
		/// カラーバッファ用テクスチャをクリア
		/// </summary>
		/// <param name="red"> 赤 </param>
		/// <param name="green"> 緑 </param>
		/// <param name="blue"> 青 </param>
		/// <param name="alpha"> 透明度 </param>
		void ClearColor(const Color& color);

	public: // ----------------- サイズ --------------------

		/// <summary>
		/// FBOの大きさを設定する
		/// </summary>
		/// <param name="fboWidth"> 幅 </param>
		/// <param name="fboHeight"> 高さ </param>
		void SetSize(GLsizei fboWidth, GLsizei fboHeight)
		{
			width = fboWidth;
			height = fboHeight;
		}

	public: // -------------- 管理番号の取得 ---------------

		// 管理番号の取得(GLuint型に変換)
		operator GLuint() const { return id; }

	public: // ------------- テクスチャの取得 --------------

		/// <summary>
		/// 描画後のテクスチャを取得する
		/// </summary>
		/// <returns> カラーバッファ用テクスチャの管理番号 </returns>
		GLuint GetTexture() { return *texture; }

	private: // ------------------- 情報 --------------------

		// FBOの管理番号
		GLuint id = 0;

		// カラーバッファ用テクスチャ
		TexturePtr texture;

		// 深度バッファ用RBO
		RenderbufferObjectPtr rbo;

		// FBOの大きさ
		GLsizei width = 0, height = 0;

		// ウィンドウ識別番号
		WindowID windowID;
	};

} // namespace PokarinEngine

#endif // !FRAMEBUFFEROBJECT_H_INCLUDED
