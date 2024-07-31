/**
* @file Vertex.h
*/
#ifndef POKARINENGINE_VERTEX_H_INCLUDED
#define POKARINENGINE_VERTEX_H_INCLUDED

#include "../Math/Vector.h"

namespace PokarinEngine
{
	/// <summary>
	/// ���_�f�[�^
	/// </summary>
	struct Vertex
	{
		// ���_���W
		Vector3 position = Vector3::zero;

		// �e�N�X�`�����W
		Vector2 texcoord = Vector2::zero;

		// �@���x�N�g��
		Vector3 normal = Vector3::zero;
	};
}

#endif // !POKARINENGINE_VERTEX_H_INCLUDED
