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
		// Vec2�̗v�f��
		constexpr int count = 2;
	}

	namespace Info_Vec3
	{
		// Vec3�̗v�f��
		constexpr int count = 3;
	}

	namespace Info_Vec4
	{
		// Vec4�̗v�f��
		constexpr int count = 4;
	}

	// -----------------
	// �O���錾
	// -----------------

	// ------- �x�N�g���^ -------

	struct Vec2;
	struct Vec3;
	struct Vec4;

	// --------- �s��^ ---------

	struct Mat3;
	struct Mat4;

	// �~����
	constexpr float pi = 3.1415926535f;

	// -----------------
	// �\���̒�`
	// -----------------

#pragma region Vector

/// <summary>
/// 2�������W
/// </summary>
	struct Vec2
	{
		float x, y;

		// �f�t�H���g�R���X�g���N�^
		Vec2() = default;

		/// <summary>
		/// Vec3�^�ŏ���������R���X�g���N�^
		/// </summary>
		explicit constexpr Vec2(const Vec3& v);

		/// <summary>
		/// Vec4�^�ŏ���������R���X�g���N�^
		/// </summary>
		explicit constexpr Vec2(const Vec4& v);

		/// <summary>
		/// float�^�ŏ���������R���X�g���N�^
		/// </summary>
		explicit constexpr Vec2(float n) : x(n), y(n) {}

		/// <summary>
		/// float�^�ŏ���������R���X�g���N�^
		/// </summary>
		constexpr Vec2(float _x, float _y) : x(_x), y(_y) {}

		// ------------------------
		// �Y�����Z�q
		// ------------------------

		float& operator[](size_t i) { return *(&x + i); }
		float operator[](size_t i)const { return *(&x + i); }

		// ------------------------
		// �ϊ�
		// ------------------------

		operator ImVec2() { return ImVec2(x, y); }
	};

	/// <summary>
	/// 3�������W
	/// </summary>
	struct Vec3
	{
		float x, y, z;

		// �f�t�H���g�R���X�g���N�^
		Vec3() = default;

		/// <summary>
		/// Vec4�ŏ���������R���X�g���N�^
		/// (�ÖٓI�ȕϊ����֎~)
		/// </summary>
		explicit constexpr Vec3(const Vec4& v);

		/// <summary>
		/// float�^�ŏ���������R���X�g���N�^
		/// (�ÖٓI�ȕϊ����֎~)
		/// </summary>
		explicit constexpr Vec3(float n) : x(n), y(n), z(n) {}

		/// <summary>
		/// float�^�ŏ���������R���X�g���N�^
		/// </summary>
		constexpr Vec3(float _x, float _y, float _z) : x(_x), y(_y), z(_z) {}

		/// <summary>
		/// Vec2�^��float�^�ŏ���������R���X�g���N�^
		/// </summary>
		constexpr Vec3(const Vec2& v, float _z) : x(v.x), y(v.y), z(_z) {}

		// ------------------------
		// �Y�����Z�q
		// ------------------------

		float& operator[](size_t i) { return *(&x + i); }
		float operator[](size_t i)const { return *(&x + i); }
	};

	/// <summary>
	/// 4�������W
	/// </summary>
	struct Vec4
	{
		float x, y, z, w;

		// �f�t�H���g�R���X�g���N�^
		Vec4() = default;

		/// <summary>
		/// float�^�ŏ���������R���X�g���N�^
		/// </summary>
		explicit constexpr Vec4(float n) : x(n), y(n), z(n), w(n) {}

		/// <summary>
		/// float�^�ŏ���������R���X�g���N�^
		/// </summary>
		constexpr Vec4(float _x, float _y, float _z, float _w)
			: x(_x), y(_y), z(_z), w(_w) {}

		/// <summary>
		/// Vec2�^��float�^�ŏ���������R���X�g���N�^
		/// </summary>
		constexpr Vec4(const Vec2& v, float _z, float _w)
			: x(v.x), y(v.y), z(_z), w(_w) {}

		/// <summary>
		/// Vec3�^��float�^�ŏ���������R���X�g���N�^
		/// </summary>
		constexpr Vec4(const Vec3& v, float _w)
			: x(v.x), y(v.y), z(v.z), w(_w) {}

		// ------------------
		// �Y�����Z�q
		// ------------------

		float& operator[](size_t i) { return *(&x + i); }
		float operator[](size_t i)const { return *(&x + i); }
	};

