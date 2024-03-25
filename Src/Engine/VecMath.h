/**
* @file VecMath.h
*/
#ifndef VECMATH_H_INCLUDED
#define VECMATH_H_INCLUDED

#include "ImGui/ImGui.h"

#include <cmath>

namespace PokarinEngine
{
	namespace Info_Vec2
	{
		// Vec2の要素数
		constexpr int count = 2;
	}

	namespace Info_Vec3
	{
		// Vec3の要素数
		constexpr int count = 3;
	}

	namespace Info_Vec4
	{
		// Vec4の要素数
		constexpr int count = 4;
	}

	// -----------------
	// 前方宣言
	// -----------------

	// ------- ベクトル型 -------

	struct Vec2;
	struct Vec3;
	struct Vec4;

	// --------- 行列型 ---------

	struct Mat3;
	struct Mat4;

	// 円周率
	constexpr float pi = 3.1415926535f;

	// -----------------
	// 構造体定義
	// -----------------

#pragma region Vector

/// <summary>
/// 2次元座標
/// </summary>
	struct Vec2
	{
		float x, y;

		// デフォルトコンストラクタ
		Vec2() = default;

		/// <summary>
		/// Vec3型で初期化するコンストラクタ
		/// </summary>
		explicit constexpr Vec2(const Vec3& v);

		/// <summary>
		/// Vec4型で初期化するコンストラクタ
		/// </summary>
		explicit constexpr Vec2(const Vec4& v);

		/// <summary>
		/// float型で初期化するコンストラクタ
		/// </summary>
		explicit constexpr Vec2(float n) : x(n), y(n) {}

		/// <summary>
		/// float型で初期化するコンストラクタ
		/// </summary>
		constexpr Vec2(float _x, float _y) : x(_x), y(_y) {}

		// ------------------------
		// 添字演算子
		// ------------------------

		float& operator[](size_t i) { return *(&x + i); }
		float operator[](size_t i)const { return *(&x + i); }

		// ------------------------
		// 変換
		// ------------------------

		operator ImVec2() { return ImVec2(x, y); }
	};

	/// <summary>
	/// 3次元座標
	/// </summary>
	struct Vec3
	{
		float x, y, z;

		// デフォルトコンストラクタ
		Vec3() = default;

		/// <summary>
		/// Vec4で初期化するコンストラクタ
		/// (暗黙的な変換を禁止)
		/// </summary>
		explicit constexpr Vec3(const Vec4& v);

		/// <summary>
		/// float型で初期化するコンストラクタ
		/// (暗黙的な変換を禁止)
		/// </summary>
		explicit constexpr Vec3(float n) : x(n), y(n), z(n) {}

		/// <summary>
		/// float型で初期化するコンストラクタ
		/// </summary>
		constexpr Vec3(float _x, float _y, float _z) : x(_x), y(_y), z(_z) {}

		/// <summary>
		/// Vec2型とfloat型で初期化するコンストラクタ
		/// </summary>
		constexpr Vec3(const Vec2& v, float _z) : x(v.x), y(v.y), z(_z) {}

		// ------------------------
		// 添字演算子
		// ------------------------

		float& operator[](size_t i) { return *(&x + i); }
		float operator[](size_t i)const { return *(&x + i); }
	};

	/// <summary>
	/// 4次元座標
	/// </summary>
	struct Vec4
	{
		float x, y, z, w;

		// デフォルトコンストラクタ
		Vec4() = default;

		/// <summary>
		/// float型で初期化するコンストラクタ
		/// </summary>
		explicit constexpr Vec4(float n) : x(n), y(n), z(n), w(n) {}

		/// <summary>
		/// float型で初期化するコンストラクタ
		/// </summary>
		constexpr Vec4(float _x, float _y, float _z, float _w)
			: x(_x), y(_y), z(_z), w(_w) {}

		/// <summary>
		/// Vec2型とfloat型で初期化するコンストラクタ
		/// </summary>
		constexpr Vec4(const Vec2& v, float _z, float _w)
			: x(v.x), y(v.y), z(_z), w(_w) {}

		/// <summary>
		/// Vec3型とfloat型で初期化するコンストラクタ
		/// </summary>
		constexpr Vec4(const Vec3& v, float _w)
			: x(v.x), y(v.y), z(v.z), w(_w) {}

