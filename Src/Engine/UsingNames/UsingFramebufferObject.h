/**
* @file UsingFramebufferObject.h
*/
#ifndef POKARINENGINE_USINGFRAMEBUFFEROBJECT_H_INCLUDED
#define POKARINENGINE_USINGFRAMEBUFFEROBJECT_H_INCLUDED

#include <memory>

namespace PokarinEngine
{
	// ---------------------
	// 前方宣言
	// ---------------------

	class FramebufferObject;

	// -----------------------------
	// 型の別名を定義
	// -----------------------------

	using FramebufferObjectPtr = std::shared_ptr<FramebufferObject>;
}

#endif // !POKARINENGINE_USINGFRAMEBUFFEROBJECT_H_INCLUDED
