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
	/// 剛体の物理的挙動を扱うコンポーネント
	/// </summary>
	class Rigidbody : public Component
	{
	public: // --------- コンストラクタ・デストラクタ ----------

		Rigidbody() = default;
		~Rigidbody() = default;

	public: // ------------- コンポーネント制御 ----------------

		/// <summary>
		/// ゲーム再生中の更新
		/// </summary>
		void Update_PlayGame() override;

	public: // ---------------- 名前の取得 ------------------

		/// <summary>
		/// コンポーネントの名前を取得する
		/// </summary>
		/// <returns> コンポーネントの名前 </returns>
		const std::string& GetName() override
		{
			return name;
		}

	public: // ------------------- 情報 ---------------------

		// 重力加速度
		const float gravity = 9.81f;

		// 重力の影響を制御する係数
		float gravityScale = 1;

		// 速度
		Vector3 velocity = { 0, 0, 0 };

		// コンポーネントの名前
		inline static const std::string name = "Rigidbody";

	private: // --------------- エディタ用 -----------------

		/// <summary>
		/// 情報を編集できるように表示する
		/// </summary>
		void InfoEditor() override {}
	};

} // namespace PokarinEngine

#endif // !RIGIDBODY_H_INCLUDED
