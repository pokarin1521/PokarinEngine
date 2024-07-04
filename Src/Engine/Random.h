/**
* @file Random.h
*/
#ifndef RANDOM_H_INCLUDED
#define RANDOM_H_INCLUDED

namespace PokarinEngine
{
	/// <summary>
	/// 乱数生成機能を格納する名前空間
	/// </summary>
	namespace Random
	{
		/// <summary>
		/// 乱数を初期化する
		/// </summary>
		/// <param name="[in] seed"> 乱数の種 </param>
		void Initialize(int seed);

		/// <summary>
		/// 0.0 ～ 1.0 の範囲の一様乱数を取得する
		/// </summary>
		/// <returns> 生成した乱数 </returns>
		float Value();

		/// <summary>
		/// 指定されたの範囲の一様乱数を取得する
		/// </summary>
		/// <param name="[in] min"> 生成範囲の最小値 </param>
		/// <param name="[in] max"> 生成範囲の最大値 </param>
		/// <returns> 生成した乱数 </returns>
		float Range(float min, float max);

		/// <summary>
		/// 指定されたの範囲の一様乱数を取得する
		/// </summary>
		/// <param name="[in] min"> 生成範囲の最小値 </param>
		/// <param name="[in] max"> 生成範囲の最大値 </param>
		/// <returns> 生成した乱数 </returns>
		int Range(int min, int max);

	} // namespace Random

} // namespace PokarinEngine

#endif // !RANDOM_H_INCLUDED
