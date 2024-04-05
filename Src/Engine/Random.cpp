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
		// 変数
		// -------------------------------------

		// 乱数生成オブジェクト
		// 真の乱数(常に予測不能の乱数)を返す
		// シード(種)として使う
		std::random_device rd;

		// メルセンヌ・ツイスターというアルゴリズムを使っている
		// 同じパターンが繰り返される周期が非常に長い
		std::mt19937 randomEngine(rd());

		// ----------------------------------------------
		// 関数
		// ----------------------------------------------

		/// <summary>
		/// 乱数を初期化する
		///	</summary>
		/// <param name="seed"> 乱数の種 </param>
		void Initialize(int seed)
		{
			// シードを指定
			randomEngine.seed(seed);
		}

		/// <summary>
		/// 0.0 ～ 1.0 の範囲の一様乱数を取得する
		/// </summary>
		/// <returns> 生成した乱数 </returns>
		float Value()
		{
			// 指定した範囲の一様乱数を返す
			// realは実数
			return std::uniform_real_distribution<float>(0, 1)(randomEngine);
		}

		/// <summary>
		/// 指定されたの範囲の一様乱数を取得する
		/// </summary>
		/// <param name="min"> 生成範囲の最小値 </param>
		/// <param name="max"> 生成範囲の最大値 </param>
		/// <returns> 生成した乱数 </returns>
		int Range(int min, int max)
		{
			// 指定した範囲の一様乱数を返す
			// realは実数
			return std::uniform_int_distribution<int>(min, max)(randomEngine);
		}

		/// <summary>
		/// 指定されたの範囲の一様乱数を取得する
		/// </summary>
		/// <param name="min"> 生成範囲の最小値 </param>
		/// <param name="max"> 生成範囲の最大値 </param>
		/// <returns> 生成した乱数 </returns>
		float Range(float min, float max)
		{
			// 指定した範囲の一様乱数を返す
			// realは実数
			return std::uniform_real_distribution<float>(min, max)(randomEngine);
		}

	} // namespace Random

} // namespace PokarinEngine