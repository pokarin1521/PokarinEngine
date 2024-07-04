/**
* @file Vector.h
*/
#ifndef VECTOR_H_INCLUDED
#define VECTOR_H_INCLUDED

#include <cmath>

namespace PokarinEngine
{
	// ----------------------------
	// 前方宣言
	// ----------------------------

	struct Vector2;
	struct Vector3;
	struct Vector4;

	// -------------------------------
	// 構造体定義
	// -------------------------------

	/// <summary>
	/// 2次元座標
	/// </summary>
	struct Vector2
	{
	public: // -------------------- コンストラクタ --------------------

		// デフォルトコンストラクタ
		Vector2() = default;

		/// <summary>
		/// Vec3型で初期化するコンストラクタ
		/// </summary>
		explicit constexpr Vector2(const Vector3& v);

		/// <summary>
		/// Vec4型で初期化するコンストラクタ
		/// </summary>
		explicit constexpr Vector2(const Vector4& v);

		/// <summary>
		/// float型で初期化するコンストラクタ
		/// </summary>
		explicit constexpr Vector2(float n) : x(n), y(n) {}

		/// <summary>
		/// float型で初期化するコンストラクタ
		/// </summary>
		constexpr Vector2(float _x, float _y) : x(_x), y(_y) {}

	public: // ---------------------- 添字演算子 ----------------------

		float& operator[](size_t i) { return *(&x + i); }
		float operator[](size_t i)const { return *(&x + i); }

	public: // ------------------------- 情報 -------------------------

		float x, y;

		// 要素数
		static constexpr int size = 2;
	};

	/// <summary>
	/// 3次元座標
	/// </summary>
	struct Vector3
	{
	public: // -------------------- コンストラクタ --------------------

		// デフォルトコンストラクタ
		Vector3() = default;

		/// <summary>
		/// Vec4で初期化するコンストラクタ
		/// (暗黙的な変換を禁止)
		/// </summary>
		explicit constexpr Vector3(const Vector4& v);

		/// <summary>
		/// float型で初期化するコンストラクタ
		/// (暗黙的な変換を禁止)
		/// </summary>
		explicit constexpr Vector3(float n) : x(n), y(n), z(n) {}

		/// <summary>
		/// float型で初期化するコンストラクタ
		/// </summary>
		constexpr Vector3(float _x, float _y, float _z) : x(_x), y(_y), z(_z) {}

		/// <summary>
		/// Vec2型とfloat型で初期化するコンストラクタ
		/// </summary>
		constexpr Vector3(const Vector2& v, float _z) : x(v.x), y(v.y), z(_z) {}

	public: // ------------------------- 関数 -------------------------

		/// <summary>
		/// ベクトルの内積を求める
		/// </summary>
		/// <param name="[in] a"> ベクトルA </param>
		/// <param name="[in] b"> ベクトルB </param>
		/// <returns> ベクトルの内積 </returns>
		static float Dot(const Vector3& a, const Vector3& b);

		/// <summary>
		/// ベクトルの大きさ(2乗値)を求める
		/// </summary>
		/// <returns> ベクトルの大きさ(2乗値) </returns>
		inline float SprMagnitude() const;

		/// <summary>
		/// ベクトルの大きさを求める
		/// </summary>
		/// <returns> ベクトルの大きさ </returns>
		inline float Length() const;

		/// <summary>
		/// ベクトルを正規化する
		/// </summary>
		/// <returns> 単位ベクトル </returns>
		inline Vector3 Normalize() const;

	public: // ---------------------- 添字演算子 ----------------------

		float& operator[](size_t i) { return *(&x + i); }
		float operator[](size_t i)const { return *(&x + i); }

	public: // ------------------------- 情報 -------------------------

		float x, y, z;

		// 要素数
		static constexpr int size = 3;
	};

	/// <summary>
	/// 4次元座標
	/// </summary>
	struct Vector4
	{
	public: // -------------------- コンストラクタ --------------------

		// デフォルトコンストラクタ
		Vector4() = default;

		/// <summary>
		/// float型で初期化するコンストラクタ
		/// </summary>
		explicit constexpr Vector4(float n) : x(n), y(n), z(n), w(n) {}

		/// <summary>
		/// float型で初期化するコンストラクタ
		/// </summary>
		constexpr Vector4(float _x, float _y, float _z, float _w)
			: x(_x), y(_y), z(_z), w(_w) {}

