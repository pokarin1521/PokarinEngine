/**
* @file Light.cpp
*/
#include "Light.h"

#include "ImGui/imgui.h"

#include "../Engine.h"

namespace PokarinEngine
{
	/// <summary>
	/// ���C�g���������f�X�g���N�^
	/// </summary>
	Light::~Light()
	{
		LightParameter::EraseLightData(lightData);
	}

	/// <summary>
	/// <para> �R���|�[�l���g�������� </para> 
	/// <para> �Q�[���I�u�W�F�N�g�ɒǉ������Ƃ��Ɏ��s </para>
	/// </summary>
	void Light::Awake()
	{
		LightParameter::AddLightData(lightData);
	}

	/// <summary>
	/// �R���|�[�l���g���X�V
	/// </summary>
	/// <param name="deltaTime"> �O��̍X�V����̌o�ߎ���(�b) </param>
	void Light::Update(float deltaTime)
	{
		// --------------------------------
		// ���C�g�̍��W��ݒ�
		// --------------------------------

		// �R���|�[�l���g�̎�����
		const GameObject& owner = GetOwner();

		// ���C�g�̍��W��ݒ�
		lightData.position = owner.transform->position;

		// �|�C���g���C�g�Ȃ�����͕s�v�Ȃ̂Ō����̌v�Z�͂��Ȃ�
		if (lightData.type == LightParameter::Type::pointLight)
		{
			return;
		}

		// -------------------------------------------
		// �������K�v�Ȃ̂Ōv�Z����
		// -------------------------------------------

		// ���C�g�̌���
		// ��]�Ȃ��̌����x�N�g���Ƃ��ď�����
		Vector3 direction = { 0, 0, -1 };

		// ------ X����]�𔽉f���������x�N�g�����v�Z ------

		// �������X��]�p�x(Sin)
		const float sinX = sin(owner.transform->rotation.x);

		// �������X��]�p�x(Cos)
		const float cosX = cos(owner.transform->rotation.x);

		// ��]�𔽉f���������x�N�g�����v�Z
		direction = {
			direction.x,
			direction.y * cosX - direction.z * sinX,
			direction.y * sinX + direction.z * cosX };

		// ------ Y����]�𔽉f���������x�N�g�����v�Z ------

		// �������Y��]�p�x(Sin)
		const float sinY = sin(owner.transform->rotation.y);

		// �������Y��]�p�x(Cos)
		const float cosY = cos(owner.transform->rotation.y);

		// ��]�𔽉f���������x�N�g�����v�Z
		direction = {
			direction.x * cosY + direction.z * sinY,
			direction.y,
			-direction.x * sinY + direction.z * cosY };

		// ---------------------------------- 
		// ���C�g�̌�����ݒ� 
		// ----------------------------------

		lightData.direction = direction;
	}

	/// <summary>
	/// �G�f�B�^�ɏ���`�悷��
	/// </summary>
	void Light::RenderInfo()
	{
		if (ImGui::CollapsingHeader("Light"))
		{
			ImGui::Text("Light Information");
		}
	}

} // namespace PokarinEngine