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
	// ëOï˚êÈåæ
	// -------------------------

	class GameObject;
	
	// --------------------------------------
	// å^ÇÃï ñºÇíËã`
	// --------------------------------------

	using GameObjectPtr = std::shared_ptr<GameObject>;
	using GameObjectList = std::vector<GameObjectPtr>;
}

#endif // !POKARINENGINE_USINGGAMEOBJECT_H_INCKUDED
