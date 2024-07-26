/**
* @file BoxCollider.cpp
*/
#include "BoxCollider.h"

#include "../../GameObject.h"

namespace PokarinEngine
{
	/// <summary>
	/// ���W�ϊ������R���C�_�[���擾����
	/// </summary>
	/// <returns> ���W�ϊ������R���C�_�[ </returns>
	ColliderPtr BoxCollider::GetTransformedCollider() const
	{
		// ���g�̍��W�ϊ��s��
		Matrix4x4 transformMatrix = GetTransformMatrix();

		// ������ł���Q�[���I�u�W�F�N�g�̍��W�ϊ��s��
		Matrix4x4 objectMatrix = GetOwnerObject().transform->GetTransformMatrix();

		// ���W�ϊ��p�R���C�_�[
		auto collider = std::make_shared<BoxCollider>();

		// ���S���W���擾����
		collider->box.center = Matrix4x4::Translate(transformMatrix);

		// �傫�����擾����
		collider->box.size = Matrix4x4::Scale(transformMatrix);

		// ���x�N�g���Ƒ傫����ϊ�����
		for (int i = 0; i < Vector3::size; ++i)
		{
			// ���x�N�g����ϊ�
			// �����x�N�g���Ȃ̂ŁA��]�s��ŕϊ�����
			collider->box.axis[i] = Matrix4x4::Rotate(objectMatrix) * box.axis[i];
		}

		return collider;
	}

	/// <summary>
	/// ���W�ϊ��s����擾����
	/// </summary>
	/// <returns> ���W�ϊ��s�� </returns>
	Matrix4x4 BoxCollider::GetTransformMatrix() const
	{
		// ������ł���Q�[���I�u�W�F�N�g�̈ʒu�E��]�E�g�嗦
		TransformPtr transform = GetOwnerObject().transform;

		// ������ł���Q�[���I�u�W�F�N�g�̍��W�ϊ��s��
		Matrix4x4 transformMatrix = transform->GetTransformMatrix();

		// ���W�ϊ��������S���W
		const Vector3 center = Vector3(transformMatrix * Vector4(box.center, 1));

		// �g�嗦�𔽉f�����傫��
		Vector3 size = box.size * Matrix4x4::Scale(transformMatrix);

		// ���W�ϊ��s����쐬���Đݒ肷��
		return Matrix4x4::CreateTransformMatrix(center, transform->rotation, size);
	}

	/// <summary>
	/// �R���|�[�l���g�̏���Json�^�Ɋi�[����
	/// </summary>
	/// <param name="[out] Json"> �����i�[����Json�^ </param>
	void BoxCollider::ToJson(Json& data) const
	{

	}

	/// <summary>
	/// �R���|�[�l���g�̏���Json�^����擾����
	/// </summary>
	/// <param name="[out] data"> �����i�[���Ă���Json�^ </param>
	void BoxCollider::FromJson(const Json& data)
	{

	}

	/// <summary>
	/// ����ҏW�ł���悤�ɕ\������
	/// </summary>
	void BoxCollider::InfoEditor()
	{
		// ���S���W��\��
		box.center.RenderDrag("Center", GetID_String(), sliderWidth);

		// �傫����\��
		box.size.RenderDrag("Size", GetID_String(), sliderWidth);
	}
}