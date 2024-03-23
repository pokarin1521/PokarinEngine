/**
* @file UsingMesh.h
*/
#ifndef USINGMESH_H_INCLUDED
#define USINGMESH_H_INCLUDED

#include <memory>
#include <vector>

namespace PokarinEngine
{
	// -----------------
	// 前方宣言
	// -----------------

	class MeshBuffer;

	struct Material;
	struct StaticMesh;

	// -----------------------------------
	// 型の別名を定義
	// -----------------------------------

	// ------------------ メッシュバッファ ------------------

	using MeshBufferPtr = std::shared_ptr<MeshBuffer>;

	// --------------------- マテリアル ---------------------

	using MaterialPtr = std::shared_ptr<Material>;

	using MaterialList = std::vector<MaterialPtr>;

	// ---------------- スタティックメッシュ ----------------

	using  StaticMeshPtr = std::shared_ptr<StaticMesh>;
}

#endif // !USINGMESH_H_INCLUDED
