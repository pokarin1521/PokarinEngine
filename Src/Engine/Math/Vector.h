/**
* @file Vector.h
*/
#ifndef VECTOR_H_INCLUDED
#define VECTOR_H_INCLUDED

#include "Json/UsingNameJson.h"

#include <string>
#include <cmath>

namespace PokarinEngine
{
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
	public: // -------------------- �R���X�g���N�^ --------------------

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

	public: // ---------------------- �Y�����Z�q ----------------------

		float& operator[](size_t i) { return *(&x + i); }
		float operator[](size_t i)const { return *(&x + i); }

	public: // ------------------------- ��� -------------------------

		float x, y;

		// �v�f��
		static constexpr int size = 2;

		// �S�Ă̒l��0
		static const Vector2 zero;
	};

	/// <summary>
	/// 3�������W
	/// </summary>
	struct Vector3
	{
	public: // -------------------- �R���X�g���N�^ --------------------

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

	public: // ------------------------- �v�Z -------------------------

		/// <summary>
		/// �x�N�g���̓��ς����߂�
		/// </summary>
		/// <param name="[in] a"> �x�N�g��A </param>
		/// <param name="[in] b"> �x�N�g��B </param>
		/// <returns> �x�N�g���̓��� </returns>
		static float Dot(const Vector3& a, const Vector3& b);

		/// <summary>
		/// �x�N�g���̑傫��(2��l)�����߂�
		/// </summary>
		/// <returns> �x�N�g���̑傫��(2��l) </returns>
		inline float SqrMagnitude() const;

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

	public: // ----------------------- �G�f�B�^ -----------------------

		/// <summary>
		///	�G�f�B�^�ɏ����h���b�O����p�X���C�_�[�ŕ\������
		/// </summary>
		/// <param name="[in] title"> �\������^�C�g�� </param>
		/// <param name="[in] id_string"> ���ʔԍ��̕����� </param>
		/// <param name="[in] startX"> �\���̊J�n�ʒu(X���W) </param>
		/// <param name="[in] width"> �\������X���C�_�[�̕� </param>
		void RenderDrag(const std::string& title, const std::string& id_string,
			const float width, const float startX = 0);

	public: // ------------------------- Json -------------------------

		/// <summary>
		/// ����Json�^�Ɋi�[����
		/// </summary>
		/// <param name="[out] data"> �����i�[����Json�^ </param>
		void ToJson(Json& data) const;

		/// <summary>
		/// ����Json�^����擾����
		/// </summary>
		/// <param name="[in] data"> �����i�[���Ă���Json�^ </param>
		void FromJson(const Json& data);

	public: // ---------------------- �Y�����Z�q ----------------------

		float& operator[](size_t i) { return *(&x + i); }
		float operator[](size_t i)const { return *(&x + i); }

	public: // ------------------------- ��� -------------------------

		float x, y, z;

		// �v�f��
		static constexpr int size = 3;

		// �S�Ă̒l��0
		static const Vector3 zero;
	};

	/// <summary>
	/// 4�������W
	/// </summary>
	struct Vector4
	{
	public: // -------------------- �R���X�g���N�^ --------------------

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

	public: // ---------------------- �Y�����Z�q ----------------------

		float& operator[](size_t i) { return *(&x + i); }
		float operator[](size_t i)const { return *(&x + i); }

	public: // ------------------------- ��� -------------------------

		float x, y, z, w;

		// �v�f��
		static constexpr int size = 4;

		// �S�Ă̒l��0
		static const Vector4 zero;
	};

	// -------------------------------
	// static�ϐ���`
	// -------------------------------

	// �S�Ă̒l��0
	inline const Vector2 Vector2::zero = { 0,0 };

	// �S�Ă̒l��0
	inline const Vector3 Vector3::zero = { 0,0,0 };

	// �S�Ă̒l��0
	inline const Vector4 Vector4::zero = { 0,0,0,0 };

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
	/// <param name="[in] a"> �x�N�g��A </param>
	/// <param name="[in] b"> �x�N�g��B </param>
	/// <returns> �x�N�g���̓��� </returns>
	inline float Vector3::Dot(const Vector3& a, const Vector3& b)
	{
		return a.x * b.x + a.y * b.y + a.z * b.z;
	}

	/// <summary>
	/// �x�N�g���̑傫��(2��l)�����߂�
	/// </summary>
	/// <returns> �x�N�g���̑傫��(2��l) </returns>
	inline float Vector3::SqrMagnitude() const
	{
		Vector3 v = { x, y, z };
		return Dot(v, v);
	}

	/// <summary>
	/// �x�N�g���̑傫�������߂�
	/// </summary>
	/// <returns> �x�N�g���̑傫�� </returns>
	inline float Vector3::Length() const
	{
		return sqrt(SqrMagnitude());
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

} // namespace PokarinEngine

#endif // !VECTOR_H_INCLUDED