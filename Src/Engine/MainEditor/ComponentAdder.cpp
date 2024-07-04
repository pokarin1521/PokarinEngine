/**
* @file ComponentAdder.cpp
*/
#include "Inspector.h"

#include "../GameObject.h"

#include "../Components/Rigidbody.h"
#include "../Components/Light.h"
#include "../Components/Colliders/AabbCollider.h"
#include "../Components/Colliders/SphereCollider.h"

#include "../Components/ComponentName.h"

namespace PokarinEngine
{
	/// <summary>
	/// コンポーネント追加用ボタン
	/// </summary>
	/// <typeparam name="T"> 追加するコンポーネント </typeparam>
	/// <param name="[out] object"> コンポーネントの追加対象 </param>
	template<class T>
	void AddButton(GameObjectPtr& object)
	{
		// 追加用ボタンを表示
		if (ImGui::Button(ComponentName::Get<T>().c_str()))
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
	void Inspector::AddComponentList()
	{
		AddButton<Rigidbody>(selectObject);
		AddButton<Light>(selectObject);
		AddButton<AabbCollider>(selectObject);
		AddButton<SphereCollider>(selectObject);
	}
}