/**
* @file UsingScene.h
*/
#ifndef POKARINENGINE_USINGSCENE_H_INCLUDED
#define POKARINENGINE_USINGSCENE_H_INCLUDED

#include <memory>

namespace PokarinEngine
{
	// -----------------
	// �O���錾
	// -----------------

	class Scene;

	// ---------------------------
	// �^�̕ʖ����`
	// ---------------------------

	using ScenePtr = std::shared_ptr<Scene>;
}

#endif // !POKARINENGINE_USINGSCENE_H_INCLUDED
