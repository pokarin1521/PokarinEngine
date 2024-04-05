/**
* @file AabbCollider.cpp
*/
#include "AabbCollider.h"

namespace PokarinEngine
{
	/// <summary>
	/// <para> ���W�ϊ������R���C�_�[���擾���� </para>
	/// <para> ��]�p�x��90�x�P�ʂŎw�肷�邱�� </para>
	/// <para> ����ȊO�̊p�x�ł͐������������肪�s���Ȃ� </para>
	/// </summary>
	/// <param name="transform"> ���W�ϊ��s�� </param>
	/// <returns> ���W�ϊ������R���C�_�[ </returns>
	ColliderPtr AabbCollider::GetTransformedCollider(
		const Matrix4x4& transform) const
	{
		// ------------------------------
		// ���W�ϊ������R�s�[���쐬
		// ------------------------------

		// ���W�ϊ�����AABB�R���C�_�[���i�[����ϐ�
		auto p = std::make_shared<AabbCollider>();

		// ���W�ϊ�
		p->aabb.min = Vector3(transform * Vector4(aabb.min, 1));
		p->aabb.max = Vector3(transform * Vector4(aabb.max, 1));

		// ---------------------------------------
		// min�̕����傫�����������ւ���
		// ---------------------------------------

		for (int i = 0; i < Vector_Size::vector3; ++i)
		{
			if (p->aabb.min[i] > p->aabb.max[i])
			{
				const float tmp = p->aabb.min[i];
				p->aabb.min[i] = p->aabb.max[i];
				p->aabb.max[i] = tmp;
			}
		}

		// ���W�ϊ�����AABB�R���C�_�[
		return p;
	}
}