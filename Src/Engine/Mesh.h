/**
* @file Mesh.h
*/
#ifndef MESH_H_INCLUDED
#define MESH_H_INCLUDED

#include "Texture.h"

#include "Math/Vector.h"
#include "Color.h"

#include "BufferObject.h"
#include "VertexArrayObject.h"

#include "UsingNames/UsingMesh.h"

#include <string>
#include <vector>
#include <memory>
#include <unordered_map>
#include <functional>

namespace PokarinEngine
{
	// -----------------
	// 前方宣言
	// -----------------

	class Engine;

	// -----------------------------------
	// 型の別名を定義
	// -----------------------------------

	// テクスチャ制作コールバック型
	// (引数に指定されたファイル名からテクスチャを作成する型)
	using TextureCallback = std::function<TexturePtr(const char*)>;

	/// <summary>
	/// 頂点データ
	/// </summary>
	struct Vertex
	{
		Vector3 position; // 頂点座標
		Vector2 texcoord; // テクスチャ座標
		Vector3 normal;   // 法線ベクトル
	};

	/// <summary>
	/// 描画パラメータ
	/// </summary>
	struct DrawParameter
	{
		GLenum mode = GL_TRIANGLES; // プリミティブの種類
		GLsizei count = 0;			// 描画するインデックス数
		const void* indices = 0;	// 描画開始インデックスのバイトオフセット
		GLint baseVertex = 0;		// インデックス0となる頂点配列の要素番号
		int materialNo = -1;		// マテリアルインデックス
	};

	/// <summary>
	/// マテリアル
	/// </summary>
	struct Material
	{
		std::string name = "<Default>";	  // マテリアル名
		Color baseColor = { 1, 1, 1, 1 }; // 基本色 + 透明度
		Color emission = { 0, 0, 0, 0 };  // 発光色(深度値は使わない)
		TexturePtr texBaseColor;		  // 基本色テクスチャ
		TexturePtr texEmission;			  // 発光色テクスチャ
	};

	/// <summary>
	/// スタティックメッシュ
	/// </summary>
	struct StaticMesh
	{
		// ファイル名
		std::string filename;

		// 描画パラメータ配列
		std::vector<DrawParameter> drawParamList;

		// 共有マテリアル配列
		MaterialList materials;
	};

	/// <summary>
	/// メッシュを描画する
	/// </summary>
	/// <param name="[in] mesh"> 描画するスタティックメッシュ </param>
	/// <param name="[in] program"> 使用するシェーダプログラムの管理番号 </param>
	/// <param name="[in] materialss"> 使用するマテリアル配列 </param>
	void Draw(const StaticMeshPtr& mesh, GLuint program, const MaterialList& materials);

	/// <summary>
	/// 共有マテリアル配列をコピー
	/// </summary>
	/// <param name="[in] original"> コピー元マテリアル配列の持ち主 </param>
	/// <returns> 共有マテリアル配列のコピー </returns>
	MaterialList CloneMaterialList(const StaticMeshPtr& original);

	/// <summary>
	/// 欠けている法線を補う
	/// </summary>
	/// <param name="[in,out] vertices"> 頂点配列 </param>
	/// <param name="[in] vertexCount"> 頂点配列の要素数 </param>
	/// <param name="[in] indices"> インデックス配列 </param>
	/// <param name="[in] indexCount"> インデックス配列の要素数 </param>
	void FillMissingNormals(
		Vertex* vertices, size_t vertexCount,
		const uint16_t* indices, size_t indexCount);

	/// <summary>
	/// メッシュバッファ(描画パラメータや頂点データなど)
	/// </summary>
	class MeshBuffer
	{
	public: // ------------------------ メッシュバッファ作成 ------------------------

		/// <summary>
		/// メッシュバッファを作成する
		/// </summary>
		/// <param name="[in] bufferSize"> 格納できる頂点データのサイズ(バイト数) </param>
		/// <returns> 作成したメッシュバッファへのポインタ </returns>
		static MeshBufferPtr Create(size_t bufferSize)
		{
			return std::make_shared<MeshBuffer>(bufferSize);
		}

	public: // -------------------- コンストラクタ・デストラクタ --------------------

