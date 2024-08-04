/**
* @file TextureGetter.h
*/
#ifndef POKARINENGINE_TEXTUREGETTER_H_INCLUDED
#define POKARINENGINE_TEXTUREGETTER_H_INCLUDED

#include "Texture.h"

#include <unordered_map>

namespace PokarinEngine
{
	/// <summary>
	/// テクスチャ取得用クラス
	/// </summary>
	class TextureGetter
	{
	public: // ----------------------- 禁止事項 -----------------------

		// コピーコンストラクタの禁止
		TextureGetter(const TextureGetter&) = delete;

		// 代入の禁止
		TextureGetter& operator=(const TextureGetter&) = delete;

	public: // ------------------- テクスチャの取得 -------------------

		/// <summary>
		/// テクスチャを取得する
		/// </summary>
		/// <param name="[in] fileName"> ファイル名 </param>
		/// <returns> ファイル名が一致するテクスチャ </returns>
		static TexturePtr Get(const std::string& fileName);

		/// <summary>
		/// テクスチャを取得する
		/// </summary>
		/// <param name="[in] width"> 幅 </param>
		/// <param name="[in] height"> 高さ </param>
		/// <returns> 指定した大きさのテクスチャ </returns>
		static TexturePtr Get(GLsizei width, GLsizei height);

	private: // ------------ コンストラクタ・デストラクタ -------------

		/* static関数のみ使用できるように、インスタンス化できないようにする */

		TextureGetter() = default;
		~TextureGetter() = default;

	private: // ------------------ テクスチャ作成用 -------------------

		// コンストラクタ、デストラクタを
		// 呼べるようにするための補助クラス
		struct TexHelper : public Texture
		{
			TexHelper(const char* fileName) : Texture(fileName) {}
			TexHelper(GLsizei w, GLsizei h) : Texture(w, h) {}
		};

	private: // --------------------- キャッシュ ----------------------

		// テクスチャ用キャッシュ
		// <テクスチャのファイル名, テクスチャ>
		inline static std::unordered_map<std::string, TexturePtr> textureCache;
	};
}

#endif // !POKARINENGINE_TEXTUREGETTER_H_INCLUDED