		// ------------------
		// 添字演算子
		// ------------------

		float& operator[](size_t i) { return *(&x + i); }
		float operator[](size_t i)const { return *(&x + i); }
	};

#pragma endregion

#pragma region Matrix

	/// <summary>
	/// 3x3行列
	/// </summary>
	struct Mat3
	{
		Vec3 data[3];

		// ---------------------
		// コンストラクタ
		// ---------------------

		// デフォルトコンストラクタ
		Mat3() = default;

		/// <summary>
		/// Vec3型で初期化するコンストラクタ
		/// </summary>
		Mat3(const Vec3& v0, const Vec3& v1, const Vec3& v2)
		{
			data[0] = v0;
			data[1] = v1;
			data[2] = v2;
		}

		/// <summary>
		/// float型で初期化するコンストラクタ
		/// </summary>
		explicit Mat3(float f)
		{
			data[0] = { f, 0, 0 };
			data[1] = { 0, f, 0 };
			data[2] = { 0, 0, f };
		}

		/// <summary>
		/// Mat4型で初期化するコンストラクタ
		/// </summary>
		explicit Mat3(const struct Mat4& m);

		// ---------------------
		// 添字演算子
		// ---------------------

		Vec3& operator[](size_t i) { return data[i]; }
		const Vec3& operator[](size_t i) const { return data[i]; }
	};

	/// <summary>
	/// 4x4行列
	/// </summary>
	struct Mat4
	{
		Vec4 data[4];

		// --------------------
		// コンストラクタ
		// --------------------

		// デフォルトコンストラクタ
		Mat4() = default;

		/// <summary>
		/// Vec4型で初期化するコンストラクタ
		/// </summary>
		Mat4(const Vec4& v0, const Vec4& v1, const Vec4& v2, const Vec4& v3)
		{
			data[0] = v0;
			data[1] = v1;
			data[2] = v2;
			data[3] = v3;
		}

		/// <summary>
		/// float型で初期化するコンストラクタ
		/// </summary>
		explicit Mat4(float f)
		{
			data[0] = { f, 0, 0, 0 };
			data[1] = { 0, f, 0, 0 };
			data[2] = { 0, 0, f ,0 };
			data[3] = { 0, 0, 0, f };
		}

		/// <summary>
		/// Mat3型で初期化するコンストラクタ
		/// </summary>
		explicit Mat4(const struct Mat3& m)
		{
			data[0] = { m.data[0], 0 };
			data[1] = { m.data[1], 0 };
			data[2] = { m.data[2], 0 };
			data[3] = { 0, 0, 0, 1 };
		}

		// ------------------
		// 添字演算子
		// ------------------

		Vec4& operator[](size_t i) { return data[i]; }
		const Vec4& operator[](size_t i) const { return data[i]; }
	};

#pragma endregion

	// -----------------
	// オペレーター
	// -----------------

#pragma region Vec2_Operator

	// -------
	// 単項
	// -------
	
	// --------- Vec2の -演算子 -----------

	inline Vec2 operator-(const Vec2& a)
	{
		return { -a.x, -a.y };
	}

	// --------------
	// 加算
	// --------------

	// --------- Vec2同士の加算 ----------

	inline Vec2& operator+=(Vec2& a, const Vec2& b)
	{
		a.x += b.x;
		a.y += b.y;

		return a;
	}

	inline Vec2 operator+(const Vec2& a, const Vec2& b)
	{
		Vec2 n = a;
		return n += b;
	}

	// --------- Vec2とfloatの加算 -----------

	inline Vec2& operator+=(Vec2& a, float b)
	{
		return a += { b, b };
	}

	inline Vec2 operator+(const Vec2& a, float b)
	{
		Vec2 n = a;
		return n += b;
	}

	inline Vec2 operator+(float a, const Vec2& b)
	{
		return b + a;
	}

	// --------------
	// 減算
	// --------------

	// ---------- Vec2同士の減算 -----------

	inline Vec2& operator-=(Vec2& a, const Vec2& b)
	{
		a.x -= b.x;
		a.y -= b.y;

		return a;
	}

	inline Vec2 operator-(const Vec2& a, const Vec2& b)
	{
		Vec2 n = a;
		return n -= b;
	}

