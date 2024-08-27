/**
* @file Color.h
*/
#ifndef POKARINENGINE_COLOR_H_INCLUDED
#define POKARINENGINE_COLOR_H_INCLUDED

#include "ImGui/imgui.h"

#include "Math/Vector.h"

namespace PokarinEngine
{
	/// <summary>
	/// 色
	/// </summary>
	struct Color
	{
	public: // -------------------- コンストラクタ --------------------

		// デフォルトコンストラクタ
		Color() = default;

		/// <summary>
		/// float型で初期化するコンストラクタ
		/// </summary>
		explicit constexpr Color(float n) : r(n), g(n), b(n), a(n) {}

		/// <summary>
		/// Vector4型で初期化するコンストラクタ
		/// </summary>
		constexpr Color(const Vector4& v)
			: r(v.x), g(v.y), b(v.z), a(v.w) {}

		/// <summary>
		/// float型で初期化するコンストラクタ
		/// </summary>
		constexpr Color(float _r, float _g, float _b, float _a)
			: r(_r), g(_g), b(_b), a(_a) {}

	public: // ------------------------- 変換 -------------------------

		// ImVec4型に変換
		operator ImVec4() const { return ImVec4(r, g, b, a); }

		// Vector4型に変換
		operator Vector4() const { return Vector4(r, g, b, a); }

		// Vector3型に変換
		operator Vector3() const { return Vector3(r, g, b); }

	public: // ---------------------- 添字演算子 ----------------------

		float& operator[](size_t i) { return *(&r + i); }
		float operator[](size_t i)const { return *(&r + i); }

	public: // ------------------------- 変数 -------------------------

		// 赤・青・緑・透明度
		// 「0.0 ～ 1.0」で指定
		float r = 0, g = 0, b = 0, a = 0;
		 
	public: // ------------------------ 基本色 ------------------------

		// 白色
		static const Color white;

		// 黒色
		static const Color black;

		// 灰色
		static const Color gray;

		// 緑色
		static const Color green;
	};

	// ---------------------------------
	// 基本色の定義
	// ---------------------------------

	// 白色
	inline const Color Color::white = { 1.0f, 1.0f, 1.0f, 1.0f };

	// 黒色
	inline const Color Color::black = { 0, 0, 0, 1.0f };

	// 灰色
	inline const Color Color::gray = { 0.2f, 0.2f, 0.2f, 1.0f };	
	
	// 緑色
	inline const Color Color::green = { 0.0f, 1.0f, 0.0f, 1.0f };

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

#endif // !POKARINENGINE_COLOR_H_INCLUDED
