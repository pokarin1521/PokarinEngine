/**
* @file SkySphere.cpp
*/
#include "SkySphere.h"

#include "Mesh/Mesh.h"

#include "Configs/MeshConfig.h"

namespace PokarinEngine
{
	/// <summary>
	/// �X�J�C�X�t�B�A�Ǘ��p
	/// </summary>
	namespace SkySphere
	{
		// -------------------------
		// �ϐ�
		// -------------------------

		// �X�J�C�X�t�B�A�p���b�V��
		StaticMeshPtr skySphere;

		// -------------------------
		// �֐�
		// -------------------------

		/// <summary>
		/// �X�J�C�X�t�B�A�p���b�V����ǂݍ���
		/// </summary>
		/// <returns> �X�J�C�X�t�B�A�p���b�V�� </returns>
		StaticMeshPtr Load()
		{
			Mesh::GetStaticMesh(StaticMeshFile::skySphere);
		}

		/// <summary>
		/// �X�J�C�X�t�B�A��`�悷��
		/// </summary>
		void Draw(const Camera& camera)
		{
			
		}
	}
}