	// ---------- Vec2とfloatの減算 -----------

	inline Vec2& operator-=(Vec2& a, float b)
	{
		return a -= { b, b };
	}

	inline Vec2 operator-(const Vec2& a, float b)
	{
		Vec2 n = a;
		return n -= b;
	}

	inline Vec2 operator-(float a, const Vec2& b)
	{
		return Vec2{ a, a } - b;
	}

	// --------------
	// 乗算
	// --------------

	// --------- Vec2同士の乗算 -----------

	inline Vec2& operator*=(Vec2& a, const Vec2& b)
	{
		a.x *= b.x;
		a.y *= b.y;

		return a;
	}

	inline Vec2 operator*(const Vec2& a, const Vec2& b)
	{
		Vec2 n = a;
		return n *= b;
	}

	// -------- Vec2とfloatの乗算 ----------

	inline Vec2 operator*=(Vec2& a, float b)
	{
		return a *= { b, b };
	}

	inline Vec2 operator*(const Vec2& a, float b)
	{
		Vec2 n = a;
		return n *= b;
	}

	inline Vec2 operator*(float a, const Vec2& b)
	{
		return b * a;
	}

	// ---------------
	// 除算
	// ---------------

	// ---------- Vec2同士の除算 -----------

	inline Vec2& operator/=(Vec2& a, const Vec2& b)
	{
		if (b.x != 0)
		{
			a.x /= b.x;
		}

		if (b.y != 0)
		{
			a.y /= b.y;
		}

		return a;
	}

	inline Vec2 operator/(const Vec2& a, const Vec2& b)
	{
		Vec2 n = a;
		return n /= b;
	}

	// ---------- Vec2とfloatの減算 -----------

	inline Vec2& operator/=(Vec2& a, float b)
	{
		return a /= { b, b };
	}

	inline Vec2 operator/(const Vec2& a, float b)
	{
		Vec2 n = a;
		return n /= b;
	}

	inline Vec2 operator/(float a, const Vec2& b)
	{
		return Vec2{ a, a } / b;
	}

#pragma endregion

#pragma region Vec3_Operator

	// --------------------
	// 単項
	// --------------------

	// --------- Vec3の -演算子 -----------

	inline Vec3 operator-(const Vec3& a)
	{
		return { -a.x, -a.y, -a.z };
	}

	// --------------
	// 加算
	// --------------

	// --------- Vec3同士の加算 ----------

	inline Vec3& operator+=(Vec3& a, const Vec3& b)
	{
		a.x += b.x;
		a.y += b.y;
		a.z += b.z;

		return a;
	}

	inline Vec3 operator+(const Vec3& a, const Vec3& b)
	{
		Vec3 n = a;
		return n += b;
	}

	// --------- Vec3とfloatの加算 -----------

	inline Vec3& operator+=(Vec3& a, float b)
	{
		return a += { b, b, b };
	}

	inline Vec3 operator+(const Vec3& a, float b)
	{
		Vec3 n = a;
		return n += b;
	}

	inline Vec3 operator+(float a, const Vec3& b)
	{
		return b + a;
	}

	// --------------
	// 減算
	// --------------

	// ---------- Vec3同士の減算 -----------

	inline Vec3& operator-=(Vec3& a, const Vec3& b)
	{
		a.x -= b.x;
		a.y -= b.y;
		a.z -= b.z;

		return a;
	}

	inline Vec3 operator-(const Vec3& a, const Vec3& b)
	{
		Vec3 n = a;
		return n -= b;
	}

	// ---------- Vec3とfloatの減算 -----------

	inline Vec3& operator-=(Vec3& a, float b)
	{
		return a -= { b, b, b };
	}

	inline Vec3 operator-(const Vec3& a, float b)
	{
		Vec3 n = a;
		return n -= b;
	}

	inline Vec3 operator-(float a, const Vec3& b)
	{
		return Vec3{ a, a, a } - b;
	}

	// --------------
	// 乗算
	// --------------

	// --------- Vec3同士の乗算 -----------

	inline Vec3& operator*=(Vec3& a, const Vec3& b)
	{
		a.x *= b.x;
		a.y *= b.y;
		a.z *= b.z;

		return a;
	}

