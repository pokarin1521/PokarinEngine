/**
* @file RenderbufferObject.h
*/
#ifndef POKARINENGINE_RENDERBUFFEROBJECT_H_INCLUDED
#define POKARINENGINE_RENDERBUFFEROBJECT_H_INCLUDED

#include "glad/glad.h"

#include <memory>

namespace PokarinEngine
{
	// ----------------------
	// 前方宣言
	// ----------------------

	class RenderbufferObject;

	// ------------------------------
	// 型の別名を定義
	// ------------------------------

	using RenderbufferObjectPtr = std::shared_ptr<RenderbufferObject>;

	/// <summary>
	/// RBO(レンダリングされた結果を保持するバッファオブジェクト)
	/// </summary>
	class RenderbufferObject
	{
	public: // ------------------- RBOを作成 ---------------------

		/// <summary>
		/// RBOを作成する
		/// </summary>
		/// <param name="[in] width"> 幅 </param>
		/// <param name="[in] height"> 高さ </param>
		/// <param name="[in] internalformat"> 保持する情報のバッファ </param>
		/// <returns> 作成したRBO </returns>
		static RenderbufferObjectPtr Create(
			GLsizei width, GLsizei height, GLenum internalformat)
		{
			return std::make_shared<RenderbufferObject>(
				width, height, internalformat);
		}

	public: // ---------- コンストラクタ・デストラクタ -----------

		/// <summary>
		/// RBOを作成する
		/// </summary>
		/// <param name="[in] width"> 幅 </param>
		/// <param name="[in] height"> 高さ </param>
		/// <param name="[in] internalformat"> 保持する情報のバッファ </param>
		RenderbufferObject(
			GLsizei width, GLsizei height, GLenum internalformat);

		/// <summary>
		/// RBOを削除するデストラクタ
		/// </summary>
		~RenderbufferObject();

	public: // -------------------- 禁止事項 ---------------------

		// コピーコンストラクタの禁止
		RenderbufferObject(const RenderbufferObject&) = delete;

		// 代入の禁止
		RenderbufferObject operator=(const RenderbufferObject&) = delete;

	public: // ---------------------- 変換 -----------------------

		// GLuint型に変換(管理番号を返す)
		operator GLuint() const { return id; }

	private: // --------------------- 情報 -----------------------

		// RBOの管理番号
		GLuint id = 0;
	};

} // namespace PokarinEngine

#endif // !POKARINENGINE_RENDERBUFFEROBJECT_H_INCLUDED
