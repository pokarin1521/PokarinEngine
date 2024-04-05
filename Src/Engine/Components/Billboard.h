/**
* @file Billboard.h
*/
#ifndef BILLBOARD_H_INCLUDED
#define BILLBOARD_H_INCLUDED

#include "Component.h"
#include "../Engine.h"

namespace PokarinEngine
{
	/// <summary>
	/// ビルボード化コンポーネント
	/// </summary>
	class Billboard : public Component
	{
	public: // ---------------- コンストラクタ・デストラクタ -------------------

		Billboard() = default;
		~Billboard() = default;

	public: // ------------------------- イベント関数 --------------------------

		/// <summary>
		/// 更新イベント
		/// </summary>
		/// <param name="deltaTime"> 前回の更新からの経過時間(秒) </param>
		void Update(float deltaTime) override
		{
			/* 常にカメラの方を向くようにメッセージオブジェクトの向きを調整 */

			// 持ち主からエンジンを取得
			const GameObject& owner = GetOwner(); // 持ち主

			// カメラの角度と持ち主の角度を合わせる
			const GameObject& mainCamera = owner.GetScene().GetMainCamera();
			owner.transform->rotation.y = mainCamera.transform->rotation.y;
		}
	};

} // namespace PokarinEngine

#endif // !BILLBOARD_H_INCLUDED