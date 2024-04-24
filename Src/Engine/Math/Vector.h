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
	/// Vector�^�̗v�f��
	/// </summary>
	namespace Vector_Size
	{
		constexpr int vector2 = 2;
		constexpr int vector3 = 3;
		constexpr int vector4 = 4;
	}

	// ----------------------------
	// �O���錾
	// ----------------------------

	struct Vector2;
	struct Vector3;
	struct Vector4;

	// -------------------------------
	// �\���̒�`
	// -------------------------------

	/// <summary>
	/// 2�������W
	/// </summary>
	struct Vector2
	{
		// -----------------------
		// �ϐ�
		// -----------------------

		float x, y;

		// -----------------------------
		// �R���X�g���N�^
		// -----------------------------

		// �f�t�H���g�R���X�g���N�^
		Vector2() = default;

		/// <summary>
		/// Vec3�^�ŏ���������R���X�g���N�^
		/// </summary>
		explicit constexpr Vector2(const Vector3& v);

		/// <summary>
		/// Vec4�^�ŏ���������R���X�g���N�^
		/// </summary>
		explicit constexpr Vector2(const Vector4& v);

		/// <summary>
		/// float�^�ŏ���������R���X�g���N�^
		/// </summary>
		explicit constexpr Vector2(float n) : x(n), y(n) {}

		/// <summary>
		/// float�^�ŏ���������R���X�g���N�^
		/// </summary>
		constexpr Vector2(float _x, float _y) : x(_x), y(_y) {}

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
	struct Vector3
	{
		// ----------------------------
		// �ϐ�
		// ----------------------------

		float x, y, z;

		// �v�f��
		static const size_t size;

		// -----------------------------------
		// �R���X�g���N�^
		// -----------------------------------

		// �f�t�H���g�R���X�g���N�^
		Vector3() = default;

		/// <summary>
		/// Vec4�ŏ���������R���X�g���N�^
		/// (�ÖٓI�ȕϊ����֎~)
		/// </summary>
		explicit constexpr Vector3(const Vector4& v);

		/// <summary>
		/// float�^�ŏ���������R���X�g���N�^
		/// (�ÖٓI�ȕϊ����֎~)
		/// </summary>
		explicit constexpr Vector3(float n) : x(n), y(n), z(n) {}

		/// <summary>
		/// float�^�ŏ���������R���X�g���N�^
		/// </summary>
		constexpr Vector3(float _x, float _y, float _z) : x(_x), y(_y), z(_z) {}

		/// <summary>
		/// Vec2�^��float�^�ŏ���������R���X�g���N�^
		/// </summary>
		constexpr Vector3(const Vector2& v, float _z) : x(v.x), y(v.y), z(_z) {}

		// ---------------------------------
		// �����o�֐�
		// ---------------------------------

		/// <summary>
		/// �x�N�g���̓��ς����߂�
		/// </summary>
		/// <param name="v"> �x�N�g�� </param>
		/// <returns> �x�N�g���̓��� </returns>
		inline float Dot(const Vector3& v) const;

		/// <summary>
		/// �x�N�g���̑傫��(2��l)�����߂�
		/// </summary>
		/// <returns> �x�N�g���̑傫��(2��l) </returns>
		inline float SprMagnitude() const;

		/// <summary>
		/// �x�N�g���̑傫�������߂�
		/// </summary>
		/// <returns> �x�N�g���̑傫�� </returns>
		inline float Length() const;

		/// <summary>
		/// �x�N�g���𐳋K������
		/// </summary>
		/// <returns> �P�ʃx�N�g�� </returns>
		inline Vector3 Normalize() const;

		// ------------------------
		// �Y�����Z�q
		// ------------------------

		float& operator[](size_t i) { return *(&x + i); }
		float operator[](size_t i)const { return *(&x + i); }
	};

	/// <summary>
	/// 4�������W
	/// </summary>
	struct Vector4
	{
		// -------------------------
		// �ϐ�
		// -------------------------

		float x, y, z, w;

		// ----------------------------------
		// �R���X�g���N�^
		// ----------------------------------

		// �f�t�H���g�R���X�g���N�^
		Vector4() = default;

		/// <summary>
		/// float�^�ŏ���������R���X�g���N�^
		/// </summary>
		explicit constexpr Vector4(float n) : x(n), y(n), z(n), w(n) {}

		/// <summary>
		/// float�^�ŏ���������R���X�g���N�^
		/// </summary>
		constexpr Vector4(float _x, float _y, float _z, float _w)
			: x(_x), y(_y), z(_z), w(_w) {}

		/// <summary>
		/// Vec2�^��float�^�ŏ���������R���X�g���N�^
		/// </summary>
		constexpr Vector4(const Vector2& v, float _z, float _w)
			: x(v.x), y(v.y), z(_z), w(_w) {}

		/// <summary>
		/// Vec3�^��float�^�ŏ���������R���X�g���N�^
		/// </summary>
		constexpr Vector4(const Vector3& v, float _w)
			: x(v.x), y(v.y), z(v.z), w(_w) {}

		// ------------------
		// �Y�����Z�q
		// ------------------

		float& operator[](size_t i) { return *(&x + i); }
		float operator[](size_t i)const { return *(&x + i); }
	};

	// -------------------------------
	// �I�y���[�^�[
	// -------------------------------

#pragma region Vec2_Operator

	// -------
	// �P��
	// -------

	// --------- Vec2�� -���Z�q -----------

	inline Vector2 operator-(const Vector2& a)
	{
		return { -a.x, -a.y };
	}

	// --------------
	// ���Z
	// --------------

	// --------- Vec2���m�̉��Z ----------

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

	// --------- Vec2��float�̉��Z -----------

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
	// ���Z
	// --------------

	// ---------- Vec2���m�̌��Z -----------

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

	// ---------- Vec2��float�̌��Z -----------

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
	// ��Z
	// --------------

	// --------- Vec2���m�̏�Z -----------

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

	// -------- Vec2��float�̏�Z ----------

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
	// ���Z
	// ---------------

	// ---------- Vec2���m�̏��Z -----------

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

	// ---------- Vec2��float�̌��Z -----------

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
	// �P��
	// --------------------

	// --------- Vec3�� -���Z�q -----------

	inline Vector3 operator-(const Vector3& a)
	{
		return { -a.x, -a.y, -a.z };
	}

	// --------------
	// ���Z
	// --------------

	// --------- Vec3���m�̉��Z ----------

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

	// --------- Vec3��float�̉��Z -----------

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
	// ���Z
	// --------------

	// ---------- Vec3���m�̌��Z -----------

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

	// ---------- Vec3��float�̌��Z -----------

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
	// ��Z
	// --------------

	// --------- Vec3���m�̏�Z -----------

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

	// -------- Vec3��float�̏�Z ----------

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
	// ���Z
	// ---------------

	// ---------- Vec3���m�̏��Z -----------

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

	// ---------- Vec3��float�̏��Z -----------

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
	// �P��
	// -----------

	// --------- Vec4�� -���Z�q ----------

	inline Vector4 operator-(const Vector4& a)
	{
		return { -a.x, -a.y, -a.z, -a.w };
	}

	// ------------
	// ���Z
	// ------------

	// --------- Vec4���m�̉��Z ------------

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

	// --------- Vec4��float�̉��Z -----------

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
	// ���Z
	// --------------

	// ---------- Vec4���m�̌��Z -----------

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

	// ---------- Vec4��float�̌��Z -----------

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
	// ��Z
	// --------------

	// --------- Vec4���m�̏�Z -----------

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

	// -------- Vec4��float�̏�Z ----------

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
	// ���Z
	// ---------------

	// ---------- Vec4���m�̏��Z -----------

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

	// ---------- Vec4��float�̏��Z -----------

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
	// �֐�
	// ------------------------

#pragma region Vec2_Function

	/// <summary>
	/// Vec3�^�ŏ���������R���X�g���N�^
	/// </summary>
	constexpr Vector2::Vector2(const Vector3& v)
		:x(v.x), y(v.y)
	{

	}

	/// <summary>
	/// Vec4�^�ŏ���������R���X�g���N�^
	/// </summary>
	constexpr Vector2::Vector2(const Vector4& v)
		:x(v.x), y(v.y)
	{

	}

#pragma endregion

#pragma region Vec3_Function

	/// <summary>
	/// Vec4��Vec3�ɕϊ�����R���X�g���N�^
	/// (�ÖٓI�ȕϊ����֎~)
	/// </summary>
	constexpr Vector3::Vector3(const Vector4& v)
		:x(v.x), y(v.y), z(v.z)
	{

	}

	/// <summary>
	/// �x�N�g���̓��ς����߂�
	/// </summary>
	/// <param name="v"> �x�N�g�� </param>
	/// <returns> �x�N�g���̓��� </returns>
	inline float Vector3::Dot(const Vector3& v) const
	{
		return v.x * x + v.y * y + v.z * z;
	}

	/// <summary>
	/// �x�N�g���̑傫��(2��l)�����߂�
	/// </summary>
	/// <returns> �x�N�g���̑傫��(2��l) </returns>
	inline float Vector3::SprMagnitude() const
	{
		Vector3 v = { x, y, z };
		return Dot(v);
	}

	/// <summary>
	/// �x�N�g���̑傫�������߂�
	/// </summary>
	/// <returns> �x�N�g���̑傫�� </returns>
	inline float Vector3::Length() const
	{
		return sqrt(SprMagnitude());
	}

	/// <summary>
	/// �x�N�g���𐳋K������
	/// </summary>
	/// <returns> �P�ʃx�N�g�� </returns>
	inline Vector3 Vector3::Normalize() const
	{
		Vector3 v = { x, y, z };
		return v / Length();
	}

#pragma endregion

#pragma region Vec4_Function

	/// <summary>
	/// ���W�ϊ��x�N�g���̔z����v�Z����
	/// </summary>
	/// <param name="[out] result"> �v�Z����(�v�f��4�̔z��) </param>
	/// <param name="[in] scale"> �g�嗦 </param>
	/// <param name="[in] rotation"> ��]�p�x </param>
	/// <param name="[in] position"> ���s�ړ��� </param>
	inline void GetTransformVectors(Vector4* result,
		const Vector3& scale, const Vector3& rotation, const Vector3& position)
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
	inline void GetRotationVectors(Vector3* result, const Vector3& rotation)
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
		Vector4* result, const Vector4* m, const Vector4* n)
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
		Vector4 mn[4] = { Vector4(0) };

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
		Vector3* result, const Vector3* m, const Vector3* n)
	{
		// ���f�[�^�j���������邽��
		// ��x���̕ϐ��Ɍv�Z���ʂ�������
		Vector3 mn[3] = { Vector3(0) };

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

#pragma endregion

} // namespace PokarinEngine

#endif // !VECTOR_H_INCLUDED