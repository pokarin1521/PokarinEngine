/**
* @file MeshBuffer.h
*/
#ifndef POKARINENGINE_MESHBUFFER_H_INCLUDED
#define POKARINENGINE_MESHBUFFER_H_INCLUDED

#include "StaticMesh.h"
#include "Vertex.h"

#include "../Debug.h"
#include "../TextureGetter.h"

#include "../BufferObject.h"
#include "../VertexArrayObject.h"

namespace PokarinEngine
{
	/// <summary>
	/// メッシュバッファ(描画パラメータや頂点データなど)
	/// </summary>
	class MeshBuffer
	{
	public: // -------------------- コンストラクタ・デストラクタ --------------------

		/// <summary>
		/// メッシュバッファを作成するコンストラクタ
		/// </summary>
		/// <param name="[in] bufferSize"> バッファサイズ(バイト数) </param>
		MeshBuffer(size_t bufferSize);

		// デフォルトデストラクタ
		~MeshBuffer() = default;

	public: // ------------------------------ 禁止事項 ------------------------------

		/* コピーの必要がないので禁止 */

		// コピーコンストラクタの禁止
		MeshBuffer(const MeshBuffer&) = delete;

		// 代入の禁止
		MeshBuffer& operator=(const MeshBuffer&) = delete;

	public: // ----------------------------- 情報の取得 -----------------------------

		/// <summary>
		/// スタティックメッシュを取得する
		/// </summary>
		/// <param name="[in] fileName"> ファイル名 </param>
		/// <returns> ファイル名が一致するスタティックメッシュ </returns>
		StaticMeshPtr GetStaticMesh(const std::string& fileName);

		/// <summary>
		/// VAOの取得
		/// </summary>
		/// <returns> VAO </returns>
		VertexArrayObjectPtr GetVAO() const { return vao; }

	public: // -------------------------- ファイル読み込み --------------------------

		/// <summary>
		/// OBJファイルを読み込む
		/// </summary>
		/// <param name="[in] fileName"> OBJファイル名 </param>
		/// <returns> filenameから作成したメッシュ </returns>
		StaticMeshPtr LoadOBJ(const std::string& fileName);

	private: // -------------------------- 頂点データの管理 -------------------------

		/// <summary>
		/// 頂点データを追加する
		/// </summary>
		/// <param name="[in] vertices"> 頂点データ </param>
		/// <param name="[in] vertexBytes"> 頂点データのバイト数 </param>
		/// <param name="[in] indices"> インデックスデータ </param>
		/// <param name="[in] indexBytes"> インデックスデータのバイト数 </param>
		/// <param name="[in] mode"> プリミティブの種類(デフォルトで三角形) </param>
		/// <param name="[out] indexOffset"> インデックスデータの位置(バイト数で指定) </param>
		/// <param name="[out] baseVertex"> インデックス0となる頂点配列の要素番号 </param>
		void AddVertexData(const Vertex* vertices, size_t vertexBytes,
			const uint16_t* indices, size_t indexBytes,
			const void* indexOffset, GLint& baseVertex);

	private: // ---------------------- メッシュバッファの情報 -----------------------

		// スタティックメッシュ管理用配列
		// <ファイル名, スタティックメッシュのポインタ>
		std::unordered_map<std::string, StaticMeshPtr> staticMeshList;

		// 頂点属性配列
		VertexArrayObjectPtr vao;

		// 頂点データおよびインデックスデータ用のバッファ
		BufferObjectPtr buffer;

		// バッファの使用済み容量(バイト)
		size_t usedBytes = 0;
	};
}

#endif // !POKARINENGINE_MESHBUFFER_H_INCLUDED
