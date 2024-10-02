/**
* @file Rigidbody.h
*/
#ifndef POKARINENGINE_RIGIDBODY_H_INCLUDED
#define POKARINENGINE_RIGIDBODY_H_INCLUDED

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
		/// ゲーム再生時の初期化処理
		/// </summary>
		void Initialize_PlayGame() override;

		/// <summary>
		/// ゲーム再生中の更新
		/// </summary>
		void Update_PlayGame() override;

		/// <summary>
		/// 削除時の処理
		/// </summary>
		void OnDestroy() override;

	public: // --------------------- Json ----------------------

		/// <summary>
		/// コンポーネントの情報をJson型に格納する
		/// </summary>
		/// <param name="[out] json"> 情報を格納するJson型 </param>
		void ToJson(Json& json) const override;

		/// <summary>
		/// コンポーネントの情報をJson型から取得する
		/// </summary>
		/// <param name="[in] json"> 情報を格納しているJson型 </param>
		void FromJson(const Json& json) override;

	private: // --------------- エディタ用 -----------------

		/// <summary>
		/// 情報を編集できるように表示する
		/// </summary>
		void InfoEditor() override;

	private: // ------------------ 情報 --------------------

		// 重力加速度
		const float gravity = 9.81f;

		// 重力の影響を制御する係数
		float gravityScale = 1;

		// 速度
		Vector3 velocity = { 0, 0, 0 };

		// 重力を有効にするならtrue
		bool useGravity = true;
	};

} // namespace PokarinEngine

#endif // !POKARINENGINE_RIGIDBODY_H_INCLUDED
