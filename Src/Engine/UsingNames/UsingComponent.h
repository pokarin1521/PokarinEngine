/**
* @file UsingComponent.h
*/
#ifndef POKARINENGINE_USINGCOMPONENT_H_INCLUDED
#define POKARINENGINE_USINGCOMPONENT_H_INCLUDED

#include <memory>

namespace PokarinEngine
{
	// ------------------
	// �O���錾
	// ------------------

	class Component;

	// -----------------------
	// �^�̕ʖ����`
	// -----------------------

	using ComponentPtr = std::shared_ptr<Component>;
}

#endif // !POKARINENGINE_USINGCOMPONENT_H_INCLUDED
