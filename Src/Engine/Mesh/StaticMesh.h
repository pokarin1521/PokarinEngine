/**
* @file StaticMesh.h
*/
#ifndef POKARINENGINE_STATICMESH_H_INCLUDED
#define POKARINENGINE_STATICMESH_H_INCLUDED

#include "DrawParameter.h"
#include "Material.h"

#include <string>
#include <vector>

namespace PokarinEngine
{
	// ---------------------
	// 前方宣言
	// ---------------------

	struct StaticMesh;

	// ------------------------------
	// 型の別名を定義
	// ------------------------------

	using  StaticMeshPtr = std::shared_ptr<StaticMesh>;

	/// <summary>
	/// スタティックメッシュ
	/// </summary>
	class StaticMesh
	{
	private: // ----------- コンストラクタ・デストラクタ -----------

		StaticMesh() = default;
		~StaticMesh() = default;

	private: // ----------------------- 情報 -----------------------

		// ファイル名
		std::string filename;

		// 描画パラメータ配列
		std::vector<DrawParameter> drawParamList;

		// 共有マテリアル配列
		MaterialList materials;
	};
}

#endif // !POKARINENGINE_STATICMESH_H_INCLUDED
