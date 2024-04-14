/**
* @file RenderbufferObject.cpp
*/
#include "RenderbufferObject.h"

namespace PokarinEngine
{
	/// <summary>
	/// RBOを作成する
	/// </summary>
	/// <param name="width"> 幅 </param>
	/// <param name="height"> 高さ </param>
	/// <param name="internalformat"> 保持する情報のバッファ </param>
	RenderbufferObject::RenderbufferObject(
		GLsizei width, GLsizei height, GLenum internalformat)
	{
		// --------------------------------------------
		// 深度バッファ用レンダーバッファを作成
		// --------------------------------------------

		glCreateRenderbuffers(1, &id);

		// ---------------------------------------
		// 情報を保持できるように紐づける
		// ---------------------------------------

		// 設定できるようにバインド
		glBindRenderbuffer(GL_RENDERBUFFER, id);

		// 指定された情報を保持できるように紐づけ
		glRenderbufferStorage(
			GL_RENDERBUFFER, internalformat, width, height);

		// 誤操作のないようにバインド解除
		glBindRenderbuffer(GL_RENDERBUFFER, 0);
	}

	/// <summary>
	/// RBOを削除するデストラクタ
	/// </summary>
	RenderbufferObject::~RenderbufferObject()
	{
		glDeleteRenderbuffers(1, &id);
	}

} // namespace PokarinEngine