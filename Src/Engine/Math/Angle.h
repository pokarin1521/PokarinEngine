/**
* @file Angle.h
*/
#ifndef POKARINENGINE_ANGLE_H_INCLUDED
#define POKARINENGINE_ANGLE_H_INCLUDED

#include "Vector.h"

namespace PokarinEngine
{
	// �~����
	inline constexpr float pi = 3.1415926535f;

	/// <summary>
	/// �x���@���ʓx�@�ɕϊ�����
	/// </summary>
	/// <param name="[in] deg"> �p�x(�x���@) </param>
	/// <returns> �p�x(�ʓx�@) </returns>
	constexpr float Radians(float deg)
	{
		return deg * pi / 180.0f;
	}

	/// <summary>
	/// �x���@���ʓx�@�ɕϊ�����
	/// </summary>
	/// <param name="[in] deg"> �p�x(�x���@) </param>
	/// <returns> �p�x(�ʓx�@) </returns>
	constexpr Vector3 Radians(const Vector3& deg)
	{
		return Vector3(Radians(deg.x), Radians(deg.y), Radians(deg.z));
	}

	/// <summary>
	/// �ʓx�@��x���@�ɕϊ�����
	/// </summary>
	/// <param name="[in] rad"> �p�x(�ʓx�@) </param>
	/// <returns> �p�x(�x���@) </returns>
	constexpr float Degrees(float rad)
	{
		return rad * 180.0f / pi;
	}

	/// <summary>
	/// �ʓx�@��x���@�ɕϊ�����
	/// </summary>
	/// <param name="[in] rad"> �p�x(�ʓx�@) </param>
	/// <returns> �p�x(�x���@) </returns>
	constexpr Vector3 Degrees(const Vector3& rad)
	{
		return Vector3(Degrees(rad.x), Degrees(rad.y), Degrees(rad.z));
	}
}

#endif // !POKARINENGINE_ANGLE_H_INCLUDED
