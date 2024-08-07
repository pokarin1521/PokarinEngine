/**
* @file Scene.h
*/
#ifndef POKARINENGINE_SCENE_H_INCLUDED
#define POKARINENGINE_SCENE_H_INCLUDED

#include "Shader/Shader.h"

#include "UsingNames/UsingScene.h"
#include "UsingNames/UsingGameObject.h"
#include "UsingNames/UsingFramebufferObject.h"

#include "Components/Camera.h"

#include <string>
#include <memory>
#include <vector>
#include <set>
#include <unordered_set>
#include <unordered_map>

namespace PokarinEngine
{
	// -------------------
	// �O���錾
	// -------------------

	class Camera;

	// ------------------------------------
	// �^�̕ʖ����`
	// ------------------------------------

	using CameraPtr = std::shared_ptr<Camera>;

	/// <summary>
	/// �V�[���Ǘ��N���X
	/// </summary>
	class Scene
	{
	public: // --------------- �R���X�g���N�^�f�X�g���N�^ ------------------

		/// <summary>
		/// �V�[���쐬�p�R���X�g���N�^
		/// </summary>
		/// <param name="[in] sceneID"> �V�[�����ʔԍ� </param>
		/// <param name="[in] sceneName"> �V�[���� </param>
		Scene(int sceneID, const char* sceneName);

		~Scene() = default;

	public: // ------------------------- �֎~���� ----------------------------

		/* ���ʂł��Ȃ��Ȃ�̂ŁA�֎~���� */

		// �R�s�[�R���X�g���N�^�̋֎~
		Scene(const Scene&) = delete;

		// ����̋֎~
		Scene& operator=(const Scene&) = delete;

	public: // ------------------- �Q�[���I�u�W�F�N�g�쐬 --------------------

		/// <summary>
		/// �Q�[���I�u�W�F�N�g���쐬����
		/// </summary>
		/// <typeparam name="T"> �Q�[���I�u�W�F�N�g�N���X�܂��͂��̔h�� </typeparam>
		/// <param name="[in] name"> �I�u�W�F�N�g�̖��O </param>
		/// <param name="[in] position"> �I�u�W�F�N�g��z�u����ʒu </param>
		/// <param name="[in] rotation"> �I�u�W�F�N�g�̉�]�p�x </param>
		/// <returns> �ǉ������Q�[���I�u�W�F�N�g�̃|�C���^ </returns>
		GameObjectPtr CreateGameObject(const std::string& name,
			const Vector3& position = Vector3::zero, const Vector3& rotation = Vector3::zero);

		/// <summary>
		/// �Q�[���I�u�W�F�N�g���R�s�[����
		/// </summary>
		/// <param name="[in] copyObject"> �R�s�[���̃Q�[���I�u�W�F�N�g </param>
		void CopyGameObject(const GameObjectPtr& object);

	public: // ------------------------- �V�[������ --------------------------

		/// <summary>
		/// �V�[�����̏�Ԃ��X�V����
		/// </summary>
		/// <param name="[in] isPlayGame"> �Q�[���Đ����Ȃ�true </param>
		void Update(bool isPlayGame);

		/// <summary>
		/// �V�[�����̏�Ԃ�`�悷��
		/// </summary>
		void Render();

	public: // -------------------- �Q�[���I�u�W�F�N�g���� -------------------

		/// <summary>
		/// �V�[�����̑S�ẴQ�[���I�u�W�F�N�g��`�悷��
		/// </summary>
		void DrawGameObjectAll();

		/// <summary>
		/// �Q�[���I�u�W�F�N�g���폜����
		/// </summary>
		/// <param name="[in,out] object"> �폜����Q�[���I�u�W�F�N�g </param>
		void DestroyObject(GameObjectPtr& object);

	public: // ------------------------- ���̎擾 --------------------------

		/// <summary>
		/// �V�[�����̑S�ẴQ�[���I�u�W�F�N�g���擾����
		/// </summary>
		/// <returns> �Q�[���I�u�W�F�N�g�z�� </returns>
		const GameObjectList& GetGameObjectAll() const
		{
			return gameObjectList;
		}

		/// <summary>
		/// �V�[�������擾����
		/// </summary>
		/// <returns> �V�[���̖��O </returns>
		const char* GetName() const
		{
			return name.c_str();
		}

