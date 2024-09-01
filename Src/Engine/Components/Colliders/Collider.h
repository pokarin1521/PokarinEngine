/**
* @file Collider.h
*/
#ifndef POKARINENGINE_COLLIDER_H_INCLUDED
#define POKARINENGINE_COLLIDER_H_INCLUDED

#include "../Component.h"

#include "../../UsingNames/UsingCollider.h"
#include "../../UsingNames/UsingStaticMesh.h"

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
		void Update() override final;

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

	public: // ------------------ 取得 -------------------

		/// <summary>
		/// 重複可能か取得する
		/// </summary>
		/// <returns> 重複可能ならtrue </returns>
		bool IsTrigger() const
		{
			return isTrigger;
		}

		/// <summary>
		/// 動かないか取得する
		/// </summary>
		/// <returns> 動かないならtrue </returns>
		bool IsStatic() const
		{
			return isStatic;
		}

		/// <summary>
		/// 図形の種類を取得する
		/// </summary>
		virtual Type GetType() const = 0;

		/// <summary>
		/// 座標変換したコライダーを取得する
		/// </summary>
		/// <returns> 座標変換したコライダー </returns>
		virtual ColliderPtr GetTransformedCollider() const = 0;

	public: // ------------------ 移動 -------------------

		/// <summary>
		/// コライダーを移動させる
		/// </summary>
		/// <param name="[in] translate"> 移動量 </param> 
		virtual void AddPosition(const Vector3& translate) = 0;

	private: // ----------------- 初期化 ------------------

		/// <summary>
		/// ゲームオブジェクトに追加された時の初期化
		/// </summary>
		void Awake() override final;

	private: // -------------- 座標変換行列 ---------------

		/// <summary>
		/// 座標変換行列を取得する
		/// </summary>
		/// <returns> 座標変換行列 </returns>
		virtual Matrix4x4 GetTransformMatrix() const = 0;

	private: // --------------- エディタ用 ----------------

		/// <summary>
		/// 情報を編集できるように表示する
		/// </summary>
		void InfoEditor() override final;

		/// <summary>
		/// コライダー別の情報を編集できるように表示する
		/// </summary>
		virtual void ColliderInfoEditor() = 0;

	private: // ------------ コライダーの情報 -------------

		// 重複可能ならtrue
		bool isTrigger = false;

		// 動かないならtrue
		bool isStatic = false;

		// スタティックメッシュ
		StaticMeshPtr staticMesh;
	};

} // namespace PokarinEngine

#endif // !POKARINENGINE_COLLIDER_H_INCLUDED
