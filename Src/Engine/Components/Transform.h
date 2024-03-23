/**
* @file Transform.h
*/
#ifndef TRANSFORM_H_INCLUDED
#define TRANSFORM_H_INCLUDED

#include "Component.h"
#include "../VecMath.h"

#include <vector>
#include <string>

namespace PokarinEngine
{
	// -----------------
	// 前方宣言
	// -----------------

	class Transform;

	// ------------------------
	// 型の別名を定義
	// ------------------------

	using TransformPtr = std::shared_ptr<Transform>;

	/// <summary>
	/// 位置、回転、拡大率を保持するコンポーネント
	/// </summary>
	class Transform : public Component
	{
	public: // ------- コンストラクタ・デストラクタ --------

		Transform() = default;

		virtual ~Transform() = default;

	public: // ----------------- 禁止事項 ------------------

		// コピーの禁止
		Transform(Transform&) = delete;

		// 代入の禁止
		Transform& operator=(Transform&) = delete;

	public: // ----------- コンポーネントの制御 ------------

		/// <summary>
		/// ゲームオブジェクトが削除された時の処理
		/// </summary>
		void OnDestroy() override;

	public: // ------------------- 回転 --------------------

		/// <summary>
		/// pointを中心にY軸回転
		/// </summary>
		/// <param name="point"> 中心の位置 </param>
		/// <param name="rotY"> Y軸回転させる角度(弧度法) </param>
		/// <param name="radius"> 中心からの距離 </param>
		void RotateAroundY(const Vec3& point, float rotY, float distance);

		/// <summary>
		/// ターゲットの方を向くようにY軸回転
		/// </summary>
		/// <param name="target"> ターゲットの座標 </param>
		void LookAtY(const Vec3& target);

	public: // ---------- 親オブジェクトの取得・設定 ----------

		/// <summary>
		/// 親オブジェクトを取得
		/// </summary>
		/// <returns> 親オブジェクトのトランスフォーム </returns>
		Transform* GetParent() const
		{
			return parent;
		}

		/// <summary>
		/// 親オブジェクトを設定
		/// </summary>
		/// <param name="parent"> 
		/// <para> 親にするゲームオブジェクトのトランスフォーム </para>
		/// <para> nullptrを指定すると親子関係を解除する </para>
		/// </param>
		void SetParent(Transform* parent);

		/// <summary>
		/// 親オブジェクトを設定
		/// </summary>
		/// <param name="parent"> 
		/// <para> 親にするゲームオブジェクトのトランスフォーム </para>
		/// <para> nullptrを指定すると親子関係を解除する </para>
		/// </param>
		void SetParent(const TransformPtr& parent);

	public: // ------------ 子オブジェクトの取得 ------------

		/// <summary>
		/// 子オブジェクトの数を取得する
		/// </summary>
		/// <returns> 子オブジェクトの数 </returns>
		size_t GetChildCount() const
		{
			return children.size();
		}

		/// <summary>
		/// 子オブジェクトを取得する
		/// </summary>
		/// <param name="index"> 子オブジェクトの要素番号 </param>
		/// <returns> index番の子オブジェクトのトランスフォーム </returns>
		Transform* GetChild(size_t index) const
		{
			return children[index];
		}

	public: // ------------- 変換行列の取得・設定 -------------

		/// <summary>
		/// 座標変換行列を取得する
		/// </summary>
		/// <returns> 座標変換行列 </returns>
		const Mat4& GetTransformMatrix() const
		{
			return transformMatrix;
		}

		/// <summary>
		/// 法線変換行列を取得する
		/// </summary>
		/// <returns> 法線変換行列 </returns>
		const Mat3& GetNormalMatrix() const
		{
			return normalMatrix;
		}

		/// <summary>
		/// 座標変換行列を設定する
		/// </summary>
		/// <param name="mat"> 座標変換行列 </param>
		void SetTransformMatrix(const Mat4& mat)
		{
			transformMatrix = mat;
		}

		/// <summary>
		/// 法線変換行列を設定する
		/// </summary>
		/// <param name="mat"> 法線変換行列 </param>
		void SetNormalMatrix(const Mat3& mat)
		{
			normalMatrix = mat;
		}

	public: // ----------------- エディタ用 -----------------

		/// <summary>
		/// エディタに情報を表示する
		/// </summary>
		void RenderEditor() override;

	public: // -------------------- 情報 --------------------

		// 位置
		Vec3 position = { 0, 0, 0 };

		// 回転(弧度法)
		Vec3 rotation = { 0, 0, 0 };

		// 拡大率
		Vec3 scale = { 1, 1, 1 };

	private: // ---------------- エディタ用 ----------------

		// 回転(度数法)
		// スライダー表示に使う
		Vec3 rotationDeg = { 0, 0, 0 };

		bool isDrag = false;

	private: // ----------------- 変換行列 -----------------

		// 座標変換行列
		Mat4 transformMatrix = Mat4(0);

		// 法線変換行列
		Mat3 normalMatrix = Mat3(0);

	private: // ------------------- 親子 --------------------

		/* 循環参照を防ぐために生ポインタで管理する */

		// 親オブジェクトのトランスフォーム
		Transform* parent = nullptr;

		// 子オブジェクトのトランスフォーム
		std::vector<Transform*> children;
	};

} // namespace PokarinEngine

#endif // !TRANSFORM_H_INCLUDED
