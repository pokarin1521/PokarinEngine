/**
* @file ComponentAdder.cpp
*/
#include "ComponentAdder.h"

#include "Colliders/AabbCollider.h"
#include "Colliders/SphereCollider.h"

#include "Camera.h"
#include "Light.h"
#include "Rigidbody.h"
#include "Transform.h"

#include "../GameObject.h"

#include <functional>

namespace PokarinEngine
{
	// -----------------------------
	// 変数
	// -----------------------------

	// 名前判別用
	// <型(typeid), コンポーネントの名前>
	ComponentAdder::NameList ComponentAdder::nameList = {
		{typeid(AabbCollider), "Aabb Collider"},
		{typeid(SphereCollider), "Sphere Collider"},

		{typeid(Camera), "Camera"},
		{typeid(Light), "Light"},
		{typeid(Rigidbody), "Rigidbody"},
		{typeid(Transform), "Transform"},
	};

	// コンポーネント追加用関数の配列
	// <コンポーネントの名前, コンポーネント追加用関数>
	ComponentAdder::FunctionList ComponentAdder::functionList = {
		{"Aabb Collider", &GameObject::AddComponent<AabbCollider>},
		{"Sphere Collider", &GameObject::AddComponent<SphereCollider>},

		{"Camera", &GameObject::AddComponent<Camera>},
		{"Light", &GameObject::AddComponent<Light>},
		{"Rigidbody", &GameObject::AddComponent<Rigidbody>},
		{"Transform", &GameObject::AddComponent<Transform>},
	};

	// --------------------------------
	// 関数
	// --------------------------------

	/// ここでしか使わないので、cppのみに書く
	/// ヘッダーにインクルードしたくないので、メンバ関数にしない
	/// <summary>
	/// コンポーネント追加用ボタン
	/// </summary>
	/// <typeparam name="T"> 追加するコンポーネント </typeparam>
	/// <param name="[out] object"> コンポーネントの追加対象 </param>
	template<class T>
	void AddButton(GameObjectPtr& object)
	{
		// 追加用ボタンを表示
		if (ImGui::Button(ComponentAdder::GetName<T>().c_str()))
		{
			// ボタンが押されたら追加する
			object->AddComponent<T>();

			// 追加したら不要になるので、ポップアップを閉じる
			ImGui::CloseCurrentPopup();
		}
	}

	/// <summary>
	/// コンポーネント追加用リストを表示する
	/// </summary>
	/// <param name="[out] gameObject"> ゲームオブジェクト </param>
	void ComponentAdder::RenderList(GameObjectPtr gameObject)
	{
		AddButton<Rigidbody>(gameObject);
		AddButton<Light>(gameObject);
		AddButton<AabbCollider>(gameObject);
		AddButton<SphereCollider>(gameObject);
	}

	/// <summary>
	/// ゲームオブジェクトにコンポーネントを追加する
	/// </summary>
	/// <param name="[in] name"> 追加するコンポーネントの名前 </param>
	/// <param name="[out] gameObject"> ゲームオブジェクト </param>
	/// <returns> 追加したコンポーネント </returns>
	ComponentPtr ComponentAdder::AddComponent(const std::string& name, GameObject& gameObject)
	{
		if (functionList.find(name) == functionList.end())
		{
			return nullptr;
		}

		return functionList[name](gameObject).first;
	}
}