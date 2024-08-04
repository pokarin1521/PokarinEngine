/**
* @file TextureGetter.cpp
*/
#include "TextureGetter.h"

#include <unordered_map>

namespace PokarinEngine
{
	/// <summary>
	/// テクスチャを取得する
	/// </summary>
	/// <param name="fileName"> ファイル名 </param>
	/// <returns> ファイル名が一致するテクスチャ </returns>
	TexturePtr TextureGetter::Get(const std::string& fileName)
	{
		// ----------------------------------------
		// キャッシュにあれば
		// キャッシュされたテクスチャを返す
		// ----------------------------------------
		
		// テクスチャを検索
		// なければendが入る
		auto itr = textureCache.find(fileName);

		// テクスチャが見つかった場合
		if (itr != textureCache.end())
		{
			// キャッシュされたテクスチャを返す
			return itr->second;
		}

		// --------------------------------------------
		// キャッシュになければ
		// テクスチャを読み込んでキャッシュに登録
		// --------------------------------------------

		/* make_shared関数はEngineクラスのメンバ関数ではない
		なので、補助クラスを作成して
		間接的にコンストラクタ、デストラクタを呼び出す */

		// テクスチャ
		// ファイルからテクスチャを読み込む
		TexturePtr tex = std::make_shared<TexHelper>(fileName);

		// テクスチャをキャッシュに登録
		textureCache.emplace(fileName, tex);

		// テクスチャを返す
		return tex;
	}

	/// <summary>
	/// テクスチャを取得する
	/// </summary>
	/// <param name="[in] width"> 幅 </param>
	/// <param name="[in] height"> 高さ </param>
	/// <returns> 指定した大きさのテクスチャ </returns>
	TexturePtr TextureGetter::Get(GLsizei width, GLsizei height)
	{
		/* make_shared関数はEngineクラスのメンバ関数ではない
		なので、補助クラスを作成して
		間接的にコンストラクタ、デストラクタを呼び出す */

		// テクスチャ
		std::shared_ptr<TexHelper> tex;

		// テクスチャを作成
		tex = std::make_shared<TexHelper>(width, height);

		// 作成したテクスチャを返す
		return tex;
	}
}