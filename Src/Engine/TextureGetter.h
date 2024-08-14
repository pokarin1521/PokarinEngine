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

		/* static関数でアクセスしてほしいので、禁止する */

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

		/* static関数でアクセスしてほしいので、インスタンス化できないようにする */

		TextureGetter() = default;
		~TextureGetter() = default;

	private: // ----------------- テクスチャ読み込み ------------------

		/// <summary>
		/// テクスチャを読み込む
		/// </summary>
		/// <param name="[in] fileName"> テクスチャのファイル名 </param>
		/// <returns> 読み込んだテクスチャ </returns>
		static TexturePtr LoadTexture(const std::string& fileName);

	private: // --------------------- キャッシュ ----------------------

		// テクスチャ用キャッシュ
		// <テクスチャのファイル名, テクスチャ>
		inline static std::unordered_map<std::string, TexturePtr> textureCache;
	};
}

#endif // !POKARINENGINE_TEXTUREGETTER_H_INCLUDED
