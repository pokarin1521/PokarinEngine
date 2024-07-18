/**
/**
* @file Engine.h
*/
#ifndef ENGINE_H_INCLUDED
#define ENGINE_H_INCLUDED

#include "glad/glad.h"

#include "MainEditor/MainEditor.h"

#include "Scene.h"
#include "GameObject.h"
#include "Mesh.h"

#include "Math/Vector.h"

#include "Shader/Shader.h"

#include "Configs/MeshConfig.h"

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

	public: // ------------------------ �V�[���쐬 ------------------------

		/// <summary>
		/// �V�[�����쐬����
		/// </summary>
		/// <param name="[in] name"> �V�[���� </param>
		/// <returns> �쐬�����V�[���̃|�C���^ </returns>
		ScenePtr CreateScene(const char* name);

	public: // ------------------------ �V�[���擾 ------------------------

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
		int Run();

	public: // ---------------------- �X�^�e�B�b�N���b�V�� ----------------------

		/// <summary>
		/// �X�^�e�B�b�N���b�V�����擾����
		/// </summary>
		/// <param name="[in] fileName"> �t�@�C���� </param>
		/// <returns> �t�@�C�����O����v����X�^�e�B�b�N���b�V�� </returns>
		StaticMeshPtr GetStaticMesh(const std::string& fileName)
		{
			return meshBuffer->GetStaticMesh(fileName);
		}

	public: // -------------------------- �e�N�X�`�� -----------------------------

		/// <summary>
		/// <para> �e�N�X�`�����擾���� </para>
		/// <para> �쐬����ꍇ�A�T�C�Y���w�肷�� </para>
		/// </summary>
		/// <param name="[in] name"> �e�N�X�`���t�@�C���� </param>
		/// <returns> ���O��name�ƈ�v����e�N�X�`�� </returns>
		TexturePtr GetTexture(const char* name);

		/// <summary>
		/// �e�N�X�`�����擾����
		/// </summary>
		/// <param name="[in] width"> �� </param>
		/// <param name="[in] height"> ���� </param>
		/// <returns> �w�肵���傫���̃e�N�X�`�� </returns>
		TexturePtr GetTexture(GLsizei width, GLsizei height);

	public: // --------------------- �X�J�C�X�t�B�A�̕`�� ---------------------

		/// <summary>
		/// �X�J�C�X�t�B�A��`�悷��
		/// </summary>
		/// <param name="[in] skySphereMaterial"> �X�J�C�X�t�B�A�p�}�e���A�� </param>
		void DrawSkySphere(const MaterialPtr& skySphereMaterial = nullptr);

	public: // ------------------------- �Q�[���̍Đ� -------------------------

		/// <summary>
		/// �쐬���̃Q�[�����Đ������擾����
		/// </summary>
		/// <returns>
		/// <para> true : �Q�[�����Đ��� </para>
		/// <para> false : �Q�[�����Đ�����Ă��Ȃ� </para>
		/// </returns>
		bool IsPlayGame()
		{
			return isPlayGame;
		}

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

	private: // -------------------- �V�[���̎��ʔԍ� -------------------

		/// <summary>
		/// �V�[���̎��ʔԍ����쐬����
		/// </summary>
		/// <returns> �쐬�������ʔԍ� </returns>
		int CreateSceneID();

	private: // ---------------------- �`��p�r���[ ---------------------

		/// <summary>
		/// �`��p�r���[�ɃJ��������̕`�����ێ�������
		/// </summary>
		/// <param name="[in] camera"> �g�p����J���� </param>
		/// <param name="[in] renderView"> �`�����ێ�����`��p�r���[ </param>
		void DrawRenderView(const Transform& camera, const RenderView& renderView);

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

		// �V�[���Ǘ��p�z��
		SceneList sceneList;

		// �V�[�����ʔԍ��̊Ǘ��p�z��
		std::unordered_set<int> sceneIDList;

		// �V�[���̍ő吔
		const int sceneMax = 1000;

	private: // -------------------- ���C���G�f�B�^ --------------------

		// ���C���G�f�B�^
		MainEditor mainEditor;

	private: // ------------------- �Q�[���Đ��m�F�p -------------------

		// �쐬���̃Q�[�����Đ����Ȃ�true
		bool isPlayGame = false;
	};

} // namespace PokarinEngine

#endif // !ENGINE_H_INCLUDED
