/**
* @file Collision.h
*/
#ifndef COLLISION_H_INCLUDED
#define COLLISION_H_INCLUDED

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

#endif // !COLLISION_H_INCLUDED
