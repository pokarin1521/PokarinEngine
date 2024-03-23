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
		virtual ~SphereCollider() = default;

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
		void AddPosition(const Vec3& translate) override
		{
			sphere.position += translate;
		}

		/// <summary>
		/// 座標変換したコライダーを取得する
		/// </summary>
		/// <param name="transform"></param>
		/// <returns></returns>
		ColliderPtr GetTransformedCollider(
			const Mat4& transform) const override
		{
			// -------------------------------
			// 中心座標を座標変換する
			// -------------------------------

			// 座標変換した球体コライダーを格納する変数
			auto p = std::make_shared<SphereCollider>();

			// 座標変換
			p->sphere.position = Vec3(
				transform * Vec4(sphere.position, 1));

			// -------------------------------
			// 球体の拡大率を反映
			// -------------------------------

			/* 球体の拡大率は、
			XYZが等しくなくてはならない(楕円形になるから)

			オブジェクトの拡大率は個別に指定できてしまうので
			「座標変換行列に含まれるXYZの拡大率のうち最大の値」を
			球体の拡大率とする(unityの球体コライダーと同じ挙動) */

			// 拡大率を取得
			const Vec3 scale = Mat4_Function::ExtractScale(transform);

			// X,Y,Zの拡大率で最大の値
			const float maxScale = std::max({ scale.x, scale.y, scale.z });

			// 拡大率を反映
			p->sphere.radius = sphere.radius * maxScale;

			// 座標変換した球体コライダー
			return p;
		}

		/// <summary>
		/// 図形を取得する
		/// </summary>
		/// <returns> 球体 </returns>
		const Sphere& GetShape() const
		{
			return sphere;
		}

	public: // -------------- コライダーの情報 ---------------

		// 図形(球体)
		Sphere sphere = { Vec3(0), 1 };
	};

} // namespace PokarinEngine

#endif // !SPHERECOLLIDER_H_INCLUDED
