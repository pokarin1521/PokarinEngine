/**
* @file Collision.h
*/
#ifndef POKARINENGINE_COLLISION_H_INCLUDED
#define POKARINENGINE_COLLISION_H_INCLUDED

#include "../UsingNames/UsingGameObject.h"

namespace PokarinEngine
{
	/// <summary>
	/// 衝突用
	/// </summary>
	namespace Collision
	{
		/// <summary>
		/// ゲームオブジェクトの衝突を処理する
		/// </summary>
		/// <param name="[in] gameObjectList"> 衝突を処理するゲームオブジェクト配列 </param>
		void GameObjectCollision(const GameObjectList& gameObjectList);
	}
}

#endif // !POKARINENGINE_COLLISION_H_INCLUDED
