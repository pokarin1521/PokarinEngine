/**
* @file Rigidbody.cpp
*/
#include "Rigidbody.h"

#include "../Time.h"
#include "../Scene.h"

namespace PokarinEngine
{
	/// <summary>
	/// �Q�[���Đ����̍X�V
	/// </summary>
	void Rigidbody::Update_PlayGame()
	{
		// �R���|�[�l���g�̎�����
		const GameObject& owner = GetOwnerObject();
		
		// �O��̍X�V����̌o�ߎ���
		float deltaTime = Time::DeltaTime();

		// �ڒn���Ă���
		if (owner.isGrounded)
		{
			// ���n��͗���������K�v���Ȃ��̂ŁA�������ւ̑��x��0�ɂ���
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