/**
* @file Texture.h
*/
#ifndef POKARINENGINE_TEXTURE_H_INCLUDED
#define POKARINENGINE_TEXTURE_H_INCLUDED

#include "glad/glad.h"
#include "ImGui/imgui.h"

#include <string>
#include <memory>

namespace PokarinEngine
{
	// --------------------- 
	// 前方宣言
	// --------------------- 

	class Texture;

	// ----------------------
	// 型の別名を定義
	// ----------------------

	// テクスチャポインタ型
	using TexturePtr = std::shared_ptr<Texture>;

	/// <summary>
	/// テクスチャ管理クラス
	/// </summary>
	class Texture
	{
	public: // --------- コンストラクタ・デストラクタ ----------

		/// <summary>
		/// テクスチャ作成用コンストラクタ
		/// </summary>
		Texture() { glCreateTextures(GL_TEXTURE_2D, 1, &id); }

		/// <summary>
		/// テクスチャ削除用コンストラクタ
		/// </summary>
		~Texture() { glDeleteTextures(1, &id); }

	public: // ------------------- 禁止事項 --------------------

		/* 削除用デストラクタを複数回呼ばれないように禁止する */

		Texture(const Texture&) = delete;
		Texture& operator=(const Texture&) = delete;

	public: // --------------------- 変換 ----------------------

		// GLuint型に変換(識別番号を返す)
		operator GLuint() const { return id; }

		// ImTextureID型に変換(識別番号を返す)
		operator ImTextureID() const { return (void*)(std::intptr_t)id; }

	private: // -------------------- 情報 ----------------------

		// 識別番号
		GLuint id = 0;
	};

} // namespace PokarinEngine

#endif // !POKARINENGINE_TEXTURE_H_INCLUDED
