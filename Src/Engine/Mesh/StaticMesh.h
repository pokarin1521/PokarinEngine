/**
* @file StaticMesh.h
*/
#ifndef POKARINENGINE_STATICMESH_H_INCLUDED
#define POKARINENGINE_STATICMESH_H_INCLUDED

#include "DrawParameter.h"
#include "Material.h"

#include <string>

namespace PokarinEngine
{
	// ---------------------
	// 前方宣言
	// ---------------------

	class StaticMesh;

	// ------------------------------
	// 型の別名を定義
	// ------------------------------

	using  StaticMeshPtr = std::shared_ptr<StaticMesh>;

	/// <summary>
	/// スタティックメッシュ
	/// </summary>
	class StaticMesh
	{
		// メッシュバッファに情報を公開する
		friend class MeshBuffer;

	public: // --------------------- 禁止事項 ----------------------

		// コピーコンストラクタを禁止
		StaticMesh(const StaticMesh&) = delete;

		// 代入を禁止
		StaticMesh& operator=(const StaticMesh&) = delete;

	public: // -------------------- ファイル名 ---------------------

		/// <summary>
		/// ファイル名を取得する
		/// </summary>
		/// <returns> ファイル名 </returns>
		const std::string& GetFileName()
		{
			return filename;
		}

	public: // ---------------- 描画パラメータ配列 -----------------

		/// <summary>
		/// 描画パラメータ配列を取得する
		/// </summary>
		/// <returns> 描画パラメータ配列 </returns>
		const DrawParameterList& GetDrawParameterList()
		{
			return drawParameterList;
		}

	public: // ---------------- 共有マテリアル配列 -----------------

		/// <summary>
		/// 共有マテリアル配列を取得する
		/// </summary>
		/// <returns> 共有マテリアル配列 </returns>
		const MaterialList& GetMaterialList()
		{
			return materialList;
		}

		/// <summary>
		/// 共有マテリアル配列をコピーする
		/// </summary>
		/// <returns> 共有マテリアル配列のコピー </returns>
		MaterialList CopyMaterialList()
		{
			// 共有マテリアル配列のコピー
			MaterialList copy(materialList.size());

			// 共有マテリアル配列をコピー
			for (int i = 0; i < copy.size(); ++i)
			{
				copy[i] = std::make_shared<Material>(*materialList[i]);
			}

			return copy;
		}

	private: // ----------- コンストラクタ・デストラクタ -----------

		/* メッシュバッファで作成してほしいので、privateにする */

		StaticMesh() = default;
		~StaticMesh() = default;

	private: // ----------------------- 情報 -----------------------

		// ファイル名
		std::string filename;

		// 描画パラメータ配列
		DrawParameterList drawParameterList;

		// 共有マテリアル配列
		MaterialList materialList;
	};
}

#endif // !POKARINENGINE_STATICMESH_H_INCLUDED
