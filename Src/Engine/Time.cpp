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
		// 変数
		// -------------------------

		// 前回更新時の時間
		double previousTime = 0;

		// 前回の更新からの経過時間
		float deltaTime = 0;

		// -------------------------
		// 関数
		// -------------------------

		/// <summary>
		/// 前回更新からの経過時間を取得する
		/// </summary>
		/// <returns> 前回更新からの経過時間 </returns>
		float DeltaTime()
		{
			return deltaTime;
		}

		/// <summary>
		/// 時間を更新する
		/// </summary>
		void Update()
		{
			// ------------------------------------------
			// 前回の更新からの経過時間
			// ------------------------------------------
			
			// 現在の時間
			const double currentTime = glfwGetTime(); 

			// 前回の更新からの経過時間を計算
			deltaTime = static_cast<float>(currentTime - previousTime);

			// 時間を更新
			previousTime = currentTime;

			// ---------------------------------
			// デバッグ対策
			// ---------------------------------

			// ブレークポイントで一時停止しても時間は進むので経過時間が長くなりすぎる
			// なので、経過時間が長すぎる場合は適当に短くする
			if (deltaTime >= 0.5f)
			{
				deltaTime = 1.0f / 60.0f;
			}
		}
	}
}