/**
* @file JsonFile.cpp
*/
#include "JsonFile.h"

#include "Json/Json.h"

#include <fstream>

namespace PokarinEngine
{
	namespace JsonFile
	{
		/// <summary>
		/// Jsonファイルに保存する
		/// </summary>
		/// <param name="[in] fileName"> 保存先のファイル名 </param>
		/// <param name="[in] json"> 情報を格納しているJson型 </param>
		void Save(const std::string& fileName, const Json& json)
		{
			// 保存先ファイル
			std::ofstream file;

			// 上書きモードで開く
			file.open(fileName, std::ios::out);

			// ファイルに保存
			// ファイルが無かったら生成される
			file << json.dump(2);

			// ファイルを閉じる
			file.close();
		}

		/// <summary>
		/// Jsonファイルを読み込む
		/// </summary>
		/// <param name="[in] fileName"> 読み込むファイル名 </param>
		/// <param name="[out] json"> 情報を格納するJson型 </param>
		void Load(const std::string& fileName, Json& json)
		{
			// 読み込み先のファイル
			std::ifstream file;

			// 読み取り専用で開く
			file.open(fileName, std::ios::in);

			// ファイルが開けたら読み込む
			if (file)
			{
				file >> json;
			}

			// 開いたファイルを閉じる
			file.close();
		}
	}
}