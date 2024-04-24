/**
/**
* @file Engine.h
*/
#ifndef ENGINE_H_INCLUDED
#define ENGINE_H_INCLUDED

#include "glad/glad.h"

#include "Scene.h"
#include "GameObject.h"
#include "Mesh.h"
#include "Math/Vector.h"

#include "Shader/Shader.h"

#include "Settings/MeshSettings.h"

#include <GLFW/glfw3.h>
#include <string>
#include <vector>
#include <memory>
#include <utility>
#include <functional>
#include <unordered_map>

namespace PokarinEngine
{
	// -------------------------
	// �O���錾
	// -------------------------

	class RenderView;

	/// <summary>
	/// �Q�[���G���W��
	/// </summary>
	class Engine
	{
	public: // --------------- �R���X�g���N�^�E�f�X�g���N�^ ---------------

		Engine() = default;
		~Engine() = default;

	public: // ---------------------- �V�[���쐬 ------------------------

		/// <summary>
		/// �V�[�����쐬����
		/// </summary>
		/// <param name="name"> �V�[���� </param>
		/// <returns> �쐬�����V�[���̃|�C���^ </returns>
		ScenePtr CreateScene(const char* name);

	public: // ---------------------- �V�[���擾 ------------------------

		/// <summary>
		/// ���݂̃V�[�����擾����
		/// </summary>
		/// <returns> ���݂̃V�[���̎Q�� </returns>
		Scene& GetCurrentScene()
		{
			return *currentScene;
		}

		/// <summary>
		/// ���݂̃V�[�����擾����(�ǂݎ���p)
		/// </summary>
		/// <returns> ���݂̃V�[���̎Q��(�ǂݎ���p) </returns>
		const Scene& GetCurrentScene() const
		{
			return *currentScene;
		}

	public: // -------------- �G���W���A�Q�[���I�u�W�F�N�g�̊Ǘ� ----------------

		/// <summary>
		/// �Q�[���G���W�������s����
		/// </summary>
		/// <returns>
		/// <para> 0 : ����Ɏ��s���������� </para>
		/// <para> 0�ȊO : �G���[���������� </para>
		/// </returns>
		bool Run();

	public: // ---------------------- �X�^�e�B�b�N���b�V�� ----------------------

		/// <summary>
		/// �X�^�e�B�b�N���b�V�����擾����
		/// </summary>
		/// <param name="name"> �t�@�C���� </param>
		/// <returns> �t�@�C�����O����v����X�^�e�B�b�N���b�V�� </returns>
		StaticMeshPtr GetStaticMesh(const char* filename)
		{
			return meshBuffer->GetStaticMesh(filename);
		}

	public: // -------------------------- �e�N�X�`�� -----------------------------

		/// <summary>
		/// <para> �e�N�X�`�����擾���� </para>
		/// <para> �쐬����ꍇ�A�T�C�Y���w�肷�� </para>
		/// </summary>
		/// <param name="name"> �e�N�X�`���t�@�C���� </param>
		///	<param name="width"> �쐬����ꍇ�A�e�N�X�`�������w�肷�� </param>
		///	<param name="height"> �쐬����ꍇ�A�e�N�X�`���̍������w�肷�� </param>
		/// <returns> ���O��name�ƈ�v����e�N�X�`�� </returns>
		TexturePtr GetTexture(const char* name);

		/// <summary>
		/// �e�N�X�`�����擾����
		/// </summary>
		/// <param name="width"> �� </param>
		/// <param name="height"> ���� </param>
		/// <returns> �w�肵���傫���̃e�N�X�`�� </returns>
		TexturePtr GetTexture(GLsizei width, GLsizei height);

	public: // --------------------- �X�J�C�X�t�B�A�̕`�� ---------------------

		/// <summary>
		/// �X�J�C�X�t�B�A��`�悷��
		/// </summary>
		/// <param name="skySphereMaterial"> �X�J�C�X�t�B�A�p�}�e���A�� </param>
		void DrawSkySphere(const MaterialPtr skySphereMaterial = nullptr);

	private: // ---------------------- �G���W���̐��� -------------------------

		/// <summary>
		/// �Q�[���G���W��������������
		/// </summary>
		/// <returns>
		/// <para> 0 : ����ɏ��������ꂽ </para>
		/// <para> 0�ȊO : �G���[���������� </para>
		/// </returns>
		bool Initialize();

		/// <summary>
		/// �Q�[���G���W���̏�Ԃ��X�V����
		/// </summary>
		void Update();

		/// <summary>
		/// �Q�[���G���W���̏�Ԃ�`�悷��
		/// </summary>
		void Render();

	private: // ---------------------- �`��p�r���[ ---------------------

		/// <summary>
		/// �`��p�r���[�ɃJ��������̕`�����ێ�������
		/// </summary>
		/// <param name="camera"> �g�p����J���� </param>
		/// <param name="renderView"> �`�����ێ�����`��p�r���[ </param>
		void DrawRenderView(const Transform& camera, const RenderView& view);

	private: // ---------------------- �}�`�f�[�^ ------------------------

		// �}�`�f�[�^�Ǘ��p�o�b�t�@
		MeshBufferPtr meshBuffer;

	private: // ---------------------- �e�N�X�`�� ----------------------

		// �e�N�X�`���p�L���b�V��
		// <�t�@�C����, �e�N�X�`���̃|�C���^>
		std::unordered_map<std::string, TexturePtr> textureCache;

		// �R���X�g���N�^�A�f�X�g���N�^��
		// �Ăׂ�悤�ɂ��邽�߂̕⏕�N���X
		struct TexHelper : public Texture
		{
			TexHelper(const char* p) : Texture(p) {}
			TexHelper(GLsizei w, GLsizei h) : Texture(w, h) {}
		};

	private: // -------------------- �X�J�C�X�t�B�A --------------------

		// �X�J�C�X�t�B�A�p���f��
		StaticMeshPtr skySphere;

	private: // ------------------------ �V�[�� ------------------------

		// ���݂̃V�[��
		ScenePtr currentScene;

		// �V�[���z��
		SceneList scenes;
	};

} // namespace PokarinEngine

#endif // !ENGINE_H_INCLUDED