	inline Vec3 operator*(const Vec3& a, const Vec3& b)
	{
		Vec3 n = a;
		return n *= b;
	}

	// -------- Vec3とfloatの乗算 ----------

	inline Vec3 operator*=(Vec3& a, float b)
	{
		return a *= { b, b, b };
	}

	inline Vec3 operator*(const Vec3& a, float b)
	{
		Vec3 n = a;
		return n *= b;
	}

	inline Vec3 operator*(float a, const Vec3& b)
	{
		return b * a;
	}

	// ---------------
	// 除算
	// ---------------

	// ---------- Vec3同士の除算 -----------

	inline Vec3& operator/=(Vec3& a, const Vec3& b)
	{
		if (b.x != 0)
		{
			a.x /= b.x;
		}

		if (b.y != 0)
		{
			a.y /= b.y;
		}

		if (b.z != 0)
		{
			a.z /= b.z;
		}

		return a;
	}

	inline Vec3 operator/(const Vec3& a, const Vec3& b)
	{
		Vec3 n = a;
		return n /= b;
	}

	// ---------- Vec3とfloatの除算 -----------

	inline Vec3& operator/=(Vec3& a, float b)
	{
		return a /= { b, b, b };
	}

	inline Vec3 operator/(const Vec3& a, float b)
	{
		Vec3 n = a;
		return n /= b;
	}

	inline Vec3 operator/(float a, const Vec3& b)
	{
		return Vec3{ a, a, a } / b;
	}

#pragma endregion

#pragma region Vec4_Operator

	// -----------
	// 単項
	// -----------

	// --------- Vec4の -演算子 ----------

	inline Vec4 operator-(const Vec4& a)
	{
		return { -a.x, -a.y, -a.z, -a.w };
	}

	// ------------
	// 加算
	// ------------

	// --------- Vec4同士の加算 ------------

	inline Vec4& operator+=(Vec4& a, const Vec4& b)
	{
		a.x += b.x;
		a.y += b.y;
		a.z += b.z;
		a.w += b.w;

		return a;
	}

	inline Vec4 operator+(const Vec4& a, const Vec4& b)
	{
		Vec4 n = a;
		return n += b;
	}

	// --------- Vec4とfloatの加算 -----------

	inline Vec4& operator+=(Vec4& a, float b)
	{
		return a += { b, b, b, b };
	}

	inline Vec4 operator+(const Vec4& a, float b)
	{
		Vec4 n = a;
		return n += b;
	}

	// --------------
	// 減算
	// --------------

	// ---------- Vec4同士の減算 -----------

	inline Vec4& operator-=(Vec4& a, const Vec4& b)
	{
		a.x -= b.x;
		a.y -= b.y;
		a.z -= b.z;
		a.w -= b.w;

		return a;
	}

	inline Vec4 operator-(const Vec4& a, const Vec4& b)
	{
		Vec4 n = a;
		return n -= b;
	}

	// ---------- Vec4とfloatの減算 -----------

	inline Vec4& operator-=(Vec4& a, float b)
	{
		return a -= { b, b, b, b };
	}

	inline Vec4 operator-(const Vec4& a, float b)
	{
		Vec4 n = a;
		return n -= b;
	}

	inline Vec4 operator-(float a, const Vec4& b)
	{
		return Vec4{ a, a, a, a } - b;
	}

	// --------------
	// 乗算
	// --------------

	// --------- Vec4同士の乗算 -----------

	inline Vec4& operator*=(Vec4& a, const Vec4& b)
	{
		a.x *= b.x;
		a.y *= b.y;
		a.z *= b.z;
		a.w *= b.w;

		return a;
	}

	inline Vec4 operator*(const Vec4& a, const Vec4& b)
	{
		Vec4 n = a;
		return n *= b;
	}

	// -------- Vec4とfloatの乗算 ----------

	inline Vec4 operator*=(Vec4& a, float b)
	{
		return a *= { b, b, b, b };
	}

	inline Vec4 operator*(const Vec4& a, float b)
	{
		Vec4 n = a;
		return n *= b;
	}

	inline Vec4 operator*(float a, const Vec4& b)
	{
		return b * a;
	}

	// ---------------
	// 除算
	// ---------------

	// ---------- Vec4同士の除算 -----------

