/**
* @file Mesh.h
*/
#ifndef MESH_H_INCLUDED
#define MESH_H_INCLUDED

#include "Texture.h"

#include "Math/Vector.h"
#include "Color.h"

#include "BufferObject.h"
#include "VertexArrayObject.h"

#include "UsingNames/UsingMesh.h"

#include <string>
#include <vector>
#include <memory>
#include <unordered_map>
#include <functional>

namespace PokarinEngine
{
	// -----------------
	// �O���錾
	// -----------------

	class Engine;

	// -----------------------------------
	// �^�̕ʖ����`
	// -----------------------------------

	// �e�N�X�`������R�[���o�b�N�^
	// (�����Ɏw�肳�ꂽ�t�@�C��������e�N�X�`�����쐬����^)
	using TextureCallback = std::function<TexturePtr(const char*)>;

	/// <summary>
	/// ���_�f�[�^
	/// </summary>
	struct Vertex
	{
		Vector3 position; // ���_���W
		Vector2 texcoord; // �e�N�X�`�����W
		Vector3 normal;   // �@���x�N�g��
	};

	/// <summary>
	/// �`��p�����[�^
	/// </summary>
	struct DrawParameter
	{
		GLenum mode = GL_TRIANGLES; // �v���~�e�B�u�̎��
		GLsizei count = 0;			// �`�悷��C���f�b�N�X��
		const void* indices = 0;	// �`��J�n�C���f�b�N�X�̃o�C�g�I�t�Z�b�g
		GLint baseVertex = 0;		// �C���f�b�N�X0�ƂȂ钸�_�z��̗v�f�ԍ�
		int materialNo = -1;		// �}�e���A���C���f�b�N�X
	};

	/// <summary>
	/// �}�e���A��
	/// </summary>
	struct Material
	{
		std::string name = "<Default>";	  // �}�e���A����
		Color baseColor = { 1, 1, 1, 1 }; // ��{�F + �����x
		Color emission = { 0, 0, 0, 0 };  // �����F(�[�x�l�͎g��Ȃ�)
		TexturePtr texBaseColor;		  // ��{�F�e�N�X�`��
		TexturePtr texEmission;			  // �����F�e�N�X�`��
	};

	/// <summary>
	/// �X�^�e�B�b�N���b�V��
	/// </summary>
	struct StaticMesh
	{
		// �t�@�C����
		std::string filename;

		// �`��p�����[�^�z��
		std::vector<DrawParameter> drawParamList;

		// ���L�}�e���A���z��
		MaterialList materials;
	};

	/// <summary>
	/// ���b�V����`�悷��
	/// </summary>
	/// <param name="[in] mesh"> �`�悷��X�^�e�B�b�N���b�V�� </param>
	/// <param name="[in] program"> �g�p����V�F�[�_�v���O�����̊Ǘ��ԍ� </param>
	/// <param name="[in] materialss"> �g�p����}�e���A���z�� </param>
	void Draw(const StaticMeshPtr& mesh, GLuint program, const MaterialList& materials);

	/// <summary>
	/// ���L�}�e���A���z����R�s�[
	/// </summary>
	/// <param name="[in] original"> �R�s�[���}�e���A���z��̎����� </param>
	/// <returns> ���L�}�e���A���z��̃R�s�[ </returns>
	MaterialList CloneMaterialList(const StaticMeshPtr& original);

	/// <summary>
	/// �����Ă���@����₤
	/// </summary>
	/// <param name="[in,out] vertices"> ���_�z�� </param>
	/// <param name="[in] vertexCount"> ���_�z��̗v�f�� </param>
	/// <param name="[in] indices"> �C���f�b�N�X�z�� </param>
	/// <param name="[in] indexCount"> �C���f�b�N�X�z��̗v�f�� </param>
	void FillMissingNormals(
		Vertex* vertices, size_t vertexCount,
		const uint16_t* indices, size_t indexCount);

	/// <summary>
	/// ���b�V���o�b�t�@(�`��p�����[�^�Ⓒ�_�f�[�^�Ȃ�)
	/// </summary>
	class MeshBuffer
	{
	public: // ------------------------ ���b�V���o�b�t�@�쐬 ------------------------

		/// <summary>
		/// ���b�V���o�b�t�@���쐬����
		/// </summary>
		/// <param name="[in] bufferSize"> �i�[�ł��钸�_�f�[�^�̃T�C�Y(�o�C�g��) </param>
		/// <returns> �쐬�������b�V���o�b�t�@�ւ̃|�C���^ </returns>
		static MeshBufferPtr Create(size_t bufferSize)
		{
			return std::make_shared<MeshBuffer>(bufferSize);
		}

	public: // -------------------- �R���X�g���N�^�E�f�X�g���N�^ --------------------

		/// <summary>
		/// ���b�V���o�b�t�@���쐬����R���X�g���N�^
		/// </summary>
		/// <param name="[in] bufferSize"> �o�b�t�@�T�C�Y(�o�C�g��) </param>
		MeshBuffer(size_t bufferSize);

