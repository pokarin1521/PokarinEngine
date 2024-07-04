/**
* @file Collider.h
*/
#ifndef COLLIDER_H_INCLUDED
#define COLLIDER_H_INCLUDED

#include "../Component.h"

#include "../../Collision/CollisionType.h"

#include "../../Math/Vector.h"
#include "../../Math/Matrix.h"

#include "../../UsingNames/UsingCollider.h"

#include <memory>

namespace PokarinEngine
{
	/// <summary>
	/// コライダーの基底クラス
	/// </summary>
	class Collider : public Component
	{
	public: // ------- コンストラクタ・デストラクタ -------

		Collider() = default;
		virtual ~Collider() = default;

	public: // ------------------ 制御 --------------------

		/// <summary>
		/// 更新
		/// </summary>
		void Update() override;

	public: // --------------- 図形の種類 -----------------

		/// <summary>
		/// 図形の種類
		/// </summary>
		enum class Type
		{
			AABB,   // 軸平行境界ボックス
			Sphere, // 球体
		};

	public: // ------------- コライダーの機能 -------------

		/// <summary>
		/// 図形の種類を取得する
		/// </summary>
		virtual Type GetType() const = 0;

		/// <summary>
		/// コライダーを移動させる
		/// </summary>
		/// <param name="[in] translate"> 移動量 </param> 
		virtual void AddPosition(const Vector3& translate) = 0;

		/// <summary>
		/// 座標変換したコライダーを取得する
		/// </summary>
		/// <param name="[in] transform"> 座標変換行列 </param>
		/// <returns> 座標変換したコライダー </returns>
		virtual ColliderPtr GetTransformedCollider(
			const Matrix4x4& transform) const = 0;

	public: // ------------- コライダーの情報 -------------

		// true = 重複を許可
		// false = 重複を禁止
		bool isTrigger = false;

		// true = 動かない物体
		// false = 動く物体
		bool isStatic = false;

	protected: // ------------- エディタ用 ----------------

		// ドラッグ操作用スライダーの幅
		const float sliderWidth = 90.0f;

	private: // ------------------ 保存 -------------------

		/// <summary>
		/// コンポーネントの情報を保存する
		/// </summary>
		/// <param name="[in] folderName"> 保存先のフォルダ </param>
		void SaveInfo(const std::string& folderName) const override {}
	};

} // namespace PokarinEngine

#endif // !COLLIDER_H_INCLUDED