#pragma endregion

#pragma region Matrix

	/// <summary>
	/// 3x3�s��
	/// </summary>
	struct Mat3
	{
		Vec3 data[3];

		// ---------------------
		// �R���X�g���N�^
		// ---------------------

		// �f�t�H���g�R���X�g���N�^
		Mat3() = default;

		/// <summary>
		/// Vec3�^�ŏ���������R���X�g���N�^
		/// </summary>
		Mat3(const Vec3& v0, const Vec3& v1, const Vec3& v2)
		{
			data[0] = v0;
			data[1] = v1;
			data[2] = v2;
		}

		/// <summary>
		/// float�^�ŏ���������R���X�g���N�^
		/// </summary>
		explicit Mat3(float f)
		{
			data[0] = { f, 0, 0 };
			data[1] = { 0, f, 0 };
			data[2] = { 0, 0, f };
		}

		/// <summary>
		/// Mat4�^�ŏ���������R���X�g���N�^
		/// </summary>
		explicit Mat3(const struct Mat4& m);

		// ---------------------
		// �Y�����Z�q
		// ---------------------

		Vec3& operator[](size_t i) { return data[i]; }
		const Vec3& operator[](size_t i) const { return data[i]; }
	};

	/// <summary>
	/// 4x4�s��
	/// </summary>
	struct Mat4
	{
		Vec4 data[4];

		// --------------------
		// �R���X�g���N�^
		// --------------------

		// �f�t�H���g�R���X�g���N�^
		Mat4() = default;

		/// <summary>
		/// Vec4�^�ŏ���������R���X�g���N�^
		/// </summary>
		Mat4(const Vec4& v0, const Vec4& v1, const Vec4& v2, const Vec4& v3)
		{
			data[0] = v0;
			data[1] = v1;
			data[2] = v2;
			data[3] = v3;
		}

		/// <summary>
		/// float�^�ŏ���������R���X�g���N�^
		/// </summary>
		explicit Mat4(float f)
		{
			data[0] = { f, 0, 0, 0 };
			data[1] = { 0, f, 0, 0 };
			data[2] = { 0, 0, f ,0 };
			data[3] = { 0, 0, 0, f };
		}

		/// <summary>
		/// Mat3�^�ŏ���������R���X�g���N�^
		/// </summary>
		explicit Mat4(const struct Mat3& m)
		{
			data[0] = { m.data[0], 0 };
			data[1] = { m.data[1], 0 };
			data[2] = { m.data[2], 0 };
			data[3] = { 0, 0, 0, 1 };
		}

		// ------------------
		// �Y�����Z�q
		// ------------------

		Vec4& operator[](size_t i) { return data[i]; }
		const Vec4& operator[](size_t i) const { return data[i]; }
	};

#pragma endregion

	// -----------------
	// �I�y���[�^�[
	// -----------------

