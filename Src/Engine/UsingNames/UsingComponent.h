/**
* @file UsingComponent.h
*/
#ifndef USINGCOMPONENT_H_INCLUDED
#define USINGCOMPONENT_H_INCLUDED

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

#endif // !USINGCOMPONENT_H_INCLUDED