		// �f�t�H���g�f�X�g���N�^
		~MeshBuffer() = default;

	public: // ------------------------------ �֎~���� ------------------------------

		// �R�s�[�R���X�g���N�^�̋֎~
		MeshBuffer(const MeshBuffer&) = delete;

		// ����̋֎~
		MeshBuffer& operator=(const MeshBuffer&) = delete;

	public: // ---------------------- �e�N�X�`���쐬�R�[���o�b�N --------------------

		// �e�N�X�`���쐬�R�[���o�b�N
		TextureCallback textureCallback;

		/// <summary>
		/// �e�N�X�`���쐬�R�[���o�b�N��ݒ肷��
		/// </summary>
		/// <param name="[in] callback"> �ݒ肷��R�[���o�b�N�I�u�W�F�N�g </param>
		void SetTextureCallback(const TextureCallback& callback)
		{
			textureCallback = callback;
		}

	public: // --------------------------- �t�@�C���ǂݍ��� -------------------------

		/// <summary>
		/// OBJ�t�@�C����ǂݍ���
		/// </summary>
		/// <param name="[in] fileName"> OBJ�t�@�C���� </param>
		/// <returns> filename����쐬�������b�V�� </returns>
		StaticMeshPtr LoadOBJ(const char* fileName);

	public: // --------------------------- ���_�f�[�^�̊Ǘ� -------------------------

		/// <summary>
		/// ���_�f�[�^�̒ǉ�
		/// </summary>
		/// <param name="[in] vertices"> ���_�f�[�^ </param>
		/// <param name="[in] vertexBytes"> ���_�f�[�^�̃o�C�g�� </param>
		/// <param name="[in] indices"> �C���f�b�N�X�f�[�^ </param>
		/// <param name="[in] indexBytes"> �C���f�b�N�X�f�[�^�̃o�C�g�� </param>
		/// <param name="[in] mode"> �v���~�e�B�u�̎��(�f�t�H���g�ŎO�p�`) </param>
		void AddVertexData(const Vertex* vertices, size_t vertexBytes,
			const uint16_t* indices, size_t indexBytes, GLenum mode = GL_TRIANGLES);

		/// <summary>
		/// �S�Ă̒��_�f�[�^���폜
		/// </summary>
		void Clear();

	public: // ------------------------------- ���̎擾 ---------------------------

		/// <summary>
		/// �X�^�e�B�b�N���b�V�����擾����
		/// </summary>
		/// <param name="[in] fileName"> �t�@�C���� </param>
		/// <returns> �t�@�C��������v����X�^�e�B�b�N���b�V�� </returns>
		StaticMeshPtr GetStaticMesh(const char* fileName);

		/// <summary>
		/// �`��p�����[�^���擾����
		/// </summary>
		/// <param name="[in] index"> �擾����`��p�����[�^�̗v�f�ԍ� </param>
		/// <returns> �w�肵���ԍ��̕`��p�����[�^ </returns>
		const DrawParameter& GetDrawParams(size_t index) const
		{
			return drawParamList[index];
		}

		/// <summary>
		/// �`��p�����[�^�̐����擾
		/// </summary>
		/// <returns> �`��p�����[�^�z��̗v�f�� </returns>
		size_t GetDrawParamsCount() const { return drawParamList.size(); }

		/// <summary>
		/// VAO�̎擾
		/// </summary>
		/// <returns> VAO </returns>
		VertexArrayObjectPtr GetVAO() const { return vao; }

	private: // --------------------------- �}�e���A����ǂݍ��� ---------------------------

		/// <summary>
		/// MTL�t�@�C����ǂݍ���
		/// </summary>
		/// <param name="[in] folderName"> OBJ�t�@�C���̂���t�H���_�� </param>
		/// <param name="[in] fileName"> MTL�t�@�C���� </param>
		/// <returns> MTL�t�@�C���Ɋ܂܂��}�e���A���z�� </returns>
		std::vector<MaterialPtr> LoadMTL(
			const std::string& folderName, const char* fileName);

	private: // ------------------------- ���b�V���o�b�t�@�̏�� ---------------------------

		// �X�^�e�B�b�N���b�V���Ǘ��p�z��
		// <�t�@�C����, �X�^�e�B�b�N���b�V���̃|�C���^>
		std::unordered_map<std::string, StaticMeshPtr> staticMeshList;

		// �`��p�����[�^�z��
		std::vector<DrawParameter> drawParamList;

		// ���_�����z��
		VertexArrayObjectPtr vao;

		// ���_�f�[�^����уC���f�b�N�X�f�[�^�p�̃o�b�t�@
		BufferObjectPtr buffer;

		// �o�b�t�@�̎g�p�ςݗe��(�o�C�g)
		size_t usedBytes = 0;
	};

} // namespace PokarinEngine

#endif // !MESH_H_INCLUDED