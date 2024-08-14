/**
* @file Angle.h
*/
#ifndef POKARINENGINE_ANGLE_H_INCLUDED
#define POKARINENGINE_ANGLE_H_INCLUDED

#include "Vector.h"

namespace PokarinEngine
{
	// 円周率
	inline constexpr float pi = 3.1415926535f;

	/// <summary>
	/// 度数法を弧度法に変換する
	/// </summary>
	/// <param name="[in] deg"> 角度(度数法) </param>
	/// <returns> 角度(弧度法) </returns>
	constexpr float Radians(float deg)
	{
		return deg * pi / 180.0f;
	}

	/// <summary>
	/// 度数法を弧度法に変換する
	/// </summary>
	/// <param name="[in] deg"> 角度(度数法) </param>
	/// <returns> 角度(弧度法) </returns>
	constexpr Vector3 Radians(const Vector3& deg)
	{
		return Vector3(Radians(deg.x), Radians(deg.y), Radians(deg.z));
	}

	/// <summary>
	/// 弧度法を度数法に変換する
	/// </summary>
	/// <param name="[in] rad"> 角度(弧度法) </param>
	/// <returns> 角度(度数法) </returns>
	constexpr float Degrees(float rad)
	{
		return rad * 180.0f / pi;
	}

	/// <summary>
	/// 弧度法を度数法に変換する
	/// </summary>
	/// <param name="[in] rad"> 角度(弧度法) </param>
	/// <returns> 角度(度数法) </returns>
	constexpr Vector3 Degrees(const Vector3& rad)
	{
		return Vector3(Degrees(rad.x), Degrees(rad.y), Degrees(rad.z));
	}
}

#endif // !POKARINENGINE_ANGLE_H_INCLUDED
