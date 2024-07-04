/**
* @file Collider.cpp
*/
#include "Collider.h"

#include "../../GameObject.h"

namespace PokarinEngine
{
	/// <summary>
	/// 更新
	/// </summary>
	void Collider::Update()
	{
		// 持ち主であるゲームオブジェクトが
		// 物理挙動用コンポーネントを持っていたら衝突時に動くようにする
		isStatic = !GetOwnerObject().HasRigidbody();
	}
}