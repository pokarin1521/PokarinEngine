/**
* @file BoxCollider.h
*/
#ifndef POKARINENGINE_BOXCOLLIDER_H_INCLUDED
#define POKARINENGINE_BOXCOLLIDER_H_INCLUDED

#include "Collider.h"

namespace PokarinEngine
{
	/// <summary>
	/// Boxコライダー
	/// </summary>
	class BoxCollider : public Collider
	{
	public: // -------- コンストラクタ・デストラクタ --------

		BoxCollider() = default;
		~BoxCollider() = default;

	public: // ------------------- 座標 ---------------------

		/// <summary>
		/// コライダーを移動させる
		/// </summary>
		/// <param name="[in] translate"> 移動量 </param>
		void AddPosition(const Vector3& translate) override
		{
			box.center += translate;
		}

		/// <summary>
		/// 座標変換したコライダーを取得する
		/// </summary>
		/// <returns> 座標変換したコライダー </returns>
		ColliderPtr GetTransformedCollider() const override;

	public: // ---------------- 情報の取得 ------------------

		/// <summary>
		/// 図形の種類を取得する
		/// </summary>
		/// <returns> 図形の種類 </returns>
		Type GetType() const override
		{
			return Type::Box;
		}

		/// <summary>
		/// 図形を取得する
		/// </summary>
		/// <returns> 有向境界ボックス </returns>
		const Collision::Box& GetShape() const
		{
			return box;
		}

	public: // ------------------- 情報 ---------------------

		// 図形(有向境界ボックス)
		Collision::Box box;

	private: // --------------- 座標変換行列 ----------------

		/// <summary>
		/// 座標変換行列を取得する
		/// </summary>
		/// <returns> 座標変換行列 </returns>
		Matrix4x4 GetTransformMatrix() const;

	private: // ---------------- エディタ用 -----------------

		/// <summary>
		/// コライダー別の情報を編集できるように表示する
		/// </summary>
		void ColliderInfoEditor() override;

	private: // ------------------- Json --------------------

		/// <summary>
		/// コンポーネントの情報をJson型に格納する
		/// </summary>
		/// <param name="[out] json"> 情報を格納するJson型 </param>
		void ColliderToJson(Json& json) const override;

		/// <summary>
		/// コンポーネントの情報をJson型から取得する
		/// </summary>
		/// <param name="[in] json"> 情報を格納しているJson型 </param>
		void ColliderFromJson(const Json& json) override;
	};
}

#endif // !POKARINENGINE_BOXCOLLIDER_H_INCLUDED
