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
	// �O���錾
	// ---------------------

	struct StaticMesh;

	// ------------------------------
	// �^�̕ʖ����`
	// ------------------------------

	using  StaticMeshPtr = std::shared_ptr<StaticMesh>;

	/// <summary>
	/// �X�^�e�B�b�N���b�V��
	/// </summary>
	class StaticMesh
	{
	private: // ----------- �R���X�g���N�^�E�f�X�g���N�^ -----------

		StaticMesh() = default;
		~StaticMesh() = default;

	private: // ----------------------- ��� -----------------------

		// �t�@�C����
		std::string filename;

		// �`��p�����[�^�z��
		std::vector<DrawParameter> drawParamList;

		// ���L�}�e���A���z��
		MaterialList materials;
	};
}

#endif // !POKARINENGINE_STATICMESH_H_INCLUDED
