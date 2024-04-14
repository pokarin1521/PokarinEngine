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
		Vector3 data[3];

		// ---------------------
		// コンストラクタ
		// ---------------------

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
		explicit Matrix3x3(const struct Matrix4x4& m);

		// ---------------------
		// 添字演算子
		// ---------------------

		Vector3& operator[](size_t i) { return data[i]; }
		const Vector3& operator[](size_t i) const { return data[i]; }
	};

	/// <summary>
	/// 4x4行列
	/// </summary>
	struct Matrix4x4
	{
		Vector4 data[4];

		// --------------------
		// コンストラクタ
		// --------------------

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

		// ------------------
		// 添字演算子
		// ------------------

		Vector4& operator[](size_t i) { return data[i]; }
		const Vector4& operator[](size_t i) const { return data[i]; }
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

	// ------------------------------
	// 関数
	// ------------------------------

#pragma region Mat3_Function

	/// <summary>
	/// Mat4型で初期化するコンストラク
	/// </summary>
	inline Matrix3x3::Matrix3x3(const Matrix4x4& m)
	{
		data[0] = Vector3(m.data[0]);
		data[1] = Vector3(m.data[1]);
		data[2] = Vector3(m.data[2]);
	}

	namespace Mat3_Function
	{
		/// <summary>
		/// 回転角度から回転行列を作成
		/// </summary>
		/// <param name="rotation"> 回転角度 </param>
		/// <returns> 作成した回転行列 </returns>
		inline Matrix3x3 GetRotationMatrix(const Vector3& rotation)
		{
			/* 行列は、ベクトル配列と比べて
			「縦と横が反転している」ので

			{  cosZ, -sinZ,  0 }
			{  sinZ,  cosZ,  0 }
			{     0,     0, -1 } 	
			
					 ↓

			{  cosZ,  sinZ,  0 }
			{ -sinZ,  cosZ,  0 }
			{     0,     0, -1 } 

			になる
			
			X,Y,Z軸の回転行列を求めて掛けることで、XYZ軸回転の行列が求められる*/

			// X軸回転のSin・Cos
			const float sinX = sin(rotation.x), cosX = cos(rotation.x);

			// Y軸回転のSin・Cos
			const float sinY = sin(rotation.y), cosY = cos(rotation.y);

			// Z軸回転のSin・Cos
			const float sinZ = sin(rotation.z), cosZ = cos(rotation.z);

			// X軸回転の行列
			const Matrix3x3 matrix_X = {
					   { 1,     0,      0 },
					   { 0,  cosX,  -sinX },
					   { 0,  sinX,   cosX } };

			// Y軸回転の行列
			const Matrix3x3 matrix_Y = {
					   {  cosY,  0, sinY },
					   {     0,  1,    0 },
					   { -sinY,  0, cosY } };

			// Z軸回転の行列
			const Matrix3x3 matrix_Z = {
							 {  cosZ, sinZ, 0 },
							 { -sinZ, cosZ, 0 },
							 {     0,    0, -1 } };

			// X,Y,Z軸回転を掛け合わせて
			// XYZ軸回転の行列にする
			return matrix_X * matrix_Y * matrix_Z;
		}
	}

#pragma endregion

#pragma region Mat4_Function

	namespace Mat4_Function
	{
		/// <summary>
		/// 座標変換行列を作成
		/// </summary>
		/// <param name="scale"> 拡大率 </param>
		/// <param name="rotation"> 回転角度 </param>
		/// <param name="position"> 平行移動量 </param>
		/// <returns> 作成した座標変換行列 </returns>
		inline Matrix4x4 GetTransformMatrix(
			const Vector3& scale, const Vector3& rotation, const Vector3& position)
		{
			// 回転を反映
			const Matrix3x3 m = Mat3_Function::GetRotationMatrix(rotation);

			// 拡大率と平行移動量を反映
			return {
				Vector4{ scale.x * m.data[0], 0 },
				Vector4{ scale.y * m.data[1], 0 },
				Vector4{ scale.z * m.data[2], 0 },
				Vector4{ position, 1 } };
		}

		/// <summary>
		/// 4x4行列から拡大率を抽出する
		/// </summary>
		/// <param name="transform"> 拡大率を抽出したい行列 </param>
		/// <returns> 拡大率 </returns>
		inline Vector3 ExtractScale(const Matrix4x4& transform)
		{
			// 左上3x3部分の各行を3Dベクトルとして長さを求めることで
			// 拡大率を求める
			Vector3 a = Vector3(transform[0]);
			Vector3 b = Vector3(transform[1]);
			Vector3 c = Vector3(transform[2]);

			const float sx = a.Length();
			const float sy = b.Length();
			const float sz = c.Length();

			return { sx, sy, sz };
		}
	}

#pragma endregion

}

#endif // !MATRIX_H_INCLUDED