	inline Vec4& operator/=(Vec4& a, const Vec4& b)
	{
		if (b.x != 0)
		{
			a.x /= b.x;
		}

		if (b.y != 0)
		{
			a.y /= b.y;
		}

		if (b.z != 0)
		{
			a.z /= b.z;
		}

		if (b.w != 0)
		{
			a.w /= b.w;
		}

		return a;
	}

	inline Vec4 operator/(const Vec4& a, const Vec4& b)
	{
		Vec4 n = a;
		return n /= b;
	}

	// ---------- Vec4とfloatの除算 -----------

	inline Vec4& operator/=(Vec4& a, float b)
	{
		return a /= { b, b, b, b };
	}

	inline Vec4 operator/(const Vec4& a, float b)
	{
		Vec4 n = a;
		return n /= b;
	}

	inline Vec4 operator/(float a, const Vec4& b)
	{
		return Vec4{ a, a, a, a } / b;
	}

#pragma endregion

#pragma region Mat3_Operator

	// -----------------
	// 乗算
	// -----------------

	// --------- Mat3とVec3の乗算 ---------

	inline Vec3 operator*(const Mat3& m, const Vec3& v)
	{
		return m.data[0] * v.x + m.data[1] * v.y + m.data[2] * v.z;
	}

	// ---------- Mat3同士の乗算 ----------

	inline Mat3 operator*(const Mat3& a, const Mat3& b)
	{
		Mat3 m = Mat3(0);

		m.data[0] = a * b.data[0];
		m.data[1] = a * b.data[1];
		m.data[2] = a * b.data[2];

		return m;
	}

	inline Mat3& operator*=(Mat3& a, const Mat3& b)
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

	inline Vec4 operator*(const Mat4& m, const Vec4& v)
	{
		return m.data[0] * v.x + m.data[1] * v.y + m.data[2] * v.z + m.data[3] * v.w;
	}

	// ---------- Mat4同士の乗算 -----------

	inline Mat4 operator*(const Mat4& a, const Mat4& b)
	{
		Mat4 m = Mat4(0);

		m.data[0] = a * b.data[0];
		m.data[1] = a * b.data[1];
		m.data[2] = a * b.data[2];
		m.data[3] = a * b.data[3];

		return m;
	}

	inline Mat4& operator*=(Mat4& a, const Mat4& b)
	{
		a = a * b;
		return a;
	}

#pragma endregion

	// ------------
	// 関数
	// ------------

#pragma region Radians_Degrees

/// <summary>
/// 度数法を弧度法に変換する
/// </summary>
/// <param name="deg"> 角度(度数法) </param>
/// <returns> 角度(弧度法) </returns>
	constexpr float Radians(float deg)
	{
		return deg * pi / 180.0f;
	}

	/// <summary>
	/// 度数法を弧度法に変換する
	/// </summary>
	/// <param name="deg"> 角度(度数法) </param>
	/// <returns> 角度(弧度法) </returns>
	constexpr Vec3 Radians(Vec3 deg)
	{
		return Vec3(Radians(deg.x), Radians(deg.y), Radians(deg.z));
	}

	/// <summary>
	/// 弧度法を度数法に変換する
	/// </summary>
	/// <param name="rad"> 角度(弧度法) </param>
	/// <returns> 角度(度数法) </returns>
	constexpr float Degrees(float rad)
	{
		return rad * 180.0f / pi;
	}

	/// <summary>
	/// 弧度法を度数法に変換する
	/// </summary>
	/// <param name="rad"> 角度(弧度法) </param>
	/// <returns> 角度(度数法) </returns>
	constexpr Vec3 Degrees(Vec3 rad)
	{
		return Vec3(Degrees(rad.x), Degrees(rad.y), Degrees(rad.z));
	}

#pragma endregion

#pragma region Vec2_Function

	/// <summary>
	/// Vec3型で初期化するコンストラクタ
	/// </summary>
	constexpr Vec2::Vec2(const Vec3& v)
		:x(v.x), y(v.y)
	{

	}

	/// <summary>
	/// Vec4型で初期化するコンストラクタ
	/// </summary>
	constexpr Vec2::Vec2(const Vec4& v)
		:x(v.x), y(v.y)
	{

	}

#pragma endregion

#pragma region Vec3_Function

