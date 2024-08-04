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
	// �O���錾
	// -----------------------

	class MeshBuffer;

	/// <summary>
	/// ���b�V���Ǘ��p�N���X
	/// </summary>
	class Mesh
	{	
	public: // ------------------------- �֎~���� -------------------------

		Mesh(const Mesh&) = delete;
		Mesh& operator=(const Mesh&) = delete;

	public: // -------------------------- ������ --------------------------

		/// <summary>
		/// ������
		/// </summary>
		static void Initialize();

	public: // ---------------- �X�^�e�B�b�N���b�V���̎擾 ----------------

		/// <summary>
		/// �X�^�e�B�b�N���b�V�����擾����
		/// </summary>
		/// <param name="[in] fileName"> �X�^�e�B�b�N���b�V���̃t�@�C���� </param>
		/// <returns> �X�^�e�B�b�N���b�V�� </returns>
		static StaticMeshPtr GetStaticMesh(const std::string& fileName);

	public: // --------------------------- �`�� ---------------------------

		/// <summary>
		/// �X�^�e�B�b�N���b�V����`�悷��
		/// </summary>
		/// <param name="[in] mesh"> �`�悷��X�^�e�B�b�N���b�V�� </param>
		/// <param name="[in] program"> �g�p����V�F�[�_�v���O�����̎��ʔԍ� </param>
		/// <param name="[in] materialList"> �g�p����}�e���A���z�� </param>
		static void Draw(const StaticMeshPtr& mesh, GLuint program, const MaterialList& materialList);

	private: // -------------- �R���X�g���N�^�E�f�X�g���N�^ ---------------

		/* static�֐��ŃA�N�Z�X���Ăق����̂ŁA�쐬�ł��Ȃ��悤�ɂ��� */

		Mesh() = default;
		~Mesh() = default;

	private: // --------------------- ���b�V���o�b�t�@ --------------------

		// ���b�V���o�b�t�@
		inline static std::shared_ptr<MeshBuffer> meshBuffer = nullptr;
	};

} // namespace PokarinEngine

#endif // !POKARINENGINE_MESH_H_INCLUDED