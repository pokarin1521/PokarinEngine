/**
* @file ComponentName.cpp
*/
#include "ComponentName.h"

#include "Colliders/AabbCollider.h"
#include "Colliders/SphereCollider.h"

#include "Camera.h"
#include "Light.h"
#include "Rigidbody.h"
#include "Transform.h"

namespace PokarinEngine
{
	// 名前判別用
	// <型(typeid), コンポーネントの名前>
	ComponentName::NameList ComponentName::nameList = {
		{typeid(AabbCollider), "Aabb Collider"},
		{typeid(SphereCollider), "Sphere Collider"},

		{typeid(Camera), "Camera"},
		{typeid(Light), "Light"},
		{typeid(Rigidbody), "Rigidbody"},
		{typeid(Transform), "Transform"},
	};

	// 型判別用
	// <コンポーネントの名前, 型(typeid)>
	ComponentName::TypeList ComponentName::typeList = {
		{"Aabb Collider", typeid(AabbCollider)},
		{"Sphere Collider", typeid(SphereCollider)},

		{"Camera", typeid(Camera)},
		{"Light", typeid(Light)},
		{"Rigidbody", typeid(Rigidbody)},
		{"Transform", typeid(Transform)},
	};
}