	/// <summary>
	/// Vec4をVec3に変換するコンストラクタ
	/// (暗黙的な変換を禁止)
	/// </summary>
	constexpr Vec3::Vec3(const Vec4& v)
		:x(v.x), y(v.y), z(v.z)
	{

	}

	namespace Vec3_Function
	{
		/// <summary>
		/// Vec3の内積
		/// </summary>
		/// <param name="a"> ベクトルA </param>
		/// <param name="b"> ベクトルB </param>
		/// <returns> 内積の結果 </returns>
		inline float Dot(const Vec3& a, const Vec3& b)
		{
			return a.x * b.x + a.y * b.y + a.z * b.z;
		}

		/// <summary>
		/// Vec3の長さ
		/// </summary>
		/// <param name="a"> ベクトル </param>
		/// <returns> ベクトルの長さ </returns>
		inline float Length(const Vec3& a)
		{
			return sqrt(Dot(a, a));
		}

		/// <summary>
		/// Vec3の正規化
		/// </summary>
		/// <param name="a"> ベクトル </param>
		/// <returns> 単位ベクトル </returns>
		inline Vec3 Normalize(const Vec3& a)
		{
			return a / Length(a);
		}
	}

#pragma endregion

#pragma region Vec4_Function

	namespace Vec4_Function
	{
		/// <summary>
		/// 座標変換ベクトルの配列を計算する
		/// </summary>
		/// <param name="[out] result"> 計算結果(要素数4の配列) </param>
		/// <param name="[in] scale"> 拡大率 </param>
		/// <param name="[in] rotation"> 回転角度 </param>
		/// <param name="[in] position"> 平行移動量 </param>
		inline void GetTransformVectors(Vec4* result,
			const Vec3& scale, const Vec3& rotation, const Vec3& position)
		{
			/* 拡大率、Z軸X軸Y軸の回転、平行移動を反映するための計算をまとめる

			x''' = v.x * scale.x *  cosZ * cosY * sinZ * sinX * sinY
				 + v.y * scale.y * -sinZ * cosY * cosZ * sinX * sinY
				 + v.z * scale.z *  cosX * sinY
				 + position.x

			y''' = v.x * scale.x *  sinZ * cosX
				 + v.y * scale.y *  cosZ * cosX
				 + v.z * scale.z * -sinX
				 + position.y

			z''' = v.x * scale.x *  cosZ * -sinY * sinZ * sinX * cosY
				 + v.y * scale.y * -sinZ * -sinY * cosZ * sinX * cosY
				 + v.z * scale.z *  cosX * cosY
				 + position.z

			式を1行にまとめたがかなり見づらい
			「v」に掛ける成分をベクトル「m0, m1, m2」として定義し、
			内積で計算ができるように頂点座標にw要素を追加する
			(v.w = 1とする)

			x''' = v.x * m0.x + v.y * m0.y + v.z * m0.z + v.w * m0.w
			y''' = v.x * m1.x + v.y * m1.y + v.z * m1.z + v.w * m1.w
			z''' = v.x * m2.x + v.y * m2.y + v.z * m2.z + v.w * m2.w
			w''' = v.x * m3.x + v.y * m3.y + v.z * m3.z + v.w * m3.w */

			// -------------------------------------------------
			// m0, m1, m2, m3を求め、resultに格納する
			// -------------------------------------------------

			// X軸回転のSin・Cos
			const float sinX = sin(rotation.x), cosX = cos(rotation.x);

			// Y軸回転のSin・Cos
			const float sinY = sin(rotation.y), cosY = cos(rotation.y);

			// Z軸回転のSin・Cos
			const float sinZ = sin(rotation.z), cosZ = cos(rotation.z);

			// m0を計算
			result[0] = { scale.x * sinX * sinY * cosY * sinZ * cosZ,
						  scale.y * sinX * sinY * cosY * -sinZ * cosZ,
						  scale.z * cosX * sinY,
						  position.x };

			// m1を計算
			result[1] = { scale.x * cosX * sinZ,
						  scale.y * cosX * cosZ,
						  scale.z * -sinX,
						  position.y };

			// m2を計算
			result[2] = { scale.x * sinX * -sinY * cosY * sinZ * cosZ,
						  scale.y * sinX * -sinY * cosY * -sinZ * cosZ,
						  scale.z * cosX * cosY,
						  position.z };

			// m3を計算
			// w = 1になるようにする
			result[3] = { 0, 0, 0, 1 };
		}