		/// <summary>
		/// FBO�̃e�N�X�`�����ʔԍ����擾����
		/// </summary>
		/// <returns> FBO�̃e�N�X�`�����ʔԍ� </returns>
		GLuint GetTextureID() const;

	public: // ------------------------ �J�����Ǘ��p -------------------------

		/// <summary>
		/// �J������ǉ�����
		/// </summary>
		/// <param name="camera"> �ǉ�����J���� </param>
		void AddCamera(const CameraPtr& camera)
		{
			// �J�����Ǘ��p�z��ɒǉ�����
			cameraList.emplace(camera);
		}

		/// <summary>
		/// �J�������폜����
		/// </summary>
		/// <param name="camera"> �폜����J���� </param>
		void DestroyCamera(const CameraPtr& camera)
		{
			// �Ǘ��p�z�񂩂�폜����
			cameraList.erase(camera);

			// �J�����������Ȃ����烁�C���J�������폜���ďI��
			if (cameraList.empty())
			{
				mainCamera = nullptr;

				return;
			}

			// �폜����J���������C���J�����Ȃ�
			// �z��̐擪�����C���J�����ɐݒ肷��
			if (mainCamera == camera)
			{
				mainCamera = *cameraList.begin();
			}
		}

	public: // ---------------------------- �ۑ� -----------------------------

		/// <summary>
		/// �V�[���̏����t�@�C���ɕۑ�����
		/// </summary>
		void SaveScene() const;

	public: // -------------------------- �ǂݍ��� ---------------------------

		/// <summary>
		/// �V�[���̏����t�@�C������ǂݍ���
		/// </summary>
		void LoadScene();

	private: // ----------------------- �^�̕ʖ����` -----------------------

		/* ���������C���Ŏg���̂ŁA���������ӂ�set�^���g�� */

		// ���ʔԍ���1�������Ȃ��̂ŏd���֎~
		using ObjectIDList = std::unordered_set<int>;

	private: // -------------------------- ���ʔԍ� --------------------------

		/// <summary>
		/// �I�u�W�F�N�g�̎��ʔԍ����擾����
		/// </summary>
		/// <returns> �d�����Ȃ����ʔԍ� </returns>
		int GetSingleObjectID();

	private: // ------------------------- �t�@�C���� -------------------------

		/// <summary>
		/// �ۑ���̃t�@�C�������擾����
		/// </summary>
		/// <returns> �ۑ���̃t�@�C���� </returns>
		std::string GetFileName() const
		{
			return folderName + name + ".json";
		}

	private: // ------------------- �Q�[���I�u�W�F�N�g�폜 -------------------

		/// <summary>
		/// �Q�[���G���W������S�ẴQ�[���I�u�W�F�N�g���폜����
		/// </summary>
		void ClearGameObject();

		/// <summary>
		/// �폜�\��(�폜�����������s)�̃Q�[���I�u�W�F�N�g�����S�ɍ폜����
		/// </summary>
		void RemoveDestroyedGameObject();

	private: // --------------------- �Q�[���I�u�W�F�N�g ---------------------

		// �Q�[���I�u�W�F�N�g�Ǘ��p�z��
		GameObjectList gameObjectList;

		// �I�u�W�F�N�g���ʔԍ��̊Ǘ��p�z��
		ObjectIDList objectIDList;

		// �Q�[���I�u�W�F�N�g�̍ő吔
		const size_t gameObjectMax = 1000000;

	private: // --------------------------- �`��p ---------------------------

		// �`��pFBO
		FramebufferObjectPtr fbo;

	private: // --------------------------- �J���� ---------------------------

		// �J�����Ǘ��p�z��
		std::unordered_map<GLuint, CameraPtr> cameraList;

	private: // ---------------------------- ��� ----------------------------

		// �V�[���̖��O
		std::string name = "";

		// ���ʔԍ�
		int id = 0;

	private: // --------------------------- �ۑ��p ---------------------------

		// �ۑ���̃t�H���_
		const std::string folderName = "My project/Assets/Scenes/";
	};

} // namespace PokarinEngine

#endif // !POKARINENGINE_SCENE_H_INCLUDED
