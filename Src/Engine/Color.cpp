/**
* @file Color.cpp
*/
#include "Color.h"

#include "ImGui/imgui.h"

#include "Math/Vector.h"

namespace PokarinEngine
{
	/// <summary>
	/// Vector4Œ^‚Å‰Šú‰»‚·‚éƒRƒ“ƒXƒgƒ‰ƒNƒ^
	/// </summary>
	Color::Color(const Vector4& v)
		: r(v.x), g(v.y), b(v.z), a(v.w) 
	{

	}

	/// <summary>
	/// ImVec4Œ^‚É•ÏŠ·
	/// </summary>
	Color::operator ImVec4() const
	{
		return ImVec4(r, g, b, a);
	}

	/// <summary>
	/// Vector3Œ^‚É•ÏŠ·
	/// </summary>
	Color::operator Vector3() const 
	{ 
		return Vector3(r, g, b);;
	}

	/// <summary>
	/// Vector4Œ^‚É•ÏŠ·
	/// </summary>
	Color::operator Vector4() const 
	{ 
		return Vector4(r, g, b, a); 
	}
}