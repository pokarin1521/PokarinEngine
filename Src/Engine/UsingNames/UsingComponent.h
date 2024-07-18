/**
* @file UsingComponent.h
*/
#ifndef USINGCOMPONENT_H_INCLUDED
#define USINGCOMPONENT_H_INCLUDED

#include <memory>

namespace PokarinEngine
{
	// ------------------
	// ‘O•ûéŒ¾
	// ------------------

	class Component;

	// -----------------------
	// Œ^‚Ì•Ê–¼‚ğ’è‹`
	// -----------------------

	using ComponentPtr = std::shared_ptr<Component>;
}

#endif // !USINGCOMPONENT_H_INCLUDED
