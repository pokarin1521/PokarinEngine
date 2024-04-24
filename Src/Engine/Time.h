/**
* @file Time.h
*/
#ifndef TIME_H_INCLUDED
#define TIME_H_INCLUDED

namespace PokarinEngine
{
	namespace Time
	{
		/// <summary>
		/// 前回更新からの経過時間を取得する
		/// </summary>
		/// <returns> 前回更新からの経過時間 </returns>
		float DeltaTime();

		/// <summary>
		/// 時間を更新する
		/// </summary>
		void Update();
	}
}

#endif // !TIME_H_INCLUDED
