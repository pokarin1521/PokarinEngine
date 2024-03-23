/**
* @file Texture.h
*/
#ifndef TEXTURE_H_INCLUDED
#define TEXTURE_H_INCLUDED

#include "glad/glad.h"

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

	/**
	* テクスチャ管理クラス
	*/
	class Texture
	{
		// Engineクラスに情報を公開
		friend class Engine;

	public: // ----------------- 禁止事項 -------------------

		// コピーコンストラクタの禁止
		Texture(const Texture&) = delete;

		// コピー代入の禁止
		Texture& operator=(const Texture&) = delete;

	public: // ----------------- キャスト -------------------

		// GLuint型に変換(管理番号を返す)
		operator GLuint() const { return id; }

	public: // ---------- テクスチャの情報を取得 ------------

		/// <summary>
		/// テクスチャの幅を取得
		/// </summary>
		int GetWidth() const { return width; }

		/// <summary>
		/// テクスチャの高さを取得
		/// </summary>
		int GetHeight() const { return fboHeight; }

		/// <summary>
		/// 画像のアスペクト比を取得
		/// </summary>
		float GetAspectRatio() const
		{
			return static_cast<float>(width) / static_cast<float>(fboHeight);
		}

		/// <summary>
		/// テクスチャの名前を取得
		/// </summary>
		const std::string& GetName() const { return name; }

	private: // ---------- コンストラクタ・デストラクタ -----------

		/* キャッシュを有効にするために
		コンストラクタとデストラクタをプライベートメンバにして
		勝手に生成や削除ができないようにする */

		/// <summary>
		/// テクスチャを読み込むコンストラクタ
		/// (暗黙的キャストを禁止)
		/// </summary>
		/// <param name="filename"> 読み込むテクスチャファイルの名前 </param>
		explicit Texture(const char* filename);

		/// <summary>
		/// テクスチャを作成するコンストラクタ
		/// (暗黙的キャストを禁止)
		/// </summary>
		/// <param name="width"> 幅 </param>
		/// <param name="height"> 高さ </param>
		explicit Texture(GLsizei w, GLsizei h);

		/// <summary>
		/// テクスチャを削除するデストラクタ
		/// </summary>
		~Texture();

	private: // -------------- テクスチャの情報 ---------------

		std::string name;	// テクスチャ名(主にデバッグ用)
		GLuint id = 0;		// テクスチャオブジェクト管理番号
		int width = 0;		// テクスチャの幅
		int fboHeight = 0;		// テクスチャの高さ
	};

} // namespace PokarinEngine

#endif // !TEXTURE_H_INCLUDED