		/// <summary>
		/// 法線変換ベクトルの配列を計算する
		/// </summary>
		/// <param name="[out] result"> 計算結果(要素数3の配列) </param>
		/// <param name="[in] rotation"> 回転角度 </param>
		inline void GetRotationVectors(Vec3* result, const Vec3& rotation)
		{
			/* GetTransformVectorsの「scale = 1, position = 0」バージョン
			回転だけなのでw要素は不要 */

			// X軸回転のSin・Cos
			const float sinX = sin(rotation.x), cosX = cos(rotation.x);

			// Y軸回転のSin・Cos
			const float sinY = sin(rotation.y), cosY = cos(rotation.y);

			// Z軸回転のSin・Cos
			const float sinZ = sin(rotation.z), cosZ = cos(rotation.z);

			result[0] = { sinX * sinY * cosY * sinZ * cosZ,
						  sinX * sinY * cosY * -sinZ * cosZ,
						  cosX * sinY };

			result[1] = { cosX * sinZ,
						  cosX * cosZ,
						  -sinX };

			result[2] = { sinX * -sinY * cosY * sinZ * cosZ,
						  sinX * -sinY * cosY * -sinZ * cosZ,
						  cosX * cosY };
		}

		/// <summary>
		/// 座標変換ベクトルの配列を掛け合わせる
		/// </summary>
		/// <param name="[out] result"> 計算結果(要素数4の配列) </param>
		/// <param name="[in] m"> 座標変換ベクトルの配列1 </param>
		/// <param name="[in] n"> 座標変換ベクトルの配列2 </param>
		inline void MultiplyTransformVectors(
			Vec4* result, const Vec4* m, const Vec4* n)
		{
			/* 親の座標変換を追加する
			計算自体に親子による違いはないので、
			「m0 ～ m3」と同様のベクトル「n0 ～ n3」を定義できる
			(x''', y''', z''', w'''と同じ式になる)

			x2 = x''' * n0.x + y''' * n0.y + z''' * n0.z + w''' * n0.w
			y2 = x''' * n1.x + y''' * n1.y + z''' * n1.z + w''' * n1.w
			z2 = x''' * n2.x + y''' * n2.y + z''' * n2.z + w''' * n2.w
			w2 = x''' * n3.x + y''' * n3.y + z''' * n3.z + w''' * n3.w

			x'''を展開すると
			x2は「m0～m3とn0を掛けたものを頂点座標に掛けている」ということが分かる
			それぞれ展開すると同じようになっている
			「m0～m3とn0を掛けたもの」をベクトルmnとして定義する

			x2' = v.x * mn0.x + v.y * mn0.y + v.z * mn0.z + v.w * mn0.w
			y2' = v.x * mn1.x + v.y * mn1.y + v.z * mn1.z + v.w * mn1.w
			z2' = v.x * mn2.x + v.y * mn2.y + v.z * mn2.z + v.w * mn2.w
			w2' = v.x * mn3.x + v.y * mn3.y + v.z * mn3.z + v.w * mn3.w

			CPU側でmnを定義すれば
			簡単な計算で親子階層を考慮した座標変換ができる

			これ以上の親を設定する場合も、
			mやnと同じようにベクトルを定義し
			前の計算結果に掛けることで座標変換に必要なベクトルを求められる */

			// ---------------------------------------
			// 座標変換に必要なベクトルmnを求める
			// ---------------------------------------

			// 計算結果を一度この変数に入れる
			// resultとm,nには同じアドレスを指定出来るため、
			// 直接代入すると元のデータを壊してしまう
			Vec4 mn[4] = { Vec4(0) };

			// ベクトルmnを求める
			for (int i = 0; i < 4; ++i)
			{
				mn[i] = m[0] * n[i].x + m[1] * n[i].y + m[2] * n[i].z + m[3] * n[i].w;
			}

			// 計算結果を代入
			for (int i = 0; i < 4; ++i)
			{
				result[i] = mn[i];
			}
		}

