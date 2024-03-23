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
	// �O���錾
	// -----------------

	class MeshBuffer;

	struct Material;
	struct StaticMesh;

	// -----------------------------------
	// �^�̕ʖ����`
	// -----------------------------------

	// ------------------ ���b�V���o�b�t�@ ------------------

	using MeshBufferPtr = std::shared_ptr<MeshBuffer>;

	// --------------------- �}�e���A�� ---------------------

	using MaterialPtr = std::shared_ptr<Material>;

	using MaterialList = std::vector<MaterialPtr>;

	// ---------------- �X�^�e�B�b�N���b�V�� ----------------

	using  StaticMeshPtr = std::shared_ptr<StaticMesh>;
}

#endif // !USINGMESH_H_INCLUDED
