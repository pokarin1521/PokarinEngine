/**
* @file VertexArrayObject.h
*/
#ifndef VERTEXARRAYOBJECT_H_INCLUDED
#define VERTEXARRAYOBJECT_H_INCLUDED

#include "glad/glad.h"

#include <memory>

namespace PokarinEngine
{
	// -----------------
	// 前方宣言
	// -----------------

	class VertexArrayObject;

	// -----------------------
	// 型の別名を定義
	// -----------------------

	using VertexArrayObjectPtr = std::shared_ptr<VertexArrayObject>;

	/// <summary>
	/// <para> 頂点配列オブジェクト(VAO) </para>
	/// <para> 頂点バッファに格納されている頂点データについて、 </para>
	/// <para> 各要素がどのように配置されているかを記述するオブジェクト </para>
	/// </summary>
	class VertexArrayObject
	{
	public: // ---------------- static関数 ------------------

		/// <summary>
		/// VAOを作成する
		/// </summary>
		/// <returns> 作成したVAOへのポインタ </returns>
		static VertexArrayObjectPtr Create()
		{
			return std::make_shared<VertexArrayObject>();
		}

	public: // -------- コンストラクタ・デストラクタ --------

		/// <summary>
		/// VAOを作成するコンストラクタ
		/// </summary>
		VertexArrayObject() { glCreateVertexArrays(1, &id); }

		/// <summary>
		/// VAOを削除するデストラクタ
		/// </summary>
		~VertexArrayObject() { glDeleteVertexArrays(1, &id); }

	public: // ----------------- 禁止事項 -------------------

		// コピーコンストラクタの禁止
		VertexArrayObject(const VertexArrayObject&) = delete;

		// 代入の禁止
		VertexArrayObject& operator=(const VertexArrayObject&) = delete;

	public: // ---------------- 情報の取得 ------------------

		// 管理番号を取得(GLuint型にキャスト)
		operator GLuint() const { return id; }

		/// <summary>
		/// 頂点属性を設定
		/// </summary>
		/// <param name="index"> 設定する頂点属性の番号(0～15) </param>
		/// <param name="size"> 要素のデータ数(float = 1, vec2 = 2, vec3 = 3) </param>
		/// <param name="stride"> 次の頂点データにある同一要素までの間隔(バイト数) </param>
		/// <param name="offset"> 頂点データ内における要素の位置(バイト数) </param>
		void SetAttribute(GLuint index, GLuint size, size_t stride, size_t offset)
		{
			/* void glVertexAttribPointer(頂点属性配列のインデックス, データの要素数,
			データの型, 正規化の有無, 次のデータまでのバイト数, 最初のデータの位置)

			バインドしたバッファに入ってるデータの情報をGPUに教える
			GPUに教えたデータは、VertexPullerが取り出しVertexShaderに渡す
			(VertexShaderのイン変数)

			GPUは16要素の頂点属性配列を扱うことができ、
			glEnableVertexAttribArrayで必要な要素を有効化して使う */

			// 頂点属性の有効化
			glEnableVertexAttribArray(index);

			// 頂点属性を設定
			glVertexAttribPointer(index, size, GL_FLOAT, GL_FALSE,
				static_cast<GLsizei>(stride), reinterpret_cast<void*>(offset));
		}

	private: // ---------------- VAOの情報 ------------------

		GLuint id = 0; // オブジェクト管理番号
	};

} // namespace PokarinEngine

#endif // !VERTEXARRAYOBJECT_H_INCLUDED
