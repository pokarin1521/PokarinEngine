/**
* @file DrawParameter.h
*/
#ifndef POKARINENGINE_DRAWPARAMETER_H_INCLUDED
#define POKARINENGINE_DRAWPARAMETER_H_INCLUDED

#include "glad/glad.h"

#include <vector>

namespace PokarinEngine
{
	// -----------------------
	// 前方宣言
	// -----------------------

	struct DrawParameter;

	// --------------------------------
	// 型の別名を定義
	// --------------------------------

	using DrawParameterList = std::vector<DrawParameter>;

	/// <summary>
	/// 描画パラメータ
	/// </summary>
	struct DrawParameter
	{
		// プリミティブの種類
		GLenum mode = GL_TRIANGLES;

		// 描画するインデックス数
		GLsizei count = 0;

		// 描画開始インデックスの位置(バイト数で指定)
		const void* indices = 0;

		// インデックス0となる頂点配列の要素番号
		GLint baseVertex = 0;

		// マテリアルインデックス
		int materialNo = -1;
	};
}

#endif // !POKARINENGINE_DRAWPARAMETER_H_INCLUDED