#pragma region Vec2_Operator

	// -------
	// �P��
	// -------
	
	// --------- Vec2�� -���Z�q -----------

	inline Vec2 operator-(const Vec2& a)
	{
		return { -a.x, -a.y };
	}

	// --------------
	// ���Z
	// --------------

	// --------- Vec2���m�̉��Z ----------

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

	// --------- Vec2��float�̉��Z -----------

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
	// ���Z
	// --------------

	// ---------- Vec2���m�̌��Z -----------

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

	// ---------- Vec2��float�̌��Z -----------

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
	// ��Z
	// --------------

	// --------- Vec2���m�̏�Z -----------

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

	// -------- Vec2��float�̏�Z ----------

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
	// ���Z
	// ---------------

	// ---------- Vec2���m�̏��Z -----------

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

	// ---------- Vec2��float�̌��Z -----------

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
	// �P��
	// --------------------

	// --------- Vec3�� -���Z�q -----------

	inline Vec3 operator-(const Vec3& a)
	{
		return { -a.x, -a.y, -a.z };
	}

	// --------------
	// ���Z
	// --------------

	// --------- Vec3���m�̉��Z ----------

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

	// --------- Vec3��float�̉��Z -----------

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
	// ���Z
	// --------------

	// ---------- Vec3���m�̌��Z -----------

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

	// ---------- Vec3��float�̌��Z -----------

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
	// ��Z
	// --------------

	// --------- Vec3���m�̏�Z -----------

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

	// -------- Vec3��float�̏�Z ----------

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
	// ���Z
	// ---------------

	// ---------- Vec3���m�̏��Z -----------

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

	// ---------- Vec3��float�̏��Z -----------

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
	// �P��
	// -----------

	// --------- Vec4�� -���Z�q ----------

	inline Vec4 operator-(const Vec4& a)
	{
		return { -a.x, -a.y, -a.z, -a.w };
	}

	// ------------
	// ���Z
	// ------------

	// --------- Vec4���m�̉��Z ------------

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

	// --------- Vec4��float�̉��Z -----------

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
	// ���Z
	// --------------

	// ---------- Vec4���m�̌��Z -----------

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

	// ---------- Vec4��float�̌��Z -----------

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
	// ��Z
	// --------------

	// --------- Vec4���m�̏�Z -----------

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

	// -------- Vec4��float�̏�Z ----------

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
	// ���Z
	// ---------------

	// ---------- Vec4���m�̏��Z -----------

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

	// ---------- Vec4��float�̏��Z -----------

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
	// ��Z
	// -----------------

	// --------- Mat3��Vec3�̏�Z ---------

	inline Vec3 operator*(const Mat3& m, const Vec3& v)
	{
		return m.data[0] * v.x + m.data[1] * v.y + m.data[2] * v.z;
	}

	// ---------- Mat3���m�̏�Z ----------

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
	// ��Z
	// ----------------

	// --------- Mat4��Vec4�̏�Z ----------

	inline Vec4 operator*(const Mat4& m, const Vec4& v)
	{
		return m.data[0] * v.x + m.data[1] * v.y + m.data[2] * v.z + m.data[3] * v.w;
	}

	// ---------- Mat4���m�̏�Z -----------

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
	// �֐�
	// ------------

#pragma region Radians_Degrees

/// <summary>
/// �x���@���ʓx�@�ɕϊ�����
/// </summary>
/// <param name="deg"> �p�x(�x���@) </param>
/// <returns> �p�x(�ʓx�@) </returns>
	constexpr float Radians(float deg)
	{
		return deg * pi / 180.0f;
	}

	/// <summary>
	/// �x���@���ʓx�@�ɕϊ�����
	/// </summary>
	/// <param name="deg"> �p�x(�x���@) </param>
	/// <returns> �p�x(�ʓx�@) </returns>
	constexpr Vec3 Radians(Vec3 deg)
	{
		return Vec3(Radians(deg.x), Radians(deg.y), Radians(deg.z));
	}

	/// <summary>
	/// �ʓx�@��x���@�ɕϊ�����
	/// </summary>
	/// <param name="rad"> �p�x(�ʓx�@) </param>
	/// <returns> �p�x(�x���@) </returns>
	constexpr float Degrees(float rad)
	{
		return rad * 180.0f / pi;
	}

	/// <summary>
	/// �ʓx�@��x���@�ɕϊ�����
	/// </summary>
	/// <param name="rad"> �p�x(�ʓx�@) </param>
	/// <returns> �p�x(�x���@) </returns>
	constexpr Vec3 Degrees(Vec3 rad)
	{
		return Vec3(Degrees(rad.x), Degrees(rad.y), Degrees(rad.z));
	}

#pragma endregion

#pragma region Vec2_Function

	/// <summary>
	/// Vec3�^�ŏ���������R���X�g���N�^
	/// </summary>
	constexpr Vec2::Vec2(const Vec3& v)
		:x(v.x), y(v.y)
	{

	}

	/// <summary>
	/// Vec4�^�ŏ���������R���X�g���N�^
	/// </summary>
	constexpr Vec2::Vec2(const Vec4& v)
		:x(v.x), y(v.y)
	{

	}

#pragma endregion

