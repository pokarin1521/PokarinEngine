/**
* @file AabbCollider.h
*/
#ifndef AABBCOLLIDER_H_INCLUDED
#define AABBCOLLIDER_H_INCLUDED

#include "Collider.h"
#include "../../Collision.h"

#include "../../Debug.h"

namespace PokarinEngine
{
	// -----------------
	// 前方宣言
	// -----------------

	class AabbCollider;

	// -----------------------
	// 型の別名を定義
	// -----------------------

	using AabbColliderPtr = std::shared_ptr<AabbCollider>;

	/// <summary>
	/// AABBコライダー
	/// </summary>
	class AabbCollider : public Collider
	{
	public: // -------- コンストラクタ・デストラクタ --------

		AabbCollider() = default;
		~AabbCollider() = default;

	public: // ------------------- 座標 ---------------------

		/// <summary>
		/// コライダーの座標を変更する
		/// </summary>
		/// <param name="translate"> 移動量 </param>
		void AddPosition(const Vector3& translate) override
		{
			aabb.min += translate;
			aabb.max += translate;
		}

		/// <summary>
		/// <para> 座標変換したコライダーを取得する </para>
		/// <para> 回転角度は90度単位で指定すること </para>
		/// <para> それ以外の角度では正しい交差判定が行えない </para>
		/// </summary>
		/// <param name="transform"> 座標変換行列 </param>
		/// <returns> 座標変換したコライダー </returns>
		ColliderPtr GetTransformedCollider(
			const Matrix4x4& transform) const override;

	public: // ---------------- 情報の取得 ------------------

		/// <summary>
		/// 図形の種類を取得する
		/// </summary>
		/// <returns> 軸平行境界ボックスを表すType </returns>
		Type GetType() const override
		{
			return Type::AABB;
		}

		/// <summary>
		/// 図形を取得する
		/// </summary>
		/// <returns> 軸平行境界ボックス </returns>
		const Collision::AABB& GetShape() const
		{
			return aabb;
		}

		/// <summary>
		/// コンポーネントの名前を取得する
		/// </summary>
		/// <returns> コンポーネントの名前 </returns>
		const std::string& GetName() override
		{
			return name;
		}

	public: // ------------------- 情報 ---------------------

		// 図形(軸平行境界ボックス)
		Collision::AABB aabb = { Vector3(-1), Vector3(1) };

		// コンポーネントの名前
		inline static const std::string name = "Aabb Collider";

	private: // --------------- エディタ用 ------------------

		/// <summary>
		/// 情報を編集できるように表示する
		/// </summary>
		void InfoEditor() override {}
	};

} // namespace PokarinEngine

#endif // !AABBCOLLIDER_H_INCLUDED
