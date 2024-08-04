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
	// �O���錾
	// ---------------------

	class StaticMesh;

	// ------------------------------
	// �^�̕ʖ����`
	// ------------------------------

	using  StaticMeshPtr = std::shared_ptr<StaticMesh>;

	/// <summary>
	/// �X�^�e�B�b�N���b�V��
	/// </summary>
	class StaticMesh
	{
		// ���b�V���o�b�t�@�ɏ������J����
		friend class MeshBuffer;

	public: // --------------------- �֎~���� ----------------------

		// �R�s�[�R���X�g���N�^���֎~
		StaticMesh(const StaticMesh&) = delete;

		// ������֎~
		StaticMesh& operator=(const StaticMesh&) = delete;

	public: // -------------------- �t�@�C���� ---------------------

		/// <summary>
		/// �t�@�C�������擾����
		/// </summary>
		/// <returns> �t�@�C���� </returns>
		const std::string& GetFileName()
		{
			return filename;
		}

	public: // ---------------- �`��p�����[�^�z�� -----------------

		/// <summary>
		/// �`��p�����[�^�z����擾����
		/// </summary>
		/// <returns> �`��p�����[�^�z�� </returns>
		const DrawParameterList& GetDrawParameterList()
		{
			return drawParameterList;
		}

	public: // ---------------- ���L�}�e���A���z�� -----------------

		/// <summary>
		/// ���L�}�e���A���z����擾����
		/// </summary>
		/// <returns> ���L�}�e���A���z�� </returns>
		const MaterialList& GetMaterialList()
		{
			return materialList;
		}

		/// <summary>
		/// ���L�}�e���A���z����R�s�[����
		/// </summary>
		/// <returns> ���L�}�e���A���z��̃R�s�[ </returns>
		MaterialList CopyMaterialList()
		{
			// ���L�}�e���A���z��̃R�s�[
			MaterialList copy(materialList.size());

			// ���L�}�e���A���z����R�s�[
			for (int i = 0; i < copy.size(); ++i)
			{
				copy[i] = std::make_shared<Material>(*materialList[i]);
			}

			return copy;
		}

	private: // ----------- �R���X�g���N�^�E�f�X�g���N�^ -----------

		/* ���b�V���o�b�t�@�ō쐬���Ăق����̂ŁAprivate�ɂ��� */

		StaticMesh() = default;
		~StaticMesh() = default;

	private: // ----------------------- ��� -----------------------

		// �t�@�C����
		std::string filename;

		// �`��p�����[�^�z��
		DrawParameterList drawParameterList;

		// ���L�}�e���A���z��
		MaterialList materialList;
	};
}

#endif // !POKARINENGINE_STATICMESH_H_INCLUDED
