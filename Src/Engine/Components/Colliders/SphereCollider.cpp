/**
* @file SphereCollider.cpp
*/
#include "SphereCollider.h"

#include "ImGui/imgui.h"

#include "../../GameObject.h"

namespace PokarinEngine
{
	/// <summary>
	/// ���W�ϊ������R���C�_�[���擾����
	/// </summary>
	/// <returns> ���W�ϊ������R���C�_�[ </returns>
	ColliderPtr SphereCollider::GetTransformedCollider() const
	{
		// ���g�̍��W�ϊ��s��
		Matrix4x4 transformMatrix = GetTransformMatrix();

		// ���W�ϊ������R���C�_�[
		auto collider = std::make_shared<SphereCollider>();

		// ���S���W���擾����
		collider->sphere.center = Matrix4x4::Translate(transformMatrix);

		// ���a���擾����
		// ���W�ϊ��s��͒��a�ō쐬����Ă���̂ŁA�����ɂ���
		collider->sphere.radius = Matrix4x4::Scale(transformMatrix).x / 2.0f;

		return collider;
	}

	/// <summary>
	/// ���W�ϊ��s����擾����
	/// </summary>
	/// <returns> ���W�ϊ��s�� </returns>
	Matrix4x4 SphereCollider::GetTransformMatrix() const
	{
		// --------------------------------------
		// ������̍��W�ϊ��s��𕪉�����
		// --------------------------------------
		
		// ������ł���Q�[���I�u�W�F�N�g�̍��W�E��]�E�g�嗦
		TransformPtr transform = GetOwnerObject().transform;

		// ������ł���Q�[���I�u�W�F�N�g�̍��W�ϊ��s��
		Matrix4x4 transformMatrix = transform->GetTransformMatrix();

		// -------------------------------
		// ���S���W�����W�ϊ�����
		// -------------------------------

		// ���W�ϊ��������S���W
		Vector3 center = Vector3(
			transformMatrix * Vector4(sphere.center, 1));

		// ----------------------------------
		// ���̂̔��a�Ɋg�嗦�𔽉f����
		// ----------------------------------

		/* ���̂̊g�嗦�́A
		XYZ���������Ȃ��Ă͂Ȃ�Ȃ�(�ȉ~�`�ɂȂ邩��)

		�I�u�W�F�N�g�̊g�嗦�͌ʂɎw��ł��Ă��܂��̂�
		�u���W�ϊ��s��Ɋ܂܂��XYZ�̊g�嗦�̂����ő�̒l�v��
		���̂̊g�嗦�Ƃ���(unity�̋��̃R���C�_�[�Ɠ�������) */

		// �g�嗦���擾
		const Vector3 scale = Matrix4x4::Scale(transformMatrix);

		// X,Y,Z�̊g�嗦�ōő�̒l
		const float maxScale = std::max({ scale.x, scale.y, scale.z });

		// �g�嗦�𔽉f�������a
		const float radius = sphere.radius * maxScale;

		// ----------------------------------------
		// ���W�ϊ��s����쐬���ĕԂ�
		// ----------------------------------------

		// ���̂̒��a
		const float diameter = radius * 2.0f;

		// ���W�ϊ��s����쐬���ĕԂ�
		// �I�u�W�F�N�g�̑傫���Ƃ��Ă͒��a�̒l���������̂ŁA���a��ݒ肷��
		return Matrix4x4::CreateTransformMatrix(
			center, transform->rotation, Vector3(diameter));
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

		sphere.center.RenderDrag("Center", GetID_String(), sliderWidth);

		// ---------------------
		// �傫��
		// ---------------------

		ImGui::DragFloat("Radius##SphereCollider", &sphere.radius);
	}
}