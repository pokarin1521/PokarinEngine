/**
* @file UsingCollider.h
*/
#ifndef USINGCOLLIDER_H_INCLUDED
#define USINGCOLLIDER_H_INCLUDED

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

#endif // !USINGCOLLIDER_H_INCLUDED
