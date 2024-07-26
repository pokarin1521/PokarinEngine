/**
* @file Collider.h
*/
#ifndef COLLIDER_H_INCLUDED
#define COLLIDER_H_INCLUDED

#include "../Component.h"

#include "../../UsingNames/UsingCollider.h"
#include "../../UsingNames/UsingMesh.h"

#include "../../Collision/CollisionType.h"

#include "../../Math/Vector.h"
#include "../../Math/Matrix.h"

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

		/// <summary>
		/// 描画
		/// </summary>
		void Draw();

	public: // --------------- 図形の種類 -----------------

		/// <summary>
		/// 図形の種類
		/// </summary>
		enum class Type
		{
			Box,	// 有向境界ボックス
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
		/// <returns> 座標変換したコライダー </returns>
		virtual ColliderPtr GetTransformedCollider() const = 0;

	public: // ------------- コライダーの情報 -------------

		// true = 重複を許可
		// false = 重複を禁止
		bool isTrigger = false;

		// true = 動かない物体
		// false = 動く物体
		bool isStatic = false;

		// スタティックメッシュ
		StaticMeshPtr staticMesh;

	//protected: // ------------ 座標変換行列 ---------------

	//	// 座標変換行列
	//	Matrix4x4 transformMatrix = Matrix4x4();

	//	// 座標変換したコライダー
	//	ColliderPtr transformedCollider;

	protected: // ------------- エディタ用 ----------------

		// ドラッグ操作用スライダーの幅
		const float sliderWidth = 90.0f;

	private: // ----------------- 初期化 ------------------

		/// <summary>
		/// ゲームオブジェクトに追加された時の初期化
		/// </summary>
		void Awake() override;

	private: // -------------- 座標変換行列 ---------------

		/// <summary>
		/// 座標変換行列を取得する
		/// </summary>
		/// <returns> 座標変換行列 </returns>
		virtual Matrix4x4 GetTransformMatrix() const = 0;
	};

} // namespace PokarinEngine

#endif // !COLLIDER_H_INCLUDED