		/// <summary>
		/// Vec2型とfloat型で初期化するコンストラクタ
		/// </summary>
		constexpr Vector4(const Vector2& v, float _z, float _w)
			: x(v.x), y(v.y), z(_z), w(_w) {}

		/// <summary>
		/// Vec3型とfloat型で初期化するコンストラクタ
		/// </summary>
		constexpr Vector4(const Vector3& v, float _w)
			: x(v.x), y(v.y), z(v.z), w(_w) {}

	public: // ---------------------- 添字演算子 ----------------------

		float& operator[](size_t i) { return *(&x + i); }
		float operator[](size_t i)const { return *(&x + i); }

	public: // ------------------------- 情報 -------------------------

		float x, y, z, w;

		// 要素数
		static constexpr int size = 4;
	};

	// -------------------------------
	// オペレーター
	// -------------------------------

#pragma region Vec2_Operator

	// -------
	// 単項
	// -------

	// --------- Vec2の -演算子 -----------

	inline Vector2 operator-(const Vector2& a)
	{
		return { -a.x, -a.y };
	}

	// --------------
	// 加算
	// --------------

	// --------- Vec2同士の加算 ----------

	inline Vector2& operator+=(Vector2& a, const Vector2& b)
	{
		a.x += b.x;
		a.y += b.y;

		return a;
	}

	inline Vector2 operator+(const Vector2& a, const Vector2& b)
	{
		Vector2 n = a;
		return n += b;
	}

	// --------- Vec2とfloatの加算 -----------

	inline Vector2& operator+=(Vector2& a, float b)
	{
		return a += { b, b };
	}

	inline Vector2 operator+(const Vector2& a, float b)
	{
		Vector2 n = a;
		return n += b;
	}

	inline Vector2 operator+(float a, const Vector2& b)
	{
		return b + a;
	}

	// --------------
	// 減算
	// --------------

	// ---------- Vec2同士の減算 -----------

	inline Vector2& operator-=(Vector2& a, const Vector2& b)
	{
		a.x -= b.x;
		a.y -= b.y;

		return a;
	}

	inline Vector2 operator-(const Vector2& a, const Vector2& b)
	{
		Vector2 n = a;
		return n -= b;
	}

	// ---------- Vec2とfloatの減算 -----------

	inline Vector2& operator-=(Vector2& a, float b)
	{
		return a -= { b, b };
	}

	inline Vector2 operator-(const Vector2& a, float b)
	{
		Vector2 n = a;
		return n -= b;
	}

	inline Vector2 operator-(float a, const Vector2& b)
	{
		return Vector2{ a, a } - b;
	}

	// --------------
	// 乗算
	// --------------

	// --------- Vec2同士の乗算 -----------

	inline Vector2& operator*=(Vector2& a, const Vector2& b)
	{
		a.x *= b.x;
		a.y *= b.y;

		return a;
	}

	inline Vector2 operator*(const Vector2& a, const Vector2& b)
	{
		Vector2 n = a;
		return n *= b;
	}

	// -------- Vec2とfloatの乗算 ----------

	inline Vector2 operator*=(Vector2& a, float b)
	{
		return a *= { b, b };
	}

	inline Vector2 operator*(const Vector2& a, float b)
	{
		Vector2 n = a;
		return n *= b;
	}

	inline Vector2 operator*(float a, const Vector2& b)
	{
		return b * a;
	}

	// ---------------
	// 除算
	// ---------------

	// ---------- Vec2同士の除算 -----------

	inline Vector2& operator/=(Vector2& a, const Vector2& b)
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

	inline Vector2 operator/(const Vector2& a, const Vector2& b)
	{
		Vector2 n = a;
		return n /= b;
	}

	// ---------- Vec2とfloatの減算 -----------

	inline Vector2& operator/=(Vector2& a, float b)
	{
		return a /= { b, b };
	}

	inline Vector2 operator/(const Vector2& a, float b)
	{
		Vector2 n = a;
		return n /= b;
	}

	inline Vector2 operator/(float a, const Vector2& b)
	{
		return Vector2{ a, a } / b;
	}

#pragma endregion

#pragma region Vec3_Operator

	// --------------------
	// 単項
	// --------------------

	// --------- Vec3の -演算子 -----------

	inline Vector3 operator-(const Vector3& a)
	{
		return { -a.x, -a.y, -a.z };
	}

