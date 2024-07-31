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
	// �O���錾
	// -------------------------

	class GameObject;
	
	// --------------------------------------
	// �^�̕ʖ����`
	// --------------------------------------

	using GameObjectPtr = std::shared_ptr<GameObject>;
	using GameObjectList = std::vector<GameObjectPtr>;
}

#endif // !POKARINENGINE_USINGGAMEOBJECT_H_INCKUDED
