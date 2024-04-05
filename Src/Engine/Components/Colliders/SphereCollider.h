/**
* @file SphereCollider.h
*/
#ifndef SPHERECOLLIDER_H_INCLUDED
#define SPHERECOLLIDER_H_INCLUDED

#include "Collider.h"
#include "../../Collision.h"

namespace PokarinEngine
{
	/// <summary>
	/// 球体コライダー
	/// </summary>
	class SphereCollider : public Collider
	{
	public: // -------- コンストラクタ・デストラクタ ---------

		SphereCollider() = default;
		~SphereCollider() = default;

	public: // -------------- コライダーの機能 ---------------

		/// <summary>
		/// 図形の種類を取得する
		/// </summary>
		/// <returns> 球体を表すType </returns>
		Type GetType() const override
		{
			return Type::Sphere;
		}

		/// <summary>
		/// コライダーの座標を変更する
		/// </summary>
		/// <param name="translate"> 移動量 </param>
		void AddPosition(const Vector3& translate) override
		{
			sphere.position += translate;
		}

		/// <summary>
		/// 座標変換したコライダーを取得する
		/// </summary>
		/// <param name="transform"></param>
		/// <returns></returns>
		ColliderPtr GetTransformedCollider(
			const Matrix4x4& transform) const override;

		/// <summary>
		/// 図形を取得する
		/// </summary>
		/// <returns> 球体 </returns>
		const Collision::Sphere& GetShape() const
		{
			return sphere;
		}

	public: // -------------- コライダーの情報 ---------------

		// 図形(球体)
		Collision::Sphere sphere = { Vector3(0), 1 };
	};

} // namespace PokarinEngine

#endif // !SPHERECOLLIDER_H_INCLUDED
