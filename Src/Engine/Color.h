/**
* @file Color.h
*/
#ifndef COLOR_H_INCLUDED
#define COLOR_H_INCLUDED

#include "Math/Vector.h"

namespace PokarinEngine
{
	/// <summary>
	/// 色
	/// </summary>
	struct Color
	{
		// 赤・青・緑・透明度
		// 「0.0 ～ 1.0」で指定
		float r, g, b, a;

		// デフォルトコンストラクタ
		Color() = default;

		/// <summary>
		/// float型で初期化するコンストラクタ
		/// </summary>
		explicit constexpr Color(float n) : r(n), g(n), b(n), a(n) {}

		/// <summary>
		/// float型で初期化するコンストラクタ
		/// </summary>
		constexpr Color(float _r, float _g, float _b, float _a)
			: r(_r), g(_g), b(_b), a(_a) {}

		/// <summary>
		/// Vec4型で初期化するコンストラクタ
		/// </summary>
		constexpr Color(const Vector4& v)
			: r(v.x), g(v.y), b(v.z), a(v.w) {}

		// ------------------
		// 添字演算子
		// ------------------

		float& operator[](size_t i) { return *(&r + i); }
		float operator[](size_t i)const { return *(&r + i); }

		// ------------------------
		// 変換
		// ------------------------

		operator ImVec4() { return ImVec4(r, g, b, a); }

		operator const ImVec4() const { return ImVec4(r, g, b, a); }
	};

	namespace BasicColor
	{
		// 灰色
		inline constexpr Color gray = { 0.2f, 0.2f, 0.2f, 1.0f };

		// 黒色
		inline constexpr Color black = { 0, 0, 0, 1.0f };
	}

#pragma region Operator

	// --------------
	// 乗算
	// --------------

	// --------- Color同士の乗算 -----------

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

	// ------------------ Colorとfloatの乗算 -------------------

	inline Color operator*=(Color& a, float b)
	{
		a.r *= b;
		a.g *= b;
		a.b *= b;
		a.a *= b;

		return a;
	}

	inline Color operator*(const Color& a, float b)
	{
		Color n = a;
		return n *= b;
	}

#pragma endregion

} // namespace PokarinEngine

#endif // !COLOR_H_INCLUDED
