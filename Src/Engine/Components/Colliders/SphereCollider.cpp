/**
* @file SphereCollider.cpp
*/
#include "SphereCollider.h"

#include "ImGui/imgui.h"

namespace PokarinEngine
{
	/// <summary>
	/// ���W�ϊ������R���C�_�[���擾����
	/// </summary>
	/// <param name="[in] transform"> ���W�ϊ��s�� </param>
	/// <returns> ���W�ϊ������R���C�_�[ </returns>
	ColliderPtr SphereCollider::GetTransformedCollider(
		const Matrix4x4& transform) const
	{
		// -------------------------------
		// ���S���W�����W�ϊ�����
		// -------------------------------

		// ���W�ϊ��������̃R���C�_�[���i�[����ϐ�
		auto collider = std::make_shared<SphereCollider>();

		// ���W�ϊ�
		collider->sphere.center = Vector3(
			transform * Vector4(sphere.center, 1));

		// -------------------------------
		// ���̂̊g�嗦�𔽉f
		// -------------------------------

		/* ���̂̊g�嗦�́A
		XYZ���������Ȃ��Ă͂Ȃ�Ȃ�(�ȉ~�`�ɂȂ邩��)

		�I�u�W�F�N�g�̊g�嗦�͌ʂɎw��ł��Ă��܂��̂�
		�u���W�ϊ��s��Ɋ܂܂��XYZ�̊g�嗦�̂����ő�̒l�v��
		���̂̊g�嗦�Ƃ���(unity�̋��̃R���C�_�[�Ɠ�������) */

		// �g�嗦���擾
		const Vector3 scale = Matrix4x4::Scale(transform);

		// X,Y,Z�̊g�嗦�ōő�̒l
		const float maxScale = std::max({ scale.x, scale.y, scale.z });

		// �g�嗦�𔽉f
		collider->sphere.radius = sphere.radius * maxScale;

		// ���W�ϊ��������̃R���C�_�[
		return collider;
	}

	/// <summary>
	/// ����ҏW�ł���悤�ɕ\������
	/// </summary>
	void SphereCollider::InfoEditor()
	{
		// ---------------------
		// �d��
		// ---------------------

		ImGui::Checkbox("Is Trigger", &isTrigger);

		// ---------------------
		// ���S���W
		// ---------------------

		sphere.center.RenderDrag("Center", "SphereCollider", sliderWidth);		
		
		// ---------------------
		// �傫��
		// ---------------------

		ImGui::DragFloat("Center##SphereCollider", &sphere.radius);
	}
}