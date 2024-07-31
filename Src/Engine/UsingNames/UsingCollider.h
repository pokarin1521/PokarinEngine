/**
* @file UsingCollider.h
*/
#ifndef POKARINENGINE_USINGCOLLIDER_H_INCLUDED
#define POKARINENGINE_USINGCOLLIDER_H_INCLUDED

#include <memory>

namespace PokarinEngine
{
	// ---------------
	// 前方宣言
	// ---------------

	class Collider;

	// -------------------------------
	// 型の別名を定義
	// -------------------------------

	using ColliderPtr = std::shared_ptr<Collider>;
}

#endif // !POKARINENGINE_USINGCOLLIDER_H_INCLUDED
