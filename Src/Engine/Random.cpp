/**
* @file Random.cpp
*/
#include "Random.h"

#include <random>

namespace PokarinEngine
{
	namespace Random
	{
		// -------------------------------------
		// �ϐ�
		// -------------------------------------

		// ���������I�u�W�F�N�g
		// �^�̗���(��ɗ\���s�\�̗���)��Ԃ�
		// �V�[�h(��)�Ƃ��Ďg��
		std::random_device rd;

		// �����Z���k�E�c�C�X�^�[�Ƃ����A���S���Y�����g���Ă���
		// �����p�^�[�����J��Ԃ������������ɒ���
		std::mt19937 randomEngine(rd());

		// ----------------------------------------------
		// �֐�
		// ----------------------------------------------

		/// <summary>
		/// ����������������
		///	</summary>
		/// <param name="seed"> �����̎� </param>
		void Initialize(int seed)
		{
			// �V�[�h���w��
			randomEngine.seed(seed);
		}

		/// <summary>
		/// 0.0 �` 1.0 �͈̔͂̈�l�������擾����
		/// </summary>
		/// <returns> ������������ </returns>
		float Value()
		{
			// �w�肵���͈͂̈�l������Ԃ�
			// real�͎���
			return std::uniform_real_distribution<float>(0, 1)(randomEngine);
		}

		/// <summary>
		/// �w�肳�ꂽ�͈̔͂̈�l�������擾����
		/// </summary>
		/// <param name="min"> �����͈͂̍ŏ��l </param>
		/// <param name="max"> �����͈͂̍ő�l </param>
		/// <returns> ������������ </returns>
		int Range(int min, int max)
		{
			// �w�肵���͈͂̈�l������Ԃ�
			// real�͎���
			return std::uniform_int_distribution<int>(min, max)(randomEngine);
		}

		/// <summary>
		/// �w�肳�ꂽ�͈̔͂̈�l�������擾����
		/// </summary>
		/// <param name="min"> �����͈͂̍ŏ��l </param>
		/// <param name="max"> �����͈͂̍ő�l </param>
		/// <returns> ������������ </returns>
		float Range(float min, float max)
		{
			// �w�肵���͈͂̈�l������Ԃ�
			// real�͎���
			return std::uniform_real_distribution<float>(min, max)(randomEngine);
		}

	} // namespace Random

} // namespace PokarinEngine