/**
* @file Random.h
*/
#ifndef RANDOM_H_INCLUDED
#define RANDOM_H_INCLUDED

namespace PokarinEngine
{
	/// <summary>
	/// ���������@�\���i�[���閼�O���
	/// </summary>
	namespace Random
	{
		/// <summary>
		/// ����������������
		/// </summary>
		/// <param name="[in] seed"> �����̎� </param>
		void Initialize(int seed);

		/// <summary>
		/// 0.0 �` 1.0 �͈̔͂̈�l�������擾����
		/// </summary>
		/// <returns> ������������ </returns>
		float Value();

		/// <summary>
		/// �w�肳�ꂽ�͈̔͂̈�l�������擾����
		/// </summary>
		/// <param name="[in] min"> �����͈͂̍ŏ��l </param>
		/// <param name="[in] max"> �����͈͂̍ő�l </param>
		/// <returns> ������������ </returns>
		float Range(float min, float max);

		/// <summary>
		/// �w�肳�ꂽ�͈̔͂̈�l�������擾����
		/// </summary>
		/// <param name="[in] min"> �����͈͂̍ŏ��l </param>
		/// <param name="[in] max"> �����͈͂̍ő�l </param>
		/// <returns> ������������ </returns>
		int Range(int min, int max);

	} // namespace Random

} // namespace PokarinEngine

#endif // !RANDOM_H_INCLUDED
