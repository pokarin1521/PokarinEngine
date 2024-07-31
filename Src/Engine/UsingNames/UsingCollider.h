/**
* @file UsingCollider.h
*/
#ifndef POKARINENGINE_USINGCOLLIDER_H_INCLUDED
#define POKARINENGINE_USINGCOLLIDER_H_INCLUDED

#include <memory>

namespace PokarinEngine
{
	// ---------------
	// �O���錾
	// ---------------

	class Collider;

	// -------------------------------
	// �^�̕ʖ����`
	// -------------------------------

	using ColliderPtr = std::shared_ptr<Collider>;
}

#endif // !POKARINENGINE_USINGCOLLIDER_H_INCLUDED