		/// <summary>
		/// 法線変換ベクトルの配列を掛け合わせる
		/// </summary>
		/// <param name="result"> 計算結果(要素数3の配列) </param>
		/// <param name="m"> 法線変換ベクトルの配列1 </param>
		/// <param name="n"> 法線変換ベクトルの配列2 </param>
		inline void MultiplyRotationVectors(
			Vec3* result, const Vec3* m, const Vec3* n)
		{
			// 元データ破壊を回避するため
			// 一度この変数に計算結果を代入する
			Vec3 mn[3] = { Vec3(0) };

			// ベクトルmnを求める
			for (int i = 0; i < 3; ++i)
			{
				mn[i] = m[0] * n[i].x + m[1] * n[i].y + m[2] * m[i].z;
			}

			// 計算結果を代入
			for (int i = 0; i < 3; ++i)
			{
				result[i] = mn[i];
			}
		}
	}

#pragma endregion

#pragma region Mat3_Function

	/// <summary>
	/// Mat4型で初期化するコンストラク
	/// </summary>
	inline Mat3::Mat3(const Mat4& m)
	{
		data[0] = Vec3(m.data[0]);
		data[1] = Vec3(m.data[1]);
		data[2] = Vec3(m.data[2]);
	}

	namespace Mat3_Function
	{
		/// <summary>
		/// 回転角度から回転行列を作成
		/// </summary>
		/// <param name="rotation"> 回転角度 </param>
		/// <returns> 作成した回転行列 </returns>
		inline Mat3 GetRotationMatrix(const Vec3& rotation)
		{
			/* 行列は、ベクトル配列と比べて
			「縦と横が反転している」ので

			{  cosY, sinX * sinY, cosX * sinY }
			{     0,        cosX,       -sinX }
			{ -sinY, sinX * cosY, cosX * cosY }

							↓

			{        cosY,     0,       -sinY }
			{ sinX * sinY,  cosX, sinX * cosY }
			{ cosX * sinY, -sinX, cosX * cosY }

			になる

			XY軸回転の行列は
		　　「X軸回転の行列」に「Y軸回転の行列」を掛け合わせたもの
			なので、Z軸回転を追加するには「XY軸回転の行列」に「Z軸回転の行列」を掛け合わせればいい */

			// X軸回転のSin・Cos
			const float sinX = sin(rotation.x), cosX = cos(rotation.x);

			// Y軸回転のSin・Cos
			const float sinY = sin(rotation.y), cosY = cos(rotation.y);

			// Z軸回転のSin・Cos
			const float sinZ = sin(rotation.z), cosZ = cos(rotation.z);

			// XY軸回転の行列
			Mat3 m = { {        cosY,     0,        -sinY },
					   { sinX * sinY,  cosX,  sinX * cosY },
					   { cosX * sinY, -sinX,  cosX * cosY } };

			// Z軸回転の行列
			const Mat3 n = { {  cosZ, sinZ, 0 },
							 { -sinZ, cosZ, 0 },
							 {     0,    0, 1 } };

			// XY軸回転にZ軸回転を掛け合わせて
			// XYZ軸回転の行列にする
			return m * n;
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
		inline Mat4 GetTransformMatrix(
			const Vec3& scale, const Vec3& rotation, const Vec3& position)
		{
			// 回転を反映
			const Mat3 m = Mat3_Function::GetRotationMatrix(rotation);

			// 拡大率と平行移動量を反映
			return {
				Vec4{ scale.x * m.data[0], 0 },
				Vec4{ scale.y * m.data[1], 0 },
				Vec4{ scale.z * m.data[2], 0 },
				Vec4{ position, 1 } };
		}

		/// <summary>
		/// 4x4行列から拡大率を抽出する
		/// </summary>
		/// <param name="transform"> 拡大率を抽出したい行列 </param>
		/// <returns> 拡大率 </returns>
		inline Vec3 ExtractScale(const Mat4& transform)
		{
			// 左上3x3部分の各行を3Dベクトルとして長さを求めることで
			// 拡大率を求める
			const float sx = Vec3_Function::Length(Vec3(transform[0]));
			const float sy = Vec3_Function::Length(Vec3(transform[1]));
			const float sz = Vec3_Function::Length(Vec3(transform[2]));

			return { sx, sy, sz };
		}
	}

#pragma endregion

} // namespace PokarinEngine

#endif // !VECMATH_H_INCLUDED