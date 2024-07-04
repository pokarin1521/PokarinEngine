/**
* @file Collision.h
*/
#ifndef COLLISION_H_INCLUDED
#define COLLISION_H_INCLUDED

#include "../UsingNames/UsingGameObject.h"

namespace PokarinEngine
{
	/// <summary>
	/// �Փ˗p
	/// </summary>
	namespace Collision
	{
		/// <summary>
		/// �Q�[���I�u�W�F�N�g�̏Փ˂���������
		/// </summary>
		/// <param name="[in] gameObjectList"> �Փ˂���������Q�[���I�u�W�F�N�g�z�� </param>
		void GameObjectCollision(const GameObjectList& gameObjectList);
	}
}

#endif // !COLLISION_H_INCLUDED