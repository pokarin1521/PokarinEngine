/**
* @file ComponentAdder.h
*/
#ifndef POKARINENGINE_COMPONENTADDER_H_INCLUDED
#define POKARINENGINE_COMPONENTADDER_H_INCLUDED

#include "../UsingNames/UsingComponent.h"
#include "../UsingNames/UsingGameObject.h"

#include <string>
#include <typeindex>
#include <unordered_map>
#include <cassert>
#include <functional>

namespace PokarinEngine
{
	/// <summary>
	/// コンポーネント追加に関する処理用
	/// </summary>
	class ComponentAdder
	{
	public: // --------------- コンストラクタ・デストラクタ ----------------

		ComponentAdder() = default;
		~ComponentAdder() = default;

	public: // ---------------------------- 取得 ---------------------------

		/// <summary>
		/// コンポーネントの名前を取得する
		/// </summary>
		/// <typeparam name="T"> コンポーネント </typeparam>
		/// <returns> コンポーネントの名前 </returns>
		template <class T>
		static std::string GetName()
		{
			// コンポーネント以外が指定されたらエラーを出す
			// 実行前に知りたいので、static_assert
			static_assert(std::is_base_of_v<Component, T>);

			// 取得したコンポーネント名を返す
			return nameList[typeid(T)];
		}

		/// <summary>
		/// ゲームオブジェクトにコンポーネントを追加する
		/// </summary>
		/// <param name="[in] name"> 追加するコンポーネントの名前 </param>
		/// <param name="[out] gameObject"> ゲームオブジェクト </param>
		/// <returns> 追加したコンポーネント </returns>
		static ComponentPtr AddComponent(const std::string& name, GameObject& gameObject);

		/// <summary>
		/// コンポーネント追加用リストを表示する
		/// </summary>
		/// <param name="[out] gameObject"> ゲームオブジェクト </param>
		static void RenderList(GameObjectPtr gameObject);

	private: // ------------------- コンポーネントの名前 -------------------

		// <型(typeid), コンポーネントの名前>
		using NameList = std::unordered_map<std::type_index, std::string>;

		// <コンポーネントの名前, コンポーネント追加用関数>
		using FunctionList = std::unordered_map<
			std::string, std::function<ComponentPtr(GameObject&)>>;

		// 名前判別用
		// <型(typeid), コンポーネントの名前>
		static NameList nameList;

		// コンポーネント追加用関数の配列
		// <コンポーネントの名前, コンポーネント追加用関数>
		static FunctionList functionList;
	};
}

#endif // !POKARINENGINE_COMPONENTADDER_H_INCLUDED
