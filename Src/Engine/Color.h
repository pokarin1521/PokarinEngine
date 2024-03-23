/**
* @file Color.h
*/
#ifndef COLOR_H_INCLUDED
#define COLOR_H_INCLUDED

#include "VecMath.h"

namespace PokarinEngine
{
	/// <summary>
	/// �F
	/// </summary>
	struct Color
	{
		// �ԁE�E�΁E�����x
		// �u0.0 �` 1.0�v�Ŏw��
		float r, g, b, a;

		// �f�t�H���g�R���X�g���N�^
		Color() = default;

		/// <summary>
		/// float�^�ŏ���������R���X�g���N�^
		/// </summary>
		explicit constexpr Color(float n) : r(n), g(n), b(n), a(n) {}

		/// <summary>
		/// float�^�ŏ���������R���X�g���N�^
		/// </summary>
		constexpr Color(float _r, float _g, float _b, float _a)
			: r(_r), g(_g), b(_b), a(_a) {}

		/// <summary>
		/// Vec4�^�ŏ���������R���X�g���N�^
		/// </summary>
		constexpr Color(const Vec4& v)
			: r(v.x), g(v.y), b(v.z), a(v.w) {}

		// ------------------
		// �Y�����Z�q
		// ------------------

		float& operator[](size_t i) { return *(&r + i); }
		float operator[](size_t i)const { return *(&r + i); }
	};

#pragma region Operator

	// --------------
	// ��Z
	// --------------

	// --------- Vec4���m�̏�Z -----------

	inline Color& operator*=(Color& a, const Color& b)
	{
		a.r *= b.r;
		a.g *= b.g;
		a.b *= b.b;
		a.a *= b.a;

		return a;
	}

	inline Color operator*(const Color& a, const Color& b)
	{
		Color n = a;
		return n *= b;
	}

#pragma endregion

} // namespace PokarinEngine

#endif // !COLOR_H_INCLUDED