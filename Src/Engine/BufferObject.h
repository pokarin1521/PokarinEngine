/**
* @file BufferObject.h
*/
#ifndef POKARINENGINE_BUFFEROBJECT_H_INCLUDED
#define POKARINENGINE_BUFFEROBJECT_H_INCLUDED

#include "glad/glad.h"

#include <memory>

namespace PokarinEngine
{
	// -----------------
	// 前方宣言
	// -----------------

	class BufferObject;

	// -----------------------
	// 型の別名を定義
	// -----------------------

	using BufferObjectPtr = std::shared_ptr <BufferObject>;

	/// <summary>
	/// バッファオブジェクト(BO)
	/// </summary>
	class BufferObject
	{
	public: // ----------- バッファオブジェクト作成 ------------

		/// <summary>
		/// バッファオブジェクトを作成する
		/// </summary>
		/// <param name="[in] size"> バッファオブジェクトのバイト数  </param>
		/// <param name="[in] data"> 
		/// <para> バッファにコピーするデータのアドレス、 </para>
		/// <para> nullptrを指定すると空のバッファが作成される </para>
		/// </param>
		/// <param name="[in] flags"> メモリの使用方法を示すフラグ </param>
		/// <returns> 作成したバッファオブジェクトへのポインタ </returns>
		static BufferObjectPtr Create(GLsizeiptr size,
			const void* data = nullptr, GLbitfield flags = 0)
		{
			return std::make_shared<BufferObject>(size, data, flags);
		}

	public: // --------- コンストラクタ・デストラクタ ----------

		/// <summary>
		/// バッファオブジェクトを作成するコンストラクタ
		/// </summary>
		/// <param name="[in] size"> バッファオブジェクトのバイト数  </param>
		/// <param name="[in] data"> 
		/// <para> バッファにコピーするデータのアドレス、 </para>
		/// <para> nullptrを指定すると空のバッファが作成される </para>
		/// </param>
		/// <param name="[in] flags"> メモリの使用方法を示すフラグ </param>
		BufferObject(GLsizeiptr size, const void* data, GLbitfield flags)
		{
			// バッファサイズを設定
			bufferSize = size;

			// バッファオブジェクトの作成
			glCreateBuffers(1, &id);

			// GPUメモリの確保
			glNamedBufferStorage(id, bufferSize, data, flags);
		}

		/// <summary> 
		/// バッファオブジェクトを削除するデストラクタ
		/// </summary>
		~BufferObject() { glDeleteBuffers(1, &id); }

	public: // ----------------- 禁止事項 -------------------

		/* 削除用デストラクタを複数回呼ばれないように禁止する */

		// コピーコンストラクの禁止
		BufferObject(const BufferObject&) = delete;

		// 代入の禁止
		BufferObject& operator=(const BufferObject&) = delete;

	public: // ---------------- 情報の取得 ------------------

		// 識別番号を取得(GLuint型にキャスト)
		operator GLuint() const { return id; }

		/// <summary>
		/// バッファサイズを取得
		/// </summary>
		/// <returns> バッファサイズ </returns>
		GLsizeiptr GetSize() const { return bufferSize; }

	private: // -------- バッファオブジェクトの情報 ---------

		// オブジェクト識別番号
		GLuint id = 0; 
		
		// バッファサイズ(バイト数)
		GLsizeiptr bufferSize = 0;
	};

} // namespace PokarinEngine

#endif // !POKARINENGINE_BUFFEROBJECT_H_INCLUDED
