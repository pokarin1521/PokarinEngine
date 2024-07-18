/**
* @file AabbCollider.h
*/
#ifndef AABBCOLLIDER_H_INCLUDED
#define AABBCOLLIDER_H_INCLUDED

#include "Collider.h"

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
		/// コライダーを移動させる
		/// </summary>
		/// <param name="[in] translate"> 移動量 </param>
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
		/// <param name="[in] transform"> 座標変換行列 </param>
		/// <returns> 座標変換したコライダー </returns>
		ColliderPtr GetTransformedCollider(
			const Matrix4x4& transform) const override;

	public: // ---------------- 情報の取得 ------------------

		/// <summary>
		/// 図形の種類を取得する
		/// </summary>
		/// <returns> 図形の種類 </returns>
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

	public: // ------------------- Json ---------------------

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

	public: // ------------------- 情報 ---------------------

		// 図形(軸平行境界ボックス)
		Collision::AABB aabb;

	private: // ---------------- エディタ用 -----------------

		/// <summary>
		/// 情報を編集できるように表示する
		/// </summary>
		void InfoEditor() override;
	};

} // namespace PokarinEngine

#endif // !AABBCOLLIDER_H_INCLUDED
