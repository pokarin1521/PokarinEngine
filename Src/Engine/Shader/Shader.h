/**
* @file Shader.h
*/
#ifndef SHADER_H_INCLUDED
#define SHADER_H_INCLUDED

#include "glad/glad.h"

#include <GLFW/glfw3.h>
#include <unordered_map>

namespace PokarinEngine
{
	namespace Shader
	{
		// --------------------------
		// 前方宣言
		// --------------------------

		enum class ProgType;

		// --------------------------
		// 型の別名を定義
		// --------------------------

		// シェーダプログラムの管理番号配列
		// <シェーダプログラムの種類, シェーダプログラムの管理番号>
		using ProgList = std::unordered_map<ProgType, GLuint>;

		/// <summary>
		/// シェーダプログラムの種類
		/// </summary>
		enum class ProgType
		{
			Standard, // 標準
			Unlit,    // ライティング無し 
		};

		/// <summary>
		/// シェーダを初期化
		/// </summary>
		/// <param name="[out] progList"> シェーダプログラムの管理番号配列 </param>
		void Initialize(ProgList& progList);

	} // namespace Shader

} // namespace PokarinEngine

#endif // !SHADER_H_INCLUDED
