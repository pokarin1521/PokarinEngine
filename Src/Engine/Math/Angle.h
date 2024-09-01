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
	constexpr float DegToRad(float deg)
	{
		return deg * pi / 180.0f;
	}

	/// <summary>
	/// 度数法を弧度法に変換する
	/// </summary>
	/// <param name="[in] deg"> 角度(度数法) </param>
	/// <returns> 角度(弧度法) </returns>
	constexpr Vector3 DegToRad(const Vector3& deg)
	{
		return Vector3(DegToRad(deg.x), DegToRad(deg.y), DegToRad(deg.z));
	}

	/// <summary>
	/// 弧度法を度数法に変換する
	/// </summary>
	/// <param name="[in] rad"> 角度(弧度法) </param>
	/// <returns> 角度(度数法) </returns>
	constexpr float RadToDeg(float rad)
	{
		return rad * 180.0f / pi;
	}

	/// <summary>
	/// 弧度法を度数法に変換する
	/// </summary>
	/// <param name="[in] rad"> 角度(弧度法) </param>
	/// <returns> 角度(度数法) </returns>
	constexpr Vector3 RadToDeg(const Vector3& rad)
	{
		return Vector3(RadToDeg(rad.x), RadToDeg(rad.y), RadToDeg(rad.z));
	}
}

#endif // !POKARINENGINE_ANGLE_H_INCLUDED
