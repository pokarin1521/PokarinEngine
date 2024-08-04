/**
* @file FramebufferObject.h
*/
#ifndef POKARINENGINE_FRAMEBUFFEROBJECT_H_INCLUDED
#define POKARINENGINE_FRAMEBUFFEROBJECT_H_INCLUDED

#include "glad/glad.h"

#include "Window.h"
#include "Debug.h"
#include "TextureGetter.h"

#include "Color.h"

#include <memory>

namespace PokarinEngine
{
	// -------------------
	// 前方宣言
	// -------------------

	class FramebufferObject;
	class RenderbufferObject;

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
		static FramebufferObjectPtr Create()
		{
			return std::make_shared<FramebufferObject>();
		}

	public: // -------- コンストラクタ・デストラクタ --------

		/// <summary>
		/// FBOを作成するコンストラクタ
		/// </summary>
		FramebufferObject();

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
		/// <param name="[in] Color"> クリア色 </param>
		void ClearColor(const Color& color);

	public: // ----------------- サイズ --------------------

		/// <summary>
		/// FBOの大きさを設定する
		/// </summary>
		/// <param name="[in] fboWidth"> 幅 </param>
		/// <param name="[in] fboHeight"> 高さ </param>
		void SetSize(GLsizei fboWidth, GLsizei fboHeight)
		{
			width = fboWidth;
			height = fboHeight;
		}

	public: // -------------- 識別番号の取得 ---------------

		// 識別番号の取得(GLuint型に変換)
		operator GLuint() const { return id; }

	public: // ------------- テクスチャの取得 --------------

		/// <summary>
		/// 描画後のテクスチャの識別番号を取得する
		/// </summary>
		/// <returns> カラーバッファ用テクスチャの識別番号 </returns>
		GLuint GetTextureID() const { return *texture; }

	private: // ------------------- 情報 --------------------

		// FBOの識別番号
		GLuint id = 0;

		// カラーバッファ用テクスチャ
		TexturePtr texture;

		// 深度バッファ用RBO
		RenderbufferObjectPtr rbo;

		// FBOの大きさ
		// 16 : 9 で設定
		GLsizei width = 1280, height = 720;
	};

} // namespace PokarinEngine

#endif // !POKARINENGINE_FRAMEBUFFEROBJECT_H_INCLUDED