	// --------------
	// 加算
	// --------------

	// --------- Vec3同士の加算 ----------

	inline Vector3& operator+=(Vector3& a, const Vector3& b)
	{
		a.x += b.x;
		a.y += b.y;
		a.z += b.z;

		return a;
	}

	inline Vector3 operator+(const Vector3& a, const Vector3& b)
	{
		Vector3 n = a;
		return n += b;
	}

	// --------- Vec3とfloatの加算 -----------

	inline Vector3& operator+=(Vector3& a, float b)
	{
		return a += { b, b, b };
	}

	inline Vector3 operator+(const Vector3& a, float b)
	{
		Vector3 n = a;
		return n += b;
	}

	inline Vector3 operator+(float a, const Vector3& b)
	{
		return b + a;
	}

	// --------------
	// 減算
	// --------------

	// ---------- Vec3同士の減算 -----------

	inline Vector3& operator-=(Vector3& a, const Vector3& b)
	{
		a.x -= b.x;
		a.y -= b.y;
		a.z -= b.z;

		return a;
	}

	inline Vector3 operator-(const Vector3& a, const Vector3& b)
	{
		Vector3 n = a;
		return n -= b;
	}

	// ---------- Vec3とfloatの減算 -----------

	inline Vector3& operator-=(Vector3& a, float b)
	{
		return a -= { b, b, b };
	}

	inline Vector3 operator-(const Vector3& a, float b)
	{
		Vector3 n = a;
		return n -= b;
	}

	inline Vector3 operator-(float a, const Vector3& b)
	{
		return Vector3{ a, a, a } - b;
	}

	// --------------
	// 乗算
	// --------------

	// --------- Vec3同士の乗算 -----------

	inline Vector3& operator*=(Vector3& a, const Vector3& b)
	{
		a.x *= b.x;
		a.y *= b.y;
		a.z *= b.z;

		return a;
	}

	inline Vector3 operator*(const Vector3& a, const Vector3& b)
	{
		Vector3 n = a;
		return n *= b;
	}

	// -------- Vec3とfloatの乗算 ----------

	inline Vector3 operator*=(Vector3& a, float b)
	{
		return a *= { b, b, b };
	}

	inline Vector3 operator*(const Vector3& a, float b)
	{
		Vector3 n = a;
		return n *= b;
	}

	inline Vector3 operator*(float a, const Vector3& b)
	{
		return b * a;
	}

	// ---------------
	// 除算
	// ---------------

	// ---------- Vec3同士の除算 -----------

	inline Vector3& operator/=(Vector3& a, const Vector3& b)
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

	inline Vector3 operator/(const Vector3& a, const Vector3& b)
	{
		Vector3 n = a;
		return n /= b;
	}

	// ---------- Vec3とfloatの除算 -----------

	inline Vector3& operator/=(Vector3& a, float b)
	{
		return a /= { b, b, b };
	}

	inline Vector3 operator/(const Vector3& a, float b)
	{
		Vector3 n = a;
		return n /= b;
	}

	inline Vector3 operator/(float a, const Vector3& b)
	{
		return Vector3{ a, a, a } / b;
	}

#pragma endregion

#pragma region Vec4_Operator

	// -----------
	// 単項
	// -----------

	// --------- Vec4の -演算子 ----------

	inline Vector4 operator-(const Vector4& a)
	{
		return { -a.x, -a.y, -a.z, -a.w };
	}

	// ------------
	// 加算
	// ------------

	// --------- Vec4同士の加算 ------------

	inline Vector4& operator+=(Vector4& a, const Vector4& b)
	{
		a.x += b.x;
		a.y += b.y;
		a.z += b.z;
		a.w += b.w;

		return a;
	}

	inline Vector4 operator+(const Vector4& a, const Vector4& b)
	{
		Vector4 n = a;
		return n += b;
	}

	// --------- Vec4とfloatの加算 -----------

	inline Vector4& operator+=(Vector4& a, float b)
	{
		return a += { b, b, b, b };
	}

	inline Vector4 operator+(const Vector4& a, float b)
	{
		Vector4 n = a;
		return n += b;
	}

	// --------------
	// 減算
	// --------------

	// ---------- Vec4同士の減算 -----------

