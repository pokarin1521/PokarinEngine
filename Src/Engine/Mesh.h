/**
* @file Mesh.h
*/
#ifndef MESH_H_INCLUDED
#define MESH_H_INCLUDED

#include "Texture.h"

#include "VecMath.h"
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
		Vec3 position; // ���_���W
		Vec2 texcoord; // �e�N�X�`�����W
		Vec3 normal;   // �@���x�N�g��
	};

	/// <summary>
	/// �`��p�����[�^
	/// </summary>
	struct DrawParams
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
		std::string filename; // �t�@�C����
		std::vector<DrawParams> drawParamsList; // �`��p�����[�^�z��
		MaterialList materials; // ���L�}�e���A���z��
	};

	/// <summary>
	/// ���b�V����`�悷��
	/// </summary>
	/// <param name="mesh"> �`�悷��X�^�e�B�b�N���b�V�� </param>
	/// <param name="program"> �g�p����V�F�[�_�v���O�����̊Ǘ��ԍ� </param>
	/// <param name="materialss"> �g�p����}�e���A���z�� </param>
	void Draw(const StaticMesh& mesh, GLuint program, const MaterialList& materials);

	/// <summary>
	/// ���L�}�e���A���z����R�s�[
	/// </summary>
	/// <param name="original"> �R�s�[���}�e���A���z��̎����� </param>
	/// <returns> ���L�}�e���A���z��̃R�s�[ </returns>
	MaterialList CloneMaterialList(const StaticMeshPtr& original);

	/// <summary>
	/// �����Ă���@����₤
	/// </summary>
	/// <param name="vertices"> ���_�z�� </param>
	/// <param name="vertexCount"> ���_�z��̗v�f�� </param>
	/// <param name="indices"> �C���f�b�N�X�z�� </param>
	/// <param name="indexCount"> �C���f�b�N�X�z��̗v�f�� </param>
	void FillMissingNormals(
		Vertex* vertices, size_t vertexCount,
		const uint16_t* indices, size_t indexCount);

	/// <summary>
	/// ���_�f�[�^���Ǘ�����N���X
	/// </summary>
	class MeshBuffer
	{
	public: // ------------------------ ���b�V���o�b�t�@�쐬 ------------------------

		/// <summary>
		/// ���b�V���o�b�t�@���쐬����
		/// </summary>
		/// <param name="bufferSize"> �i�[�ł��钸�_�f�[�^�̃T�C�Y(�o�C�g��) </param>
		/// <returns> �쐬�������b�V���o�b�t�@�ւ̃|�C���^ </returns>
		static MeshBufferPtr Create(size_t bufferSize)
		{
			return std::make_shared<MeshBuffer>(bufferSize);
		}

	public: // -------------------- �R���X�g���N�^�E�f�X�g���N�^ --------------------

		/// <summary>
		/// ���b�V���o�b�t�@���쐬����R���X�g���N�^
		/// </summary>
		/// <param name="bufferSize"> �o�b�t�@�T�C�Y(�o�C�g��) </param>
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
		/// <param name="callback"> �ݒ肷��R�[���o�b�N�I�u�W�F�N�g </param>
		void SetTextureCallback(const TextureCallback& callback)
		{
			textureCallback = callback;
		}

	public: // --------------------------- �t�@�C���ǂݍ��� -------------------------

		/// <summary>
		/// OBJ�t�@�C����ǂݍ���
		/// </summary>
		/// <param name="filename"> OBJ�t�@�C���� </param>
		/// <returns> filename����쐬�������b�V�� </returns>
		StaticMeshPtr LoadOBJ(const char* filename);

	public: // --------------------------- ���_�f�[�^�̊Ǘ� -------------------------

		/// <summary>
		/// ���_�f�[�^�̒ǉ�
		/// </summary>
		/// <param name="vertices"> ���_�f�[�^ </param>
		/// <param name="vertexBytes"> ���_�f�[�^�̃o�C�g�� </param>
		/// <param name="indices"> �C���f�b�N�X�f�[�^ </param>
		/// <param name="indexBytes"> �C���f�b�N�X�f�[�^�̃o�C�g�� </param>
		/// <param name="mode"> �v���~�e�B�u�̎��(�f�t�H���g�ŎO�p�`) </param>
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
		/// <param name="filename"> �t�@�C���� </param>
		/// <returns> �t�@�C��������v����X�^�e�B�b�N���b�V�� </returns>
		StaticMeshPtr GetStaticMesh(const char* filename);

		/// <summary>
		/// �`��p�����[�^���擾����
		/// </summary>
		/// <param name="index"> �擾����`��p�����[�^�̗v�f�ԍ� </param>
		/// <returns> �w�肵���ԍ��̕`��p�����[�^ </returns>
		const DrawParams& GetDrawParams(size_t index) const
		{
			return drawParamsList[index];
		}

		/// <summary>
		/// �`��p�����[�^�̐����擾
		/// </summary>
		/// <returns> �`��p�����[�^�z��̗v�f�� </returns>
		size_t GetDrawParamsCount() const { return drawParamsList.size(); }

		/// <summary>
		/// VAO�̎擾
		/// </summary>
		/// <returns> VAO </returns>
		VertexArrayObjectPtr GetVAO() const { return vao; }

	private: // --------------------------- �}�e���A����ǂݍ��� ---------------------------

		/// <summary>
		/// MTL�t�@�C����ǂݍ���
		/// </summary>
		/// <param name="foldername"> OBJ�t�@�C���̂���t�H���_�� </param>
		/// <param name="filename"> MTL�t�@�C���� </param>
		/// <returns> MTL�t�@�C���Ɋ܂܂��}�e���A���z�� </returns>
		std::vector<MaterialPtr> LoadMTL(
			const std::string& foldername, const char* filename);

	private: // ------------------------- ���b�V���o�b�t�@�̏�� ---------------------------

		// �X�^�e�B�b�N���b�V���Ǘ��p�z��
		// <�t�@�C����, �X�^�e�B�b�N���b�V���̃|�C���^>
		std::unordered_map<std::string, StaticMeshPtr> staticMeshList;

		// �`��p�����[�^�z��
		std::vector<DrawParams> drawParamsList;

		// ���_�����z��
		VertexArrayObjectPtr vao;

		// ���_�f�[�^����уC���f�b�N�X�f�[�^
		BufferObjectPtr buffer;

		// �o�b�t�@�̎g�p�ςݗe��(�o�C�g)
		size_t usedBytes = 0;
	};

} // namespace PokarinEngine

#endif // !MESH_H_INCLUDED