#pragma region Vec3_Function

	/// <summary>
	/// Vec4��Vec3�ɕϊ�����R���X�g���N�^
	/// (�ÖٓI�ȕϊ����֎~)
	/// </summary>
	constexpr Vec3::Vec3(const Vec4& v)
		:x(v.x), y(v.y), z(v.z)
	{

	}

	namespace Vec3_Function
	{
		/// <summary>
		/// Vec3�̓���
		/// </summary>
		/// <param name="a"> �x�N�g��A </param>
		/// <param name="b"> �x�N�g��B </param>
		/// <returns> ���ς̌��� </returns>
		inline float Dot(const Vec3& a, const Vec3& b)
		{
			return a.x * b.x + a.y * b.y + a.z * b.z;
		}

		/// <summary>
		/// Vec3�̒���
		/// </summary>
		/// <param name="a"> �x�N�g�� </param>
		/// <returns> �x�N�g���̒��� </returns>
		inline float Length(const Vec3& a)
		{
			return sqrt(Dot(a, a));
		}

		/// <summary>
		/// Vec3�̐��K��
		/// </summary>
		/// <param name="a"> �x�N�g�� </param>
		/// <returns> �P�ʃx�N�g�� </returns>
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
		/// ���W�ϊ��x�N�g���̔z����v�Z����
		/// </summary>
		/// <param name="[out] result"> �v�Z����(�v�f��4�̔z��) </param>
		/// <param name="[in] scale"> �g�嗦 </param>
		/// <param name="[in] rotation"> ��]�p�x </param>
		/// <param name="[in] position"> ���s�ړ��� </param>
		inline void GetTransformVectors(Vec4* result,
			const Vec3& scale, const Vec3& rotation, const Vec3& position)
		{
			/* �g�嗦�AZ��X��Y���̉�]�A���s�ړ��𔽉f���邽�߂̌v�Z���܂Ƃ߂�

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

			����1�s�ɂ܂Ƃ߂������Ȃ茩�Â炢
			�uv�v�Ɋ|���鐬�����x�N�g���um0, m1, m2�v�Ƃ��Ē�`���A
			���ςŌv�Z���ł���悤�ɒ��_���W��w�v�f��ǉ�����
			(v.w = 1�Ƃ���)

			x''' = v.x * m0.x + v.y * m0.y + v.z * m0.z + v.w * m0.w
			y''' = v.x * m1.x + v.y * m1.y + v.z * m1.z + v.w * m1.w
			z''' = v.x * m2.x + v.y * m2.y + v.z * m2.z + v.w * m2.w
			w''' = v.x * m3.x + v.y * m3.y + v.z * m3.z + v.w * m3.w */

			// -------------------------------------------------
			// m0, m1, m2, m3�����߁Aresult�Ɋi�[����
			// -------------------------------------------------

			// X����]��Sin�ECos
			const float sinX = sin(rotation.x), cosX = cos(rotation.x);

			// Y����]��Sin�ECos
			const float sinY = sin(rotation.y), cosY = cos(rotation.y);

			// Z����]��Sin�ECos
			const float sinZ = sin(rotation.z), cosZ = cos(rotation.z);

			// m0���v�Z
			result[0] = { scale.x * sinX * sinY * cosY * sinZ * cosZ,
						  scale.y * sinX * sinY * cosY * -sinZ * cosZ,
						  scale.z * cosX * sinY,
						  position.x };

			// m1���v�Z
			result[1] = { scale.x * cosX * sinZ,
						  scale.y * cosX * cosZ,
						  scale.z * -sinX,
						  position.y };

			// m2���v�Z
			result[2] = { scale.x * sinX * -sinY * cosY * sinZ * cosZ,
						  scale.y * sinX * -sinY * cosY * -sinZ * cosZ,
						  scale.z * cosX * cosY,
						  position.z };

			// m3���v�Z
			// w = 1�ɂȂ�悤�ɂ���
			result[3] = { 0, 0, 0, 1 };
		}

		/// <summary>
		/// �@���ϊ��x�N�g���̔z����v�Z����
		/// </summary>
		/// <param name="[out] result"> �v�Z����(�v�f��3�̔z��) </param>
		/// <param name="[in] rotation"> ��]�p�x </param>
		inline void GetRotationVectors(Vec3* result, const Vec3& rotation)
		{
			/* GetTransformVectors�́uscale = 1, position = 0�v�o�[�W����
			��]�����Ȃ̂�w�v�f�͕s�v */

			// X����]��Sin�ECos
			const float sinX = sin(rotation.x), cosX = cos(rotation.x);

			// Y����]��Sin�ECos
			const float sinY = sin(rotation.y), cosY = cos(rotation.y);

			// Z����]��Sin�ECos
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
		/// ���W�ϊ��x�N�g���̔z����|�����킹��
		/// </summary>
		/// <param name="[out] result"> �v�Z����(�v�f��4�̔z��) </param>
		/// <param name="[in] m"> ���W�ϊ��x�N�g���̔z��1 </param>
		/// <param name="[in] n"> ���W�ϊ��x�N�g���̔z��2 </param>
		inline void MultiplyTransformVectors(
			Vec4* result, const Vec4* m, const Vec4* n)
		{
			/* �e�̍��W�ϊ���ǉ�����
			�v�Z���̂ɐe�q�ɂ��Ⴂ�͂Ȃ��̂ŁA
			�um0 �` m3�v�Ɠ��l�̃x�N�g���un0 �` n3�v���`�ł���
			(x''', y''', z''', w'''�Ɠ������ɂȂ�)

			x2 = x''' * n0.x + y''' * n0.y + z''' * n0.z + w''' * n0.w
			y2 = x''' * n1.x + y''' * n1.y + z''' * n1.z + w''' * n1.w
			z2 = x''' * n2.x + y''' * n2.y + z''' * n2.z + w''' * n2.w
			w2 = x''' * n3.x + y''' * n3.y + z''' * n3.z + w''' * n3.w

			x'''��W�J�����
			x2�́um0�`m3��n0���|�������̂𒸓_���W�Ɋ|���Ă���v�Ƃ������Ƃ�������
			���ꂼ��W�J����Ɠ����悤�ɂȂ��Ă���
			�um0�`m3��n0���|�������́v���x�N�g��mn�Ƃ��Ē�`����

			x2' = v.x * mn0.x + v.y * mn0.y + v.z * mn0.z + v.w * mn0.w
			y2' = v.x * mn1.x + v.y * mn1.y + v.z * mn1.z + v.w * mn1.w
			z2' = v.x * mn2.x + v.y * mn2.y + v.z * mn2.z + v.w * mn2.w
			w2' = v.x * mn3.x + v.y * mn3.y + v.z * mn3.z + v.w * mn3.w

			CPU����mn���`�����
			�ȒP�Ȍv�Z�Őe�q�K�w���l���������W�ϊ����ł���

			����ȏ�̐e��ݒ肷��ꍇ���A
			m��n�Ɠ����悤�Ƀx�N�g�����`��
			�O�̌v�Z���ʂɊ|���邱�Ƃō��W�ϊ��ɕK�v�ȃx�N�g�������߂��� */

			// ---------------------------------------
			// ���W�ϊ��ɕK�v�ȃx�N�g��mn�����߂�
			// ---------------------------------------

			// �v�Z���ʂ���x���̕ϐ��ɓ����
			// result��m,n�ɂ͓����A�h���X���w��o���邽�߁A
			// ���ڑ������ƌ��̃f�[�^���󂵂Ă��܂�
			Vec4 mn[4] = { Vec4(0) };

			// �x�N�g��mn�����߂�
			for (int i = 0; i < 4; ++i)
			{
				mn[i] = m[0] * n[i].x + m[1] * n[i].y + m[2] * n[i].z + m[3] * n[i].w;
			}

			// �v�Z���ʂ���
			for (int i = 0; i < 4; ++i)
			{
				result[i] = mn[i];
			}
		}

		/// <summary>
		/// �@���ϊ��x�N�g���̔z����|�����킹��
		/// </summary>
		/// <param name="result"> �v�Z����(�v�f��3�̔z��) </param>
		/// <param name="m"> �@���ϊ��x�N�g���̔z��1 </param>
		/// <param name="n"> �@���ϊ��x�N�g���̔z��2 </param>
		inline void MultiplyRotationVectors(
			Vec3* result, const Vec3* m, const Vec3* n)
		{
			// ���f�[�^�j���������邽��
			// ��x���̕ϐ��Ɍv�Z���ʂ�������
			Vec3 mn[3] = { Vec3(0) };

			// �x�N�g��mn�����߂�
			for (int i = 0; i < 3; ++i)
			{
				mn[i] = m[0] * n[i].x + m[1] * n[i].y + m[2] * m[i].z;
			}

			// �v�Z���ʂ���
			for (int i = 0; i < 3; ++i)
			{
				result[i] = mn[i];
			}
		}
	}

#pragma endregion

#pragma region Mat3_Function

	/// <summary>
	/// Mat4�^�ŏ���������R���X�g���N
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
		/// ��]�p�x�����]�s����쐬
		/// </summary>
		/// <param name="rotation"> ��]�p�x </param>
		/// <returns> �쐬������]�s�� </returns>
		inline Mat3 GetRotationMatrix(const Vec3& rotation)
		{
			/* �s��́A�x�N�g���z��Ɣ�ׂ�
			�u�c�Ɖ������]���Ă���v�̂�

			{  cosY, sinX * sinY, cosX * sinY }
			{     0,        cosX,       -sinX }
			{ -sinY, sinX * cosY, cosX * cosY }

							��

			{        cosY,     0,       -sinY }
			{ sinX * sinY,  cosX, sinX * cosY }
			{ cosX * sinY, -sinX, cosX * cosY }

			�ɂȂ�

			XY����]�̍s���
		�@�@�uX����]�̍s��v�ɁuY����]�̍s��v���|�����킹������
			�Ȃ̂ŁAZ����]��ǉ�����ɂ́uXY����]�̍s��v�ɁuZ����]�̍s��v���|�����킹��΂��� */

			// X����]��Sin�ECos
			const float sinX = sin(rotation.x), cosX = cos(rotation.x);

			// Y����]��Sin�ECos
			const float sinY = sin(rotation.y), cosY = cos(rotation.y);

			// Z����]��Sin�ECos
			const float sinZ = sin(rotation.z), cosZ = cos(rotation.z);

			// XY����]�̍s��
			Mat3 m = { {        cosY,     0,        -sinY },
					   { sinX * sinY,  cosX,  sinX * cosY },
					   { cosX * sinY, -sinX,  cosX * cosY } };

			// Z����]�̍s��
			const Mat3 n = { {  cosZ, sinZ, 0 },
							 { -sinZ, cosZ, 0 },
							 {     0,    0, 1 } };

			// XY����]��Z����]���|�����킹��
			// XYZ����]�̍s��ɂ���
			return m * n;
		}
	}

