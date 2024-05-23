/**
* @file Transform.h
*/
#ifndef TRANSFORM_H_INCLUDED
#define TRANSFORM_H_INCLUDED

#include "Component.h"
#include "../Math/Vector.h"
#include "../Math/Matrix.h"

#include <vector>
#include <string>
#include <cmath>

namespace PokarinEngine
{
	// -----------------
	// 前方宣言
	// -----------------

	class Transform;
	class GameObject;

	// ------------------------
	// 型の別名を定義
	// ------------------------

	using TransformPtr = std::shared_ptr<Transform>;

	/// <summary>
	/// 位置、回転、拡大率を保持するコンポーネント
	/// </summary>
	class Transform : public Component
	{
		// ゲームオブジェクトに情報を公開
		friend GameObject;

	public: // ---------------- コンストラクタ・デストラクタ -----------------

		Transform() = default;
		~Transform() = default;

	public: // --------------------- コンポーネントの制御 --------------------

		/// <summary>
		/// 更新
		/// </summary>
		void Update() override;

		/// <summary>
		/// ゲームオブジェクトが削除された時の処理
		/// </summary>
		void OnDestroy() override;

	public: // ------------------ 親オブジェクトの取得・設定 -----------------

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

	public: // ---------------------- 子オブジェクトの取得 ----------------------

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

	public: // --------------------- 変換行列の取得・設定 ----------------------

		/// <summary>
		/// 座標変換行列を取得する
		/// </summary>
		/// <returns> 座標変換行列 </returns>
		const Matrix4x4& GetTransformMatrix() const
		{
			return transformMatrix;
		}

		/// <summary>
		/// 法線変換行列を取得する
		/// </summary>
		/// <returns> 法線変換行列 </returns>
		const Matrix3x3& GetNormalMatrix() const
		{
			return normalMatrix;
		}

	public: // ---------------------- 方向ベクトル ---------------------

		/// <summary>
		/// 正面ベクトルを取得する
		/// </summary>
		/// <returns></returns>
		inline Vector3 Forward() const
		{
			// 回転角度のSin
			Vector2 rotationSin = { std::sin(rotation.x), std::sin(rotation.y) };

			// 回転角度のCos
			Vector2 rotationCos = { std::cos(rotation.x), std::cos(rotation.y) };

			return { -rotationSin.y, rotationSin.x, rotationCos.x * rotationCos.y };
		}

	public: // ----------------------- エディタ用 ----------------------

		/// <summary>
		/// 情報を編集できるように表示する
		/// </summary>
		void InfoEditor() override;

	public: // ----------------------- 名前の取得 ----------------------

		/// <summary>
		/// コンポーネントの名前を取得する
		/// </summary>
		/// <returns> コンポーネントの名前 </returns>
		const std::string& GetName() override
		{
			return name;
		}

	public: // -------------------- ワールド軸の情報 -------------------

		// ワールド軸の座標
		Vector3 position = { 0, 0, 0 };

		// ワールド軸の回転角度(弧度法)
		Vector3 rotation = { 0, 0, 0 };

		// 拡大率
		Vector3 scale = { 1, 1, 1 };

	public: // -------------------------- 名前 ------------------------

		// コンポーネントの名前
		inline static const std::string name = "Transform";

	private: // ----------------------- 変換行列 -----------------------

		// 座標変換行列
		Matrix4x4 transformMatrix = Matrix4x4(0);

		// 法線変換行列
		Matrix3x3 normalMatrix = Matrix3x3(0);

	private: // ------------------------- 親子 -------------------------

		/* 循環参照を防ぐために生ポインタで管理する */

		// 親オブジェクトのトランスフォーム
		Transform* parent = nullptr;

		// 子オブジェクトのトランスフォーム
		std::vector<Transform*> children;
	};

} // namespace PokarinEngine

#endif // !TRANSFORM_H_INCLUDED
