/**
* @file Scene.h
*/
#ifndef SCENE_H_INCLUDED
#define SCENE_H_INCLUDED

#include "Shader/Shader.h"
#include "Configs/MeshConfig.h"

#include "UsingNames/UsingGameObject.h"
#include "UsingNames/UsingMesh.h"

#include "GameObject.h"

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

	class Scene;
	class Engine;

	// ------------------------------------
	// �^�̕ʖ����`
	// ------------------------------------

	using ScenePtr = std::shared_ptr<Scene>;
	using SceneList = std::vector<ScenePtr>;

	/// <summary>
	/// �V�[���Ǘ��N���X
	/// </summary>
	class Scene
	{
	public: // --------------- �R���X�g���N�^�f�X�g���N�^ ------------------

		/// <summary>
		/// �V�[���쐬�p�R���X�g���N�^
		/// </summary>
		/// <param name="[in] e"> �G���W���N���X�̎Q�� </param>
		/// <param name="[in] sceneID"> �V�[�����ʔԍ� </param>
		/// <param name="[in] sceneName"> �V�[���� </param>
		Scene(Engine& e, int sceneID, const char* sceneName);

		~Scene() = default;

	public: // ------------------------- �֎~���� ----------------------------

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
		/// <param name="[in] staticMeshFile"> �X�^�e�B�b�N���b�V���̃t�@�C���� </param>
		/// <returns> �ǉ������Q�[���I�u�W�F�N�g�̃|�C���^ </returns>
		GameObjectPtr CreateGameObject(const std::string& name,
			const Vector3& position = Vector3::zero, const Vector3& rotation = Vector3::zero,
			const char* staticMeshFile = "");

		/// <summary>
		/// �Q�[���I�u�W�F�N�g�𕡐�����
		/// </summary>
		/// <param name="[in] copyObject"> �������̃Q�[���I�u�W�F�N�g </param>
		void CopyGameObject(const GameObjectPtr& object);

	public: // ------------------- �Q�[���I�u�W�F�N�g���� -------------------

		/// <summary>
		/// �Q�[���I�u�W�F�N�g���폜����
		/// </summary>
		/// <param name="[in] object"> �폜����Q�[���I�u�W�F�N�g </param>
		void DestroyObject(GameObjectPtr& object);

		/// <summary>
		/// �Q�[���G���W������S�ẴQ�[���I�u�W�F�N�g��j������
		/// </summary>
		void ClearGameObject();

		/// <summary>
		/// �Q�[���I�u�W�F�N�g�̏�Ԃ��X�V����
		/// </summary>
		void UpdateGameObject();

		/// <summary>
		/// �V�[�����̑S�ẴQ�[���I�u�W�F�N�g��`�悷��
		/// </summary>
		void DrawGameObjectAll();

		/// <summary>
		/// �폜����Q�[���I�u�W�F�N�g�����S�ɍ폜����
		/// </summary>
		void RemoveDestroyedGameObject();

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
		/// ���C���J�������擾����
		/// </summary>
		/// <returns> ���C���J���� </returns>
		const GameObject& GetMainCamera() const
		{
			return *mainCamera;
		}

		/// <summary>
		/// ���C���J�����̏����擾����
		/// </summary>
		/// <returns> ���C���J�����̏�� </returns>
		const Camera& GetMainCameraInfo() const
		{
			return *mainCameraInfo;
		}

		/// <summary>
		/// �V�[�������擾����
		/// </summary>
		/// <returns> �V�[���̖��O </returns>
		const char* GetName() const
		{
			return name.c_str();
		}

	public: // --------------------- �X�^�e�B�b�N���b�V�� --------------------

		/// <summary>
		/// �X�^�e�B�b�N���b�V�����擾����
		/// </summary>
		/// <param name="[in] fileName"> �t�@�C���� </param>
		/// <returns> �t�@�C��������v����X�^�e�B�b�N���b�V�� </returns>
		StaticMeshPtr GetStaticMesh(const std::string& fileName);

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

	private: // -------------------------- ���ʔԍ� ----------------------------

		/// <summary>
		/// �I�u�W�F�N�g�̎��ʔԍ����擾����
		/// </summary>
		/// <returns> �d�����Ȃ����ʔԍ� </returns>
		int GetSingleObjectID();

	private: // ------------------------- �t�@�C���� ---------------------------

		/// <summary>
		/// �ۑ���̃t�@�C�������擾����
		/// </summary>
		/// <returns> �ۑ���̃t�@�C���� </returns>
		std::string GetFileName() const
		{
			return folderName + name + ".json";
		}

	private: // ---------------------- �Q�[���I�u�W�F�N�g ----------------------

		// �Q�[���I�u�W�F�N�g�Ǘ��p�z��
		GameObjectList gameObjectList;

		// �I�u�W�F�N�g���ʔԍ��̊Ǘ��p�z��
		ObjectIDList objectIDList;

		// �Q�[���I�u�W�F�N�g�̍ő吔
		const size_t gameObjectMax = 1000000;

	private: // ------------------------- ���C���J���� ------------------------

		// ���C���J����
		GameObjectPtr mainCamera;

		// ���C���J�����̏��
		CameraPtr mainCameraInfo;

	private: // ----------------------------- ��� ----------------------------

		// �V�[���̖��O
		std::string name = "";

		// ���ʔԍ�
		int id = 0;

	private: // --------------------------- �G���W�� --------------------------

		// �G���W���N���X�̃|�C���^
		Engine* engine = nullptr;

	private: // ---------------------------- �ۑ��p ---------------------------

		// �ۑ���̃t�H���_
		const std::string folderName = "My project/Assets/Scenes/";
	};

} // namespace PokarinEngine

#endif // !SCENE_H_INCLUDED
