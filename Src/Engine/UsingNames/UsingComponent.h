/**
* @file UsingComponent.h
*/
#ifndef POKARINENGINE_USINGCOMPONENT_H_INCLUDED
#define POKARINENGINE_USINGCOMPONENT_H_INCLUDED

#include <memory>

namespace PokarinEngine
{
	// ------------------
	// ëOï˚êÈåæ
	// ------------------

	class Component;

	// -----------------------
	// å^ÇÃï ñºÇíËã`
	// -----------------------

	using ComponentPtr = std::shared_ptr<Component>;
}

#endif // !POKARINENGINE_USINGCOMPONENT_H_INCLUDED
