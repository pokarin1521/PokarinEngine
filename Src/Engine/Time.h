/**
* @file Time.h
*/
#ifndef POKARINENGINE_TIME_H_INCLUDED
#define POKARINENGINE_TIME_H_INCLUDED

namespace PokarinEngine
{
	namespace Time
	{
		/// <summary>
		/// �O��X�V����̌o�ߎ��Ԃ��擾����
		/// </summary>
		/// <returns> �O��X�V����̌o�ߎ��� </returns>
		float DeltaTime();

		/// <summary>
		/// ���Ԃ��X�V����
		/// </summary>
		void Update();
	}
}

#endif // !POKARINENGINE_TIME_H_INCLUDED
