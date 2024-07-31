/**
/**
* @file Engine.h
*/
#ifndef POKARINENGINE_ENGINE_H_INCLUDED
#define POKARINENGINE_ENGINE_H_INCLUDED

#include "glad/glad.h"

#include "MainEditor/MainEditor.h"

#include "Scene.h"
#include "GameObject.h"

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

	private: // ------------------------ �G���W���̐��� -----------------------

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

	private: // ---------------------- �V�[���̎��ʔԍ� -----------------------

		/// <summary>
		/// �V�[���̎��ʔԍ����쐬����
		/// </summary>
		/// <returns> �쐬�������ʔԍ� </returns>
		int CreateSceneID();

	private: // ------------------------- �`��p�r���[ ------------------------

		/// <summary>
		/// �`��p�r���[�ɃJ��������̕`�����ێ�������
		/// </summary>
		/// <param name="[in] camera"> �g�p����J���� </param>
		/// <param name="[in] renderView"> �`�����ێ�����`��p�r���[ </param>
		void DrawRenderView(const Transform& camera, const RenderView& renderView);


	private: // -------------------- �X�J�C�X�t�B�A�̕`�� ---------------------

		/// <summary>
		/// �X�J�C�X�t�B�A��`�悷��
		/// </summary>
		void DrawSkySphere(const Transform& camera);

	private: // ------------------------- �}�`�f�[�^ --------------------------

		// �}�`�f�[�^�Ǘ��p�o�b�t�@
		MeshBufferPtr meshBuffer;

	private: // ----------------------- �X�J�C�X�t�B�A ------------------------

		// �X�J�C�X�t�B�A�p���f��
		StaticMeshPtr skySphere;

	private: // --------------------------- �V�[�� ----------------------------

		// ���݂̃V�[��
		ScenePtr currentScene;

		// �V�[���Ǘ��p�z��
		SceneList sceneList;

		// �V�[�����ʔԍ��̊Ǘ��p�z��
		std::unordered_set<int> sceneIDList;

		// �V�[���̍ő吔
		const int sceneMax = 1000;

	private: // ------------------------ ���C���G�f�B�^ -----------------------

		// ���C���G�f�B�^
		MainEditor mainEditor;

	private: // ----------------------- �Q�[���Đ��m�F�p ----------------------

		// �쐬���̃Q�[�����Đ����Ȃ�true
		bool isPlayGame = false;
	};

} // namespace PokarinEngine

#endif // !POKARINENGINE_ENGINE_H_INCLUDED
