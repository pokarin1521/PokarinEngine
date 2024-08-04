/**
* @file Mesh.h
*/
#ifndef POKARINENGINE_MESH_H_INCLUDED
#define POKARINENGINE_MESH_H_INCLUDED

#include "StaticMesh.h"

#include "../Math/Vector.h"
#include "../Color.h"

#include <string>
#include <vector>
#include <memory>
#include <unordered_map>
#include <functional>

namespace PokarinEngine
{
	// -----------------------
	// 前方宣言
	// -----------------------

	class MeshBuffer;

	/// <summary>
	/// メッシュ管理用クラス
	/// </summary>
	class Mesh
	{	
	public: // ------------------------- 禁止事項 -------------------------

		Mesh(const Mesh&) = delete;
		Mesh& operator=(const Mesh&) = delete;

	public: // -------------------------- 初期化 --------------------------

		/// <summary>
		/// 初期化
		/// </summary>
		static void Initialize();

	public: // ---------------- スタティックメッシュの取得 ----------------

		/// <summary>
		/// スタティックメッシュを取得する
		/// </summary>
		/// <param name="[in] fileName"> スタティックメッシュのファイル名 </param>
		/// <returns> スタティックメッシュ </returns>
		static StaticMeshPtr GetStaticMesh(const std::string& fileName);

	public: // --------------------------- 描画 ---------------------------

		/// <summary>
		/// スタティックメッシュを描画する
		/// </summary>
		/// <param name="[in] mesh"> 描画するスタティックメッシュ </param>
		/// <param name="[in] program"> 使用するシェーダプログラムの識別番号 </param>
		/// <param name="[in] materialList"> 使用するマテリアル配列 </param>
		static void Draw(const StaticMeshPtr& mesh, GLuint program, const MaterialList& materialList);

	private: // -------------- コンストラクタ・デストラクタ ---------------

		/* static関数でアクセスしてほしいので、作成できないようにする */

		Mesh() = default;
		~Mesh() = default;

	private: // --------------------- メッシュバッファ --------------------

		// メッシュバッファ
		inline static std::shared_ptr<MeshBuffer> meshBuffer = nullptr;
	};

} // namespace PokarinEngine

#endif // !POKARINENGINE_MESH_H_INCLUDED