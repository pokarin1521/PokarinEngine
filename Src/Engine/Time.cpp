/**
* @file Time.cpp
*/
#include "Time.h"

#include <GLFW/glfw3.h>

namespace PokarinEngine
{
	namespace Time
	{
		// -------------------------
		// �ϐ�
		// -------------------------

		// �O��X�V���̎���
		double previousTime = 0;

		// �O��̍X�V����̌o�ߎ���
		float deltaTime = 0;

		// -------------------------
		// �֐�
		// -------------------------

		/// <summary>
		/// �O��X�V����̌o�ߎ��Ԃ��擾����
		/// </summary>
		/// <returns> �O��X�V����̌o�ߎ��� </returns>
		float DeltaTime()
		{
			return deltaTime;
		}

		/// <summary>
		/// ���Ԃ��X�V����
		/// </summary>
		void Update()
		{
			// ------------------------------------------
			// �O��̍X�V����̌o�ߎ���
			// ------------------------------------------
			
			// ���݂̎���
			const double currentTime = glfwGetTime(); 

			// �O��̍X�V����̌o�ߎ��Ԃ��v�Z
			deltaTime = static_cast<float>(currentTime - previousTime);

			// ���Ԃ��X�V
			previousTime = currentTime;

			// ---------------------------------
			// �f�o�b�O�΍�
			// ---------------------------------

			// �u���[�N�|�C���g�ňꎞ��~���Ă����Ԃ͐i�ނ̂Ōo�ߎ��Ԃ������Ȃ肷����
			// �Ȃ̂ŁA�o�ߎ��Ԃ���������ꍇ�͓K���ɒZ������
			if (deltaTime >= 0.5f)
			{
				deltaTime = 1.0f / 60.0f;
			}
		}
	}
}