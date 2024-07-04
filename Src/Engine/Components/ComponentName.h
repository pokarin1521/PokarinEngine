/**
* @file ComponentName.h
*/
#ifndef COMPONENTNAME_H_INCLUDED
#define COMPONENTNAME_H_INCLUDED

#include <string>
#include <typeindex>
#include <unordered_map>
#include <cassert>

namespace PokarinEngine
{
	// ---------------------
	// 前方宣言
	// ---------------------

	class Component;

	/// <summary>
	/// コンポーネントの名前管理用
	/// </summary>
	class ComponentName
	{
	public: // --------------- コンストラクタ・デストラクタ ----------------

		ComponentName() = default;
		~ComponentName() = default;

	public: // ---------------------------- 取得 ---------------------------

		/// <summary>
		/// コンポーネントの名前を取得する
		/// </summary>
		/// <typeparam name="T"> コンポーネント </typeparam>
		/// <returns> コンポーネントの名前 </returns>
		template <class T>
		static std::string Get()
		{
			// コンポーネント以外が指定されたらエラーを出す
			// 実行前に知りたいので、static_assert
			static_assert(std::is_base_of_v<Component, T>);

			// 取得したコンポーネント名を返す
			return nameList[typeid(T)];
		}

	private: // ------------------- コンポーネントの名前 -------------------

		// <型(typeid), コンポーネントの名前>
		using NameList = std::unordered_map<std::type_index, std::string>;

		// <コンポーネントの名前, 型(typeid)>
		using TypeList = std::unordered_map<std::string, std::type_index>;

		// 名前判別用
		// <型(typeid), コンポーネントの名前>
		static NameList nameList;

		// 型判別用
		// <コンポーネントの名前, 型(typeid)>
		static TypeList typeList;
	};
}

#endif // !COMPONENTNAME_H_INCLUDED
