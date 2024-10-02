/**
* @file Vector.cpp
*/
#include "Vector.h"

#include "ImGui/imgui.h"

#include "Json/Json.h"

namespace PokarinEngine
{
#pragma region static_zero

	// 全ての値が0
	const Vector2 Vector2::zero = { 0,0 };

	// 全ての値が0
	const Vector3 Vector3::zero = { 0,0,0 };

	// 全ての値が0
	const Vector4 Vector4::zero = { 0,0,0,0 };

#pragma endregion

#pragma region static_one

	// 全ての値が1
	const Vector2 Vector2::one = { 1,1 };

	// 全ての値が1
	const Vector3 Vector3::one = { 1,1,1 };

	// 全ての値が1
	const Vector4 Vector4::one = { 1,1,1,1 };

#pragma endregion

#pragma region Vector2

	/// <summary>
	/// ImVec2型で初期化するコンストラクタ
	/// </summary>
	/// <param name="v"></param>
	Vector2::Vector2(const ImVec2& v)
		:x(v.x), y(v.y)
	{

	}

	/// <summary>
	/// ImVec2型に変換
	/// </summary>
	Vector2::operator ImVec2() const
	{
		return ImVec2(x, y);
	}

	/// <summary>
	/// Vector2型からJson型への変換
	/// </summary>
	void to_json(Json& json, const Vector2& v)
	{
		json["x"] = v.x;
		json["y"] = v.y;
	}

	/// <summary>
	/// Json型からVector2型への変換
	/// </summary>
	void from_json(const Json& json, Vector2& v)
	{
		json["x"].get_to(v.x);
		json["y"].get_to(v.y);
	}

#pragma endregion

#pragma region Vector3

	/// <summary>
	/// Vector3型からJson型への変換
	/// </summary>
	void to_json(Json& json, const Vector3& v)
	{
		json["x"] = v.x;
		json["y"] = v.y;
		json["z"] = v.z;
	}

	/// <summary>
	/// Json型からVector3型への変換
	/// </summary>
	void from_json(const Json& json, Vector3& v)
	{
		json["x"].get_to(v.x);
		json["y"].get_to(v.y);
		json["z"].get_to(v.z);
	}

#pragma endregion
}