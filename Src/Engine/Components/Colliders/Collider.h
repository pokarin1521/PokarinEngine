/**
* @file Collider.h
*/
#ifndef COLLIDER_H_INCLUDED
#define COLLIDER_H_INCLUDED

#include "../Component.h"
#include "../../Math/Vector.h"
#include "../../Math/Matrix.h"

#include <memory>

namespace PokarinEngine
{
	// ---------------
	// 前方宣言
	// ---------------

	class Collider;
	using ColliderPtr = std::shared_ptr<Collider>;

	/// <summary>
	/// コライダーの基底クラス
	/// </summary>
	class Collider : public Component
	{
	public: // ------- コンストラクタ・デストラクタ -------

		Collider() = default;
		virtual ~Collider() = default;

	public: // --------------- 図形の種類 -----------------

		/// <summary>
		/// 図形の種類
		/// </summary>
		enum class Type
		{
			AABB,   // 軸平行境界ボックス
			Sphere, // 球体
		};

	public: // ---------- コライダーの機能 ----------

		/// <summary>
		/// 図形の種類を取得
		/// </summary>
		virtual Type GetType() const = 0;

		/// <summary>
		/// コライダーの座標を変更する
		/// </summary>
		virtual void AddPosition(const Vector3& translate) = 0;

		/// <summary>
		/// 座標変換したコライダーを取得する
		/// </summary>
		virtual ColliderPtr GetTransformedCollider(
			const Matrix4x4& transform) const = 0;

		/// <summary>
		/// エディタ描画用
		/// </summary>
		virtual void RenderInfo() override {}

	public: // ----------- コライダーの情報 -----------

		// true = 重複を許可
		// false = 重複を禁止
		bool isTrigger = false;

		// true = 動かない物体
		// false = 動く物体
		bool isStatic = false;
	};

} // namespace PokarinEngine

#endif // !COLLIDER_H_INCLUDED
