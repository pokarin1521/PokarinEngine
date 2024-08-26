/**
* @file Vector.cpp
*/
#include "Vector.h"

#include "Json/Json.h"

namespace PokarinEngine
{
#pragma region static_zero

	// ‘S‚Ä‚Ì’l‚ª0
	const Vector2 Vector2::zero = { 0,0 };

	// ‘S‚Ä‚Ì’l‚ª0
	const Vector3 Vector3::zero = { 0,0,0 };

	// ‘S‚Ä‚Ì’l‚ª0
	const Vector4 Vector4::zero = { 0,0,0,0 };

#pragma endregion

#pragma region static_one

	// ‘S‚Ä‚Ì’l‚ª1
	const Vector2 Vector2::one = { 1,1 };

	// ‘S‚Ä‚Ì’l‚ª1
	const Vector3 Vector3::one = { 1,1,1 };

	// ‘S‚Ä‚Ì’l‚ª1
	const Vector4 Vector4::one = { 1,1,1,1 };

#pragma endregion

#pragma region Json

	/// <summary>
	/// î•ñ‚ğJsonŒ^‚ÉŠi”[‚·‚é
	/// </summary>
	/// <param name="[out] data"> î•ñ‚ğŠi”[‚·‚éJsonŒ^ </param>
	void Vector3::ToJson(Json& data) const
	{
		data["x"] = x;
		data["y"] = y;
		data["z"] = z;
	}

	/// <summary>
	/// î•ñ‚ğJsonŒ^‚©‚çæ“¾‚·‚é
	/// </summary>
	/// <param name="[in] data"> î•ñ‚ğŠi”[‚µ‚Ä‚¢‚éJsonŒ^ </param>
	void Vector3::FromJson(const Json& data)
	{
		x = data["x"];
		y = data["y"];
		z = data["z"];
	}

#pragma endregion
}