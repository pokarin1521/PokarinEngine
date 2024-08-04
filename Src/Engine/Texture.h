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
		// テクスチャ取得用クラスに情報を公開
		friend class TextureGetter;

	public: // ----------------- 禁止事項 -------------------

		// コピーコンストラクタの禁止
		Texture(const Texture&) = delete;

		// コピー代入の禁止
		Texture& operator=(const Texture&) = delete;

	public: // ------------------- 変換 ---------------------

		// GLuint型に変換(識別番号を返す)
		operator GLuint() const { return id; }		
		
		// ImTextureID型に変換(識別番号を返す)
		operator ImTextureID() const { return (void*)(std::intptr_t)id; }

	public: // ---------- テクスチャの情報を取得 ------------

		/// <summary>
		/// テクスチャの幅を取得
		/// </summary>
		int GetWidth() const
		{
			return width;
		}

		/// <summary>
		/// テクスチャの高さを取得
		/// </summary>
		int GetHeight() const
		{
			return height;
		}
		
		/// <summary>
		/// 画像のアスペクト比を取得
		/// </summary>
		float GetAspectRatio() const
		{
			return static_cast<float>(width) / static_cast<float>(height);
		}

		/// <summary>
		/// テクスチャの名前を取得
		/// </summary>
		const std::string& GetName() const
		{
			return name;
		}

	private: // ---------- コンストラクタ・デストラクタ -----------

		/* キャッシュを有効にするために
		コンストラクタとデストラクタをプライベートメンバにして
		勝手に生成や削除ができないようにする */

		/// <summary>
		/// テクスチャを読み込むコンストラクタ
		/// (暗黙的キャストを禁止)
		/// </summary>
		/// <param name="[in] fileName"> 読み込むテクスチャファイルの名前 </param>
		explicit Texture(const char* fileName);

		/// <summary>
		/// テクスチャを作成するコンストラクタ
		/// (暗黙的キャストを禁止)
		/// </summary>
		/// <param name="[in] width"> 幅 </param>
		/// <param name="[in] height"> 高さ </param>
		explicit Texture(GLsizei w, GLsizei h);

		/// <summary>
		/// テクスチャを削除するデストラクタ
		/// </summary>
		~Texture();

	private: // -------------------- 情報 ---------------------

		// テクスチャ名
		std::string name;

		// テクスチャオブジェクトの識別番号
		GLuint id = 0;		
		
		// テクスチャの幅
		int width = 0;	
		
		// テクスチャの高さ
		int height = 0;	
	};

} // namespace PokarinEngine

#endif // !POKARINENGINE_TEXTURE_H_INCLUDED
