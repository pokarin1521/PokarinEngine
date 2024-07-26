/**
* @file Rigidbody.h
*/
#ifndef RIGIDBODY_H_INCLUDED
#define RIGIDBODY_H_INCLUDED

#include "Component.h"

#include "../Math/Vector.h"

#include <memory>

namespace PokarinEngine
{
	// -------------------------
	// 前方宣言
	// -------------------------

	class Rigidbody;

	// --------------------------------
	// 型の別名を定義
	// --------------------------------

	using RigidbodyPtr = std::shared_ptr<Rigidbody>;

	/// <summary>
	/// 剛体の物理的挙動を扱うコンポーネント
	/// </summary>
	class Rigidbody : public Component
	{
	public: // --------- コンストラクタ・デストラクタ ----------

		Rigidbody() = default;
		~Rigidbody() = default;

	public: // -------------- コンポーネント制御 ---------------

		/// <summary>
		/// ゲーム再生中の更新
		/// </summary>
		void Update_PlayGame() override;

	public: // --------------------- Json ----------------------

		/// <summary>
		/// コンポーネントの情報をJson型に格納する
		/// </summary>
		/// <param name="[out] Json"> 情報を格納するJson型 </param>
		void ToJson(Json& data) const override {}

		/// <summary>
		/// コンポーネントの情報をJson型から取得する
		/// </summary>
		/// <param name="[out] data"> 情報を格納しているJson型 </param>
		void FromJson(const Json& data) override {}

	public: // -------------------- 情報 -----------------------

		// 重力加速度
		const float gravity = 9.81f;

		// 重力の影響を制御する係数
		float gravityScale = 1;

		// 速度
		Vector3 velocity = { 0, 0, 0 };

	private: // --------------- エディタ用 -----------------

		/// <summary>
		/// 情報を編集できるように表示する
		/// </summary>
		void InfoEditor() override {}
	};

} // namespace PokarinEngine

#endif // !RIGIDBODY_H_INCLUDED