	inline Vector4& operator-=(Vector4& a, const Vector4& b)
	{
		a.x -= b.x;
		a.y -= b.y;
		a.z -= b.z;
		a.w -= b.w;

		return a;
	}

	inline Vector4 operator-(const Vector4& a, const Vector4& b)
	{
		Vector4 n = a;
		return n -= b;
	}

	// ---------- Vec4とfloatの減算 -----------

	inline Vector4& operator-=(Vector4& a, float b)
	{
		return a -= { b, b, b, b };
	}

	inline Vector4 operator-(const Vector4& a, float b)
	{
		Vector4 n = a;
		return n -= b;
	}

	inline Vector4 operator-(float a, const Vector4& b)
	{
		return Vector4{ a, a, a, a } - b;
	}

	// --------------
	// 乗算
	// --------------

	// --------- Vec4同士の乗算 -----------

	inline Vector4& operator*=(Vector4& a, const Vector4& b)
	{
		a.x *= b.x;
		a.y *= b.y;
		a.z *= b.z;
		a.w *= b.w;

		return a;
	}

	inline Vector4 operator*(const Vector4& a, const Vector4& b)
	{
		Vector4 n = a;
		return n *= b;
	}

	// -------- Vec4とfloatの乗算 ----------

	inline Vector4 operator*=(Vector4& a, float b)
	{
		return a *= { b, b, b, b };
	}

	inline Vector4 operator*(const Vector4& a, float b)
	{
		Vector4 n = a;
		return n *= b;
	}

	inline Vector4 operator*(float a, const Vector4& b)
	{
		return b * a;
	}

	// ---------------
	// 除算
	// ---------------

	// ---------- Vec4同士の除算 -----------

	inline Vector4& operator/=(Vector4& a, const Vector4& b)
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

	inline Vector4 operator/(const Vector4& a, const Vector4& b)
	{
		Vector4 n = a;
		return n /= b;
	}

	// ---------- Vec4とfloatの除算 -----------

	inline Vector4& operator/=(Vector4& a, float b)
	{
		return a /= { b, b, b, b };
	}

	inline Vector4 operator/(const Vector4& a, float b)
	{
		Vector4 n = a;
		return n /= b;
	}

	inline Vector4 operator/(float a, const Vector4& b)
	{
		return Vector4{ a, a, a, a } / b;
	}

#pragma endregion

	// ------------------------
	// 関数
	// ------------------------

#pragma region Vec2_Function

	/// <summary>
	/// Vec3型で初期化するコンストラクタ
	/// </summary>
	constexpr Vector2::Vector2(const Vector3& v)
		:x(v.x), y(v.y)
	{

	}

	/// <summary>
	/// Vec4型で初期化するコンストラクタ
	/// </summary>
	constexpr Vector2::Vector2(const Vector4& v)
		:x(v.x), y(v.y)
	{

	}

#pragma endregion

#pragma region Vec3_Function

	/// <summary>
	/// Vec4をVec3に変換するコンストラクタ
	/// (暗黙的な変換を禁止)
	/// </summary>
	constexpr Vector3::Vector3(const Vector4& v)
		:x(v.x), y(v.y), z(v.z)
	{

	}

	/// <summary>
	/// ベクトルの内積を求める
	/// </summary>
	/// <param name="[in] a"> ベクトルA </param>
	/// <param name="[in] b"> ベクトルB </param>
	/// <returns> ベクトルの内積 </returns>
	inline float Vector3::Dot(const Vector3& a, const Vector3& b)
	{
		return a.x * b.x + a.y * b.y + a.z * b.z;
	}

	/// <summary>
	/// ベクトルの大きさ(2乗値)を求める
	/// </summary>
	/// <returns> ベクトルの大きさ(2乗値) </returns>
	inline float Vector3::SprMagnitude() const
	{
		Vector3 v = { x, y, z };
		return Dot(v, v);
	}

	/// <summary>
	/// ベクトルの大きさを求める
	/// </summary>
	/// <returns> ベクトルの大きさ </returns>
	inline float Vector3::Length() const
	{
		return sqrt(SprMagnitude());
	}

	/// <summary>
	/// ベクトルを正規化する
	/// </summary>
	/// <returns> 単位ベクトル </returns>
	inline Vector3 Vector3::Normalize() const
	{
		Vector3 v = { x, y, z };
		return v / Length();
	}

#pragma endregion

} // namespace PokarinEngine

#endif // !VECTOR_H_INCLUDED