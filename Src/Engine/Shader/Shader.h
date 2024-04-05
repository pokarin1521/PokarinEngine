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
	/// <summary>
	/// シェーダ管理用
	/// </summary>
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

		// -------------------------------
		// 種類
		// -------------------------------

		/// <summary>
		/// シェーダプログラムの種類
		/// </summary>
		enum class ProgType
		{
			Standard, // 標準
			Unlit,    // ライティング無し 
		};

		// --------------------------------
		// 関数
		// --------------------------------

		/// <summary>
		/// シェーダを初期化
		/// </summary>
		void Initialize();

		/// <summary>
		/// シェーダプログラムの管理番号を取得する
		/// </summary>
		/// <param name="progType"> シェーダプログラムの種類 </param>
		/// <returns> 指定した種類のシェーダプログラムの管理番号 </returns>
		GLuint GetProgram(const ProgType& progType);

		/// <summary>
		/// 全てのシェーダプログラムの管理番号を取得する
		/// </summary>
		/// <returns> 
		/// <para> 全てのシェーダプログラムの管理番号 </para>
		/// <para> [シェーダプログラムの種類, シェーダプログラムの管理番号] </para>
		/// </returns>
		const ProgList& GetAllProgram();

	} // namespace Shader

} // namespace PokarinEngine

#endif // !SHADER_H_INCLUDED