		/// <summary>
		/// メッシュバッファを作成するコンストラクタ
		/// </summary>
		/// <param name="[in] bufferSize"> バッファサイズ(バイト数) </param>
		MeshBuffer(size_t bufferSize);

		// デフォルトデストラクタ
		~MeshBuffer() = default;

	public: // ------------------------------ 禁止事項 ------------------------------

		// コピーコンストラクタの禁止
		MeshBuffer(const MeshBuffer&) = delete;

		// 代入の禁止
		MeshBuffer& operator=(const MeshBuffer&) = delete;

	public: // ---------------------- テクスチャ作成コールバック --------------------

		// テクスチャ作成コールバック
		TextureCallback textureCallback;

		/// <summary>
		/// テクスチャ作成コールバックを設定する
		/// </summary>
		/// <param name="[in] callback"> 設定するコールバックオブジェクト </param>
		void SetTextureCallback(const TextureCallback& callback)
		{
			textureCallback = callback;
		}

	public: // --------------------------- ファイル読み込み -------------------------

		/// <summary>
		/// OBJファイルを読み込む
		/// </summary>
		/// <param name="[in] fileName"> OBJファイル名 </param>
		/// <returns> filenameから作成したメッシュ </returns>
		StaticMeshPtr LoadOBJ(const char* fileName);

	public: // --------------------------- 頂点データの管理 -------------------------

		/// <summary>
		/// 頂点データの追加
		/// </summary>
		/// <param name="[in] vertices"> 頂点データ </param>
		/// <param name="[in] vertexBytes"> 頂点データのバイト数 </param>
		/// <param name="[in] indices"> インデックスデータ </param>
		/// <param name="[in] indexBytes"> インデックスデータのバイト数 </param>
		/// <param name="[in] mode"> プリミティブの種類(デフォルトで三角形) </param>
		void AddVertexData(const Vertex* vertices, size_t vertexBytes,
			const uint16_t* indices, size_t indexBytes, GLenum mode = GL_TRIANGLES);

		/// <summary>
		/// 全ての頂点データを削除
		/// </summary>
		void Clear();

	public: // ------------------------------- 情報の取得 ---------------------------

		/// <summary>
		/// スタティックメッシュを取得する
		/// </summary>
		/// <param name="[in] fileName"> ファイル名 </param>
		/// <returns> ファイル名が一致するスタティックメッシュ </returns>
		StaticMeshPtr GetStaticMesh(const char* fileName);

		/// <summary>
		/// 描画パラメータを取得する
		/// </summary>
		/// <param name="[in] index"> 取得する描画パラメータの要素番号 </param>
		/// <returns> 指定した番号の描画パラメータ </returns>
		const DrawParameter& GetDrawParams(size_t index) const
		{
			return drawParamList[index];
		}

		/// <summary>
		/// 描画パラメータの数を取得
		/// </summary>
		/// <returns> 描画パラメータ配列の要素数 </returns>
		size_t GetDrawParamsCount() const { return drawParamList.size(); }

		/// <summary>
		/// VAOの取得
		/// </summary>
		/// <returns> VAO </returns>
		VertexArrayObjectPtr GetVAO() const { return vao; }

	private: // --------------------------- マテリアルを読み込む ---------------------------

		/// <summary>
		/// MTLファイルを読み込む
		/// </summary>
		/// <param name="[in] folderName"> OBJファイルのあるフォルダ名 </param>
		/// <param name="[in] fileName"> MTLファイル名 </param>
		/// <returns> MTLファイルに含まれるマテリアル配列 </returns>
		std::vector<MaterialPtr> LoadMTL(
			const std::string& folderName, const char* fileName);

	private: // ------------------------- メッシュバッファの情報 ---------------------------

		// スタティックメッシュ管理用配列
		// <ファイル名, スタティックメッシュのポインタ>
		std::unordered_map<std::string, StaticMeshPtr> staticMeshList;

		// 描画パラメータ配列
		std::vector<DrawParameter> drawParamList;

		// 頂点属性配列
		VertexArrayObjectPtr vao;

		// 頂点データおよびインデックスデータ用のバッファ
		BufferObjectPtr buffer;

		// バッファの使用済み容量(バイト)
		size_t usedBytes = 0;
	};

} // namespace PokarinEngine

#endif // !MESH_H_INCLUDED