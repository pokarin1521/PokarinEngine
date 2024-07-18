/**
* @file UsingComponent.h
*/
#ifndef USINGCOMPONENT_H_INCLUDED
#define USINGCOMPONENT_H_INCLUDED

#include <memory>

namespace PokarinEngine
{
	// ------------------
	// 前方宣言
	// ------------------

	class Component;

	// -----------------------
	// 型の別名を定義
	// -----------------------

	using ComponentPtr = std::shared_ptr<Component>;
}

#endif // !USINGCOMPONENT_H_INCLUDED