#pragma endregion

#pragma region Mat4_Function

	namespace Mat4_Function
	{
		/// <summary>
		/// ���W�ϊ��s����쐬
		/// </summary>
		/// <param name="scale"> �g�嗦 </param>
		/// <param name="rotation"> ��]�p�x </param>
		/// <param name="position"> ���s�ړ��� </param>
		/// <returns> �쐬�������W�ϊ��s�� </returns>
		inline Mat4 GetTransformMatrix(
			const Vec3& scale, const Vec3& rotation, const Vec3& position)
		{
			// ��]�𔽉f
			const Mat3 m = Mat3_Function::GetRotationMatrix(rotation);

			// �g�嗦�ƕ��s�ړ��ʂ𔽉f
			return {
				Vec4{ scale.x * m.data[0], 0 },
				Vec4{ scale.y * m.data[1], 0 },
				Vec4{ scale.z * m.data[2], 0 },
				Vec4{ position, 1 } };
		}

		/// <summary>
		/// 4x4�s�񂩂�g�嗦�𒊏o����
		/// </summary>
		/// <param name="transform"> �g�嗦�𒊏o�������s�� </param>
		/// <returns> �g�嗦 </returns>
		inline Vec3 ExtractScale(const Mat4& transform)
		{
			// ����3x3�����̊e�s��3D�x�N�g���Ƃ��Ē��������߂邱�Ƃ�
			// �g�嗦�����߂�
			const float sx = Vec3_Function::Length(Vec3(transform[0]));
			const float sy = Vec3_Function::Length(Vec3(transform[1]));
			const float sz = Vec3_Function::Length(Vec3(transform[2]));

			return { sx, sy, sz };
		}
	}

#pragma endregion

} // namespace PokarinEngine

#endif // !VECMATH_H_INCLUDED