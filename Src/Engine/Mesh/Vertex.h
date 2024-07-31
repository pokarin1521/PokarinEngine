/**
* @file Vertex.h
*/
#ifndef POKARINENGINE_VERTEX_H_INCLUDED
#define POKARINENGINE_VERTEX_H_INCLUDED

#include "../Math/Vector.h"

namespace PokarinEngine
{
	/// <summary>
	/// 頂点データ
	/// </summary>
	struct Vertex
	{
		// 頂点座標
		Vector3 position = Vector3::zero;

		// テクスチャ座標
		Vector2 texcoord = Vector2::zero;

		// 法線ベクトル
		Vector3 normal = Vector3::zero;
	};
}

#endif // !POKARINENGINE_VERTEX_H_INCLUDED
