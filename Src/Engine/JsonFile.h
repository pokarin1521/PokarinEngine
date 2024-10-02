/**
* @file JsonFile.h
*/
#ifndef POKARINENGINE_JSONFILE_H_INCLUDED
#define POKARINENGINE_JSONFILE_H_INCLUDED

#include "Json/UsingNameJson.h"

#include <string>

namespace PokarinEngine
{
	/// <summary>
	/// Jsonファイル用
	/// </summary>
	namespace JsonFile
	{
		/// <summary>
		/// Jsonファイルに保存する
		/// </summary>
		/// <param name="[in] fileName"> 保存先のファイル名 </param>
		/// <param name="[in] json"> 情報を格納しているJson型 </param>
		void Save(const std::string& fileName, const Json& json);

		/// <summary>
		/// Jsonファイルを読み込む
		/// </summary>
		/// <param name="[in] fileName"> 読み込むファイル名 </param>
		/// <param name="[out] json"> 情報を格納するJson型 </param>
		void Load(const std::string& fileName, Json& json);
	}
}

#endif // !POKARINENGINE_JSONFILE_H_INCLUDED
