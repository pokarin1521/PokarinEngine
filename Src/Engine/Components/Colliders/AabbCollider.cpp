/**
* @file AabbCollider.cpp
*/
#include "AabbCollider.h"

#include "ImGui/imgui.h"

namespace PokarinEngine
{
	/// <summary>
	/// <para> ���W�ϊ������R���C�_�[���擾���� </para>
	/// <para> ��]�p�x��90�x�P�ʂŎw�肷�邱�� </para>
	/// <para> ����ȊO�̊p�x�ł͐������������肪�s���Ȃ� </para>
	/// </summary>
	/// <param name="[in] transform"> ���W�ϊ��s�� </param>
	/// <returns> ���W�ϊ������R���C�_�[ </returns>
	ColliderPtr AabbCollider::GetTransformedCollider(
		const Matrix4x4& transform) const
	{
		// ------------------------------
		// ���W�ϊ������R�s�[���쐬
		// ------------------------------

		// ���W�ϊ�����AABB�R���C�_�[���i�[����ϐ�
		auto collider = std::make_shared<AabbCollider>();

		// ���W�ϊ�
		collider->aabb.min = Vector3(transform * Vector4(aabb.min, 1));
		collider->aabb.max = Vector3(transform * Vector4(aabb.max, 1));

		// ---------------------------------------
		// min�̕����傫�����������ւ���
		// ---------------------------------------

		for (int i = 0; i < Vector3::size; ++i)
		{
			if (collider->aabb.min[i] > collider->aabb.max[i])
			{
				const float tmp = collider->aabb.min[i];
				collider->aabb.min[i] = collider->aabb.max[i];
				collider->aabb.max[i] = tmp;
			}
		}

		// ���W�ϊ�����AABB�R���C�_�[
		return collider;
	}

	/// <summary>
	/// ����ҏW�ł���悤�ɕ\������
	/// </summary>
	void AabbCollider::InfoEditor()
	{
		// -------------------------
		// �d��
		// -------------------------

		// ���ʔԍ��̕�����
		std::string id_string = GetID_String();

		//ImGui::Checkbox(IsTriggerTitle.c_str(), &isTrigger);

		// -------------------------
		// ���S���W
		// -------------------------

		aabb.center.RenderDrag("Center", id_string, sliderWidth);

		// ���S���W�Ƒ傫������
		// �ŏ����W�ƍő���W�����߂�
		aabb.min = aabb.center - aabb.size;
		aabb.max = aabb.center + aabb.size;

		// --------------------------
		// �傫��
		// --------------------------

		aabb.size.RenderDrag("Size", id_string, sliderWidth);
	}
}