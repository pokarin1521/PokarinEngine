/**
* @file UsingScene.h
*/
#ifndef POKARINENGINE_USINGSCENE_H_INCLUDED
#define POKARINENGINE_USINGSCENE_H_INCLUDED

#include <memory>

namespace PokarinEngine
{
	// -----------------
	// 前方宣言
	// -----------------

	class Scene;

	// ---------------------------
	// 型の別名を定義
	// ---------------------------

	using ScenePtr = std::shared_ptr<Scene>;
}

#endif // !POKARINENGINE_USINGSCENE_H_INCLUDED
