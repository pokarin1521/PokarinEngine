/**
* @file Json.h
*/
#ifndef JSON_H_INCLUDED
#define JSON_H_INCLUDED

#include "nlohmann/json.hpp"

#include <string>
#include <fstream>

namespace PokarinEngine
{
	/// <summary>
	/// Jsonファイル用
	/// </summary>
	namespace JsonFile
	{
		/// <summary>
		/// Jsonファイルを読み込む
		/// </summary>
		/// <param name="[in] fileName"> 読み込むファイル名 </param>
		/// <param name="[out] data"> 読み込んだデータ </param>
		inline void Load(const std::string& fileName, Json& data)
		{
			// 読み込み先のファイル
			std::ifstream file;

			// 読み取り専用で開く
			file.open(fileName, std::ios::in);
			
			// ファイルが開けたら読み込む
			if (file)
			{
				file >> data;
			}

			// 開いたファイルを閉じる
			file.close();
		}

		/// <summary>
		/// Jsonファイルに保存する
		/// </summary>
		/// <param name="[in] fileName"> 保存先のファイル名 </param>
		/// <param name="[in] data"> データ </param>
		inline void Save(const std::string& fileName, const Json& data)
		{
			// 保存先ファイル
			std::ofstream file;

			// 上書きモードで開く
			file.open(fileName, std::ios::out);

			// ファイルに保存
			// ファイルが無かったら生成される
			file << data.dump(2);

			// ファイルを閉じる
			file.close();
		}
	}
}

#endif // !JSON_H_INCLUDED
