/**
* @file Rigidbody.h
*/
#ifndef RIGIDBODY_H_INCLUDED
#define RIGIDBODY_H_INCLUDED

#include "Component.h"
#include "../GameObject.h"

namespace PokarinEngine
{
	/// <summary>
	/// 物理挙動を扱うコンポーネント
	/// </summary>
	class Rigidbody : public Component
	{
	public: // --------- コンストラクタ・デストラクタ ----------

		Rigidbody() = default;
		~Rigidbody() = default;

	public: // ------------- コンポーネント制御 ----------------

		/// <summary>
		/// Rigibodyコンポーネントの更新
		/// </summary>
		/// <param name="deltaTime"></param>
		void Update(float deltaTime) override;

	public: // ------------------- 情報 ---------------------

		// 重力加速度
		static constexpr float gravity = 9.81f;

		// 重力の影響を制御する係数
		float gravityScale = 1;

		// 速度
		Vector3 velocity = { 0, 0, 0 };
	};

} // namespace PokarinEngine

#endif // !RIGIDBODY_H_INCLUDED
