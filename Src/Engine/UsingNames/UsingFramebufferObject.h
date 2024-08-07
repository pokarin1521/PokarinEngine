/**
* @file UsingFramebufferObject.h
*/
#ifndef POKARINENGINE_USINGFRAMEBUFFEROBJECT_H_INCLUDED
#define POKARINENGINE_USINGFRAMEBUFFEROBJECT_H_INCLUDED

#include <memory>

namespace PokarinEngine
{
	// ---------------------
	// �O���錾
	// ---------------------

	class FramebufferObject;

	// -----------------------------
	// �^�̕ʖ����`
	// -----------------------------

	using FramebufferObjectPtr = std::shared_ptr<FramebufferObject>;
}

#endif // !POKARINENGINE_USINGFRAMEBUFFEROBJECT_H_INCLUDED
