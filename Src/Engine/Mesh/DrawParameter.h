/**
* @file DrawParameter.h
*/
#ifndef POKARINENGINE_DRAWPARAMETER_H_INCLUDED
#define POKARINENGINE_DRAWPARAMETER_H_INCLUDED

#include "glad/glad.h"

namespace PokarinEngine
{
	/// <summary>
	/// 描画パラメータ
	/// </summary>
	struct DrawParameter
	{
		// プリミティブの種類
		GLenum mode = GL_TRIANGLES;

		// 描画するインデックス数
		GLsizei count = 0;

		// 描画開始インデックスのバイトオフセット
		const void* indices = 0;

		// インデックス0となる頂点配列の要素番号
		GLint baseVertex = 0;

		// マテリアルインデックス
		int materialNo = -1;
	};
}

#endif // !POKARINENGINE_DRAWPARAMETER_H_INCLUDED
