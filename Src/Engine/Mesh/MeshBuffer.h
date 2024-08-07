/**
* @file MeshBuffer.h
*/
#ifndef POKARINENGINE_MESHBUFFER_H_INCLUDED
#define POKARINENGINE_MESHBUFFER_H_INCLUDED

#include "StaticMesh.h"
#include "Vertex.h"

#include "../Debug.h"
#include "../TextureGetter.h"

#include "../BufferObject.h"
#include "../VertexArrayObject.h"

namespace PokarinEngine
{
	/// <summary>
	/// ���b�V���o�b�t�@(�`��p�����[�^�Ⓒ�_�f�[�^�Ȃ�)
	/// </summary>
	class MeshBuffer
	{
	public: // -------------------- �R���X�g���N�^�E�f�X�g���N�^ --------------------

		/// <summary>
		/// ���b�V���o�b�t�@���쐬����R���X�g���N�^
		/// </summary>
		/// <param name="[in] bufferSize"> �o�b�t�@�T�C�Y(�o�C�g��) </param>
		MeshBuffer(size_t bufferSize);

		// �f�t�H���g�f�X�g���N�^
		~MeshBuffer() = default;

	public: // ------------------------------ �֎~���� ------------------------------

		/* �R�s�[�̕K�v���Ȃ��̂ŋ֎~ */

		// �R�s�[�R���X�g���N�^�̋֎~
		MeshBuffer(const MeshBuffer&) = delete;

		// ����̋֎~
		MeshBuffer& operator=(const MeshBuffer&) = delete;

	public: // ----------------------------- ���̎擾 -----------------------------

		/// <summary>
		/// �X�^�e�B�b�N���b�V�����擾����
		/// </summary>
		/// <param name="[in] fileName"> �t�@�C���� </param>
		/// <returns> �t�@�C��������v����X�^�e�B�b�N���b�V�� </returns>
		StaticMeshPtr GetStaticMesh(const std::string& fileName);

		/// <summary>
		/// VAO�̎擾
		/// </summary>
		/// <returns> VAO </returns>
		VertexArrayObjectPtr GetVAO() const { return vao; }

	public: // -------------------------- �t�@�C���ǂݍ��� --------------------------

		/// <summary>
		/// OBJ�t�@�C����ǂݍ���
		/// </summary>
		/// <param name="[in] fileName"> OBJ�t�@�C���� </param>
		/// <returns> filename����쐬�������b�V�� </returns>
		StaticMeshPtr LoadOBJ(const std::string& fileName);

	private: // -------------------------- ���_�f�[�^�̊Ǘ� -------------------------

		/// <summary>
		/// ���_�f�[�^��ǉ�����
		/// </summary>
		/// <param name="[in] vertices"> ���_�f�[�^ </param>
		/// <param name="[in] vertexBytes"> ���_�f�[�^�̃o�C�g�� </param>
		/// <param name="[in] indices"> �C���f�b�N�X�f�[�^ </param>
		/// <param name="[in] indexBytes"> �C���f�b�N�X�f�[�^�̃o�C�g�� </param>
		/// <param name="[in] mode"> �v���~�e�B�u�̎��(�f�t�H���g�ŎO�p�`) </param>
		/// <param name="[out] indexOffset"> �C���f�b�N�X�f�[�^�̈ʒu(�o�C�g���Ŏw��) </param>
		/// <param name="[out] baseVertex"> �C���f�b�N�X0�ƂȂ钸�_�z��̗v�f�ԍ� </param>
		void AddVertexData(const Vertex* vertices, size_t vertexBytes,
			const uint16_t* indices, size_t indexBytes,
			const void* indexOffset, GLint& baseVertex);

	private: // ---------------------- ���b�V���o�b�t�@�̏�� -----------------------

		// �X�^�e�B�b�N���b�V���Ǘ��p�z��
		// <�t�@�C����, �X�^�e�B�b�N���b�V���̃|�C���^>
		std::unordered_map<std::string, StaticMeshPtr> staticMeshList;

		// ���_�����z��
		VertexArrayObjectPtr vao;

		// ���_�f�[�^����уC���f�b�N�X�f�[�^�p�̃o�b�t�@
		BufferObjectPtr buffer;

		// �o�b�t�@�̎g�p�ςݗe��(�o�C�g)
		size_t usedBytes = 0;
	};
}

#endif // !POKARINENGINE_MESHBUFFER_H_INCLUDED
