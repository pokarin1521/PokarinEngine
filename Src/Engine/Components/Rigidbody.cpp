/**
* @file Rigidbody.cpp
*/
#include "Rigidbody.h"

#include "../Time.h"

namespace PokarinEngine
{
	/// <summary>
	/// Rigibody�R���|�[�l���g�̍X�V
	/// </summary>
	void Rigidbody::Update()
	{
		// �R���|�[�l���g�̎�����
		const GameObject& owner = GetOwner();

		// �O��̍X�V����̌o�ߎ���
		float deltaTime = Time::DeltaTime();

		// �ڒn���Ă���
		if (owner.isGrounded)
		{
			// �������ւ̉����x��0�ɂ���
			velocity.y = std::max(velocity.y, 0.0f);
		}
		// �ڒn���Ă��Ȃ�
		else
		{
			// �d�͉����x�ɂ���đ��x���X�V����
			velocity.y -= gravity * gravityScale * deltaTime;
		}

		// ���x�����W�ɔ��f����
		owner.transform->position += velocity * deltaTime;
	}

} // namespace PokarinEngine