/**
* @file VectorToJson.h
*/
#ifndef VECTORTOJSON_H_INCLUDED
#define VECTORTOJSON_H_INCLUDED

#include "Vector.h"
#include "Json/Json.h"

namespace PokarinEngine
{
	/// <summary>
	/// Vector3�^�̏���Json�^�Ɋi�[����
	/// </summary>
	/// <param name="[in] vector"> Json�Ɋi�[����Vector3 </param>
	/// <param name="[out] json"> �i�[���Json </param>
	void VectorToJson(const Vector3& vector, Json& json)
	{
		json["x"] = vector.x;
		json["y"] = vector.y;
		json["z"] = vector.z;
	}
}

#endif // !VECTORTOJSON_H_INCLUDED
