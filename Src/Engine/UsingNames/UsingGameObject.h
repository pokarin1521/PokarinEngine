/**
* @file GameObjectUsing.h
*/
#ifndef POKARINENGINE_USINGGAMEOBJECT_H_INCKUDED
#define POKARINENGINE_USINGGAMEOBJECT_H_INCKUDED

#include <memory>
#include <vector>

namespace PokarinEngine
{
	// -------------------------
	// 前方宣言
	// -------------------------

	class GameObject;
	
	// --------------------------------------
	// 型の別名を定義
	// --------------------------------------

	using GameObjectPtr = std::shared_ptr<GameObject>;
	using GameObjectList = std::vector<GameObjectPtr>;
}

#endif // !POKARINENGINE_USINGGAMEOBJECT_H_INCKUDED
