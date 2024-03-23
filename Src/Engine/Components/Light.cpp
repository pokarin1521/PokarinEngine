/**
* @file Light.cpp
*/
#include "Light.h"
#include "../Engine.h"
#include "../LightManager.h"

namespace PokarinEngine
{
	/// <summary>
	/// ���C�g���������f�X�g���N�^
	/// </summary>
	Light::~Light()
	{
		// ���C�g���������
		LightManager::DeallocateLight(lightIndex);
	}

	/// <summary>
	/// �R���|�[�l���g��������
	/// �Q�[���I�u�W�F�N�g�ɒǉ������Ƃ��Ɏ��s
	/// </summary>
	void Light::Awake()
	{
		// ���C�g���Ȃ���Ύ擾����
		if (lightIndex < 0)
		{
			lightIndex = LightManager::AllocateLight();
		}
	}

	/// <summary>
	/// �R���|�[�l���g���X�V
	/// </summary>
	/// <param name="deltaTime"> �O��̍X�V����̌o�ߎ���(�b) </param>
	void Light::Update(float deltaTime)
	{
		// -------------------
		// ���C�g���擾
		// -------------------

		// �R���|�[�l���g�̎�����
		const GameObject& owner = GetOwner();

		// ���C�g(�Ȃ����nullptr������)
		auto light = LightManager::GetLight(lightIndex);

		// -----------------------------------------------
		// �R���|�[�l���g�̒l���擾�������C�g�ɔ��f����
		// -----------------------------------------------

		// ���C�g������̂ŁA�l�𔽉f����
		if (light)
		{
			// -------------------- 
			// ���C�g�S�� 
			// --------------------

			// ���W
			light->position = owner.transform->position;

			// ���邳
			light->intensity = intensity;

			// �F
			light->color = color;

			// ���a
			light->radius = radius;

			// �ő�Ǝˊp�x
			// �ŏ��̓|�C���g���C�g�Ƃ��Ĉ���
			light->coneAngle = 0;

			// ---------------------- 
			// �X�|�b�g���C�g 
			// ----------------------

			// ��ނ��X�|�b�g���C�g�Ȃ�
			// �X�|�b�g���C�g�̒l�𔽉f����
			if (type == Type::spotLight)
			{
				// ���C�g�̌���
				// ��]�Ȃ��̌����x�N�g���Ƃ��ď�����
				Vec3 direction = { 0, 0, -1 };

				// ------ X����]�𔽉f���������x�N�g�����v�Z ------

				// sin, cos
				const float sinX = sin(owner.transform->rotation.x);
				const float cosX = cos(owner.transform->rotation.x);

				// ��]�𔽉f���������x�N�g�����v�Z
				direction = {
					direction.x,
					direction.y * cosX - direction.z * sinX,
					direction.y * sinX + direction.z * cosX };

				// ------ Y����]�𔽉f���������x�N�g�����v�Z ------

				// sin, cos
				const float sinY = sin(owner.transform->rotation.y);
				const float cosY = cos(owner.transform->rotation.y);

				// ��]�𔽉f���������x�N�g�����v�Z
				direction = {
					direction.x * cosY + direction.z * sinY,
					direction.y,
					-direction.x * sinY + direction.z * cosY };

				// ------------- �p�����[�^�𔽉f -------------

				light->directoin = direction;
				light->coneAngle = coneAngle;
				light->falloffAngle = falloffAngle;
			}

		} // if light
	}

} // namespace PokarinEngine