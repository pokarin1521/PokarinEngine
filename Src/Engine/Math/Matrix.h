/**
* @file Matrix.h
*/
#ifndef MATRIX_H_INCLUDED
#define MATRIX_H_INCLUDED

#include "Vector.h"

namespace PokarinEngine
{
	// -----------------------
	// 前方宣言
	// -----------------------

	struct Matrix3x3;
	struct Matrix4x4;

	// ----------------------
	// 構造体
	// ----------------------

#pragma region Matrix

	/// <summary>
	/// 3x3行列
	/// </summary>
	struct Matrix3x3
	{
	public: // -------------------- コンストラクタ --------------------

		// デフォルトコンストラクタ
		Matrix3x3() = default;

		/// <summary>
		/// Vec3型で初期化するコンストラクタ
		/// </summary>
		Matrix3x3(const Vector3& v0, const Vector3& v1, const Vector3& v2)
		{
			data[0] = v0;
			data[1] = v1;
			data[2] = v2;
		}

		/// <summary>
		/// float型で初期化するコンストラクタ
		/// </summary>
		explicit Matrix3x3(float f)
		{
			data[0] = { f, 0, 0 };
			data[1] = { 0, f, 0 };
			data[2] = { 0, 0, f };
		}

		/// <summary>
		/// Mat4型で初期化するコンストラクタ
		/// </summary>
		explicit Matrix3x3(const Matrix4x4& m);

	public: // ------------------------- 関数 -------------------------

		/// <summary>
		/// 回転角度から回転行列を作成
		/// </summary>
		/// <param name="[in] rotation"> 回転角度 </param>
		/// <returns> 作成した回転行列 </returns>
		static Matrix3x3 CreateRotationMatrix(const Vector3& rotation);

	public: // ---------------------- 添字演算子 ----------------------

		Vector3& operator[](size_t i) { return data[i]; }
		const Vector3& operator[](size_t i) const { return data[i]; }

	public: // ------------------------- 変数 -------------------------

		Vector3 data[3];
	};

	/// <summary>
	/// 4x4行列
	/// </summary>
	struct Matrix4x4
	{
	public: // -------------------- コンストラクタ --------------------

		// デフォルトコンストラクタ
		Matrix4x4() = default;

		/// <summary>
		/// Vec4型で初期化するコンストラクタ
		/// </summary>
		Matrix4x4(const Vector4& v0, const Vector4& v1, const Vector4& v2, const Vector4& v3)
		{
			data[0] = v0;
			data[1] = v1;
			data[2] = v2;
			data[3] = v3;
		}

		/// <summary>
		/// float型で初期化するコンストラクタ
		/// </summary>
		explicit Matrix4x4(float f)
		{
			data[0] = { f, 0, 0, 0 };
			data[1] = { 0, f, 0, 0 };
			data[2] = { 0, 0, f ,0 };
			data[3] = { 0, 0, 0, f };
		}

		/// <summary>
		/// Mat3型で初期化するコンストラクタ
		/// </summary>
		explicit Matrix4x4(const struct Matrix3x3& m)
		{
			data[0] = { m.data[0], 0 };
			data[1] = { m.data[1], 0 };
			data[2] = { m.data[2], 0 };
			data[3] = { 0, 0, 0, 1 };
		}

	public: // ------------------------- 関数 -------------------------

		/// <summary>
		/// 座標変換行列を作成する
		/// </summary>
		/// <param name="[in] translate"> 平行移動量 </param>
		/// <param name="[in] rotation"> 回転角度 </param>
		/// <param name="[in] scale"> 拡大率 </param>
		/// <returns> 作成した座標変換行列 </returns>
		static Matrix4x4 CreateTransformMatrix(
			const Vector3& translate, const Vector3& rotation, const Vector3& scale);

		/// <summary>
		/// 座標変換行列から移動量を取得する
		/// </summary>
		/// <param name="[in] transform"> 座標変換行列 </param>
		/// <returns> 平行移動量 </returns>
		static Vector3 Translate(const Matrix4x4& transform);

		/// <summary>
		/// 座標変換行列から拡大率を取得する
		/// </summary>
		/// <param name="[in] transform"> 座標変換行列 </param>
		/// <returns> 拡大率 </returns>
		static Vector3 Scale(const Matrix4x4& transform);
	
		/// <summary>
		/// 座標変換行列から回転行列を取得する
		/// </summary>
		/// <param name="[in] transform"> 座標変換行列 </param>
		/// <returns> 回転行列 </returns>
		static Matrix3x3 Rotate(const Matrix4x4& transform);

	public: // ---------------------- 添字演算子 ----------------------

		Vector4& operator[](size_t i) { return data[i]; }
		const Vector4& operator[](size_t i) const { return data[i]; }

	public: // ------------------------- 変数 -------------------------

		Vector4 data[4];
	};

#pragma endregion

	// ------------------------------
	// オペレーター
	// ------------------------------

#pragma region Mat3_Operator

	// -----------------
	// 乗算
	// -----------------

	// --------- Mat3とVec3の乗算 ---------

	inline Vector3 operator*(const Matrix3x3& m, const Vector3& v)
	{
		return m.data[0] * v.x + m.data[1] * v.y + m.data[2] * v.z;
	}

	// ---------- Mat3同士の乗算 ----------

	inline Matrix3x3 operator*(const Matrix3x3& a, const Matrix3x3& b)
	{
		Matrix3x3 m = Matrix3x3(0);

		m.data[0] = a * b.data[0];
		m.data[1] = a * b.data[1];
		m.data[2] = a * b.data[2];

		return m;
	}

	inline Matrix3x3& operator*=(Matrix3x3& a, const Matrix3x3& b)
	{
		a = a * b;
		return a;
	}

#pragma endregion

#pragma region Mat4_Operator

	// ----------------
	// 乗算
	// ----------------

	// --------- Mat4とVec4の乗算 ----------

	inline Vector4 operator*(const Matrix4x4& m, const Vector4& v)
	{
		return m.data[0] * v.x + m.data[1] * v.y + m.data[2] * v.z + m.data[3] * v.w;
	}

	// ---------- Mat4同士の乗算 -----------

	inline Matrix4x4 operator*(const Matrix4x4& a, const Matrix4x4& b)
	{
		Matrix4x4 m = Matrix4x4(0);

		m.data[0] = a * b.data[0];
		m.data[1] = a * b.data[1];
		m.data[2] = a * b.data[2];
		m.data[3] = a * b.data[3];

		return m;
	}

	inline Matrix4x4& operator*=(Matrix4x4& a, const Matrix4x4& b)
	{
		a = a * b;
		return a;
	}

#pragma endregion

}

#endif // !MATRIX_H_INCLUDED
