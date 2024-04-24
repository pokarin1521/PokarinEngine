/**
* @file Vector.h
*/
#ifndef VECTOR_H_INCLUDED
#define VECTOR_H_INCLUDED

#include "ImGui/imgui.h"

#include <cmath>

namespace PokarinEngine
{
	/// <summary>
	/// Vector型の要素数
	/// </summary>
	namespace Vector_Size
	{
		constexpr int vector2 = 2;
		constexpr int vector3 = 3;
		constexpr int vector4 = 4;
	}

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
		// -----------------------
		// 変数
		// -----------------------

		float x, y;

		// -----------------------------
		// コンストラクタ
		// -----------------------------

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
	struct Vector3
	{
		// ----------------------------
		// 変数
		// ----------------------------

		float x, y, z;

		// 要素数
		static const size_t size;

		// -----------------------------------
		// コンストラクタ
		// -----------------------------------

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

		// ---------------------------------
		// メンバ関数
		// ---------------------------------

		/// <summary>
		/// ベクトルの内積を求める
		/// </summary>
		/// <param name="v"> ベクトル </param>
		/// <returns> ベクトルの内積 </returns>
		inline float Dot(const Vector3& v) const;

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

		// ------------------------
		// 添字演算子
		// ------------------------

		float& operator[](size_t i) { return *(&x + i); }
		float operator[](size_t i)const { return *(&x + i); }
	};

	/// <summary>
	/// 4次元座標
	/// </summary>
	struct Vector4
	{
		// -------------------------
		// 変数
		// -------------------------

		float x, y, z, w;

		// ----------------------------------
		// コンストラクタ
		// ----------------------------------

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

		// ------------------
		// 添字演算子
		// ------------------

		float& operator[](size_t i) { return *(&x + i); }
		float operator[](size_t i)const { return *(&x + i); }
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
	/// <param name="v"> ベクトル </param>
	/// <returns> ベクトルの内積 </returns>
	inline float Vector3::Dot(const Vector3& v) const
	{
		return v.x * x + v.y * y + v.z * z;
	}

	/// <summary>
	/// ベクトルの大きさ(2乗値)を求める
	/// </summary>
	/// <returns> ベクトルの大きさ(2乗値) </returns>
	inline float Vector3::SprMagnitude() const
	{
		Vector3 v = { x, y, z };
		return Dot(v);
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

#pragma region Vec4_Function

	/// <summary>
	/// 座標変換ベクトルの配列を計算する
	/// </summary>
	/// <param name="[out] result"> 計算結果(要素数4の配列) </param>
	/// <param name="[in] scale"> 拡大率 </param>
	/// <param name="[in] rotation"> 回転角度 </param>
	/// <param name="[in] position"> 平行移動量 </param>
	inline void GetTransformVectors(Vector4* result,
		const Vector3& scale, const Vector3& rotation, const Vector3& position)
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
	inline void GetRotationVectors(Vector3* result, const Vector3& rotation)
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
		Vector4* result, const Vector4* m, const Vector4* n)
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
		Vector4 mn[4] = { Vector4(0) };

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
		Vector3* result, const Vector3* m, const Vector3* n)
	{
		// 元データ破壊を回避するため
		// 一度この変数に計算結果を代入する
		Vector3 mn[3] = { Vector3(0) };

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

#pragma endregion

} // namespace PokarinEngine

#endif // !VECTOR_H_INCLUDED