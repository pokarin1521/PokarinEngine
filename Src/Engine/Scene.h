/**
* @file Scene.h
*/
#ifndef SCENE_H_INCLUDED
#define SCENE_H_INCLUDED

#include "Shader/Shader.h"
#include "Settings/MeshSettings.h"

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
	public: // ------------------------- �V�[���쐬 --------------------------

		/// <summary>
		/// �V�[�����쐬����
		/// </summary>
		/// <param name="_engine"> �G���W���N���X�̎Q�� </param>
		/// <param name="sceneName"> �V�[���� </param>
		static ScenePtr Create(Engine& e, const char* sceneName)
		{
			return std::make_shared<Scene>(e, sceneName);
		}

	public: // --------------- �R���X�g���N�^�f�X�g���N�^ ------------------

		/// <summary>
		/// �V�[���쐬�p�R���X�g���N�^
		/// </summary>
		/// <param name="e"> �G���W���N���X�̎Q�� </param>
		/// <param name="sceneName"> �V�[���� </param>
		Scene(Engine& e, const char* sceneName);

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
		/// <param name="name"> �I�u�W�F�N�g�̖��O </param>
		/// <param name="position"> �I�u�W�F�N�g��z�u������W </param>
		/// <param name="rotation"> �I�u�W�F�N�g�̉�]�p�x </param>
		/// <param name="staticMeshFile"> �X�^�e�B�b�N���b�V���̃t�@�C���� </param>
		/// <returns> �ǉ������Q�[���I�u�W�F�N�g�̃|�C���^ </returns>
		GameObjectPtr CreateGameObject(const std::string& name,
			const Vector3& position = { 0,0,0 }, const Vector3& rotation = { 0,0,0 },
			const char* staticMeshFile = "");

		/// <summary>
		/// �Q�[���I�u�W�F�N�g�𕡐�����
		/// </summary>
		/// <param name="copyObject"> �������̃Q�[���I�u�W�F�N�g </param>
		void CopyGameObject(GameObjectPtr object);

	public: // ------------------- �Q�[���I�u�W�F�N�g���� -------------------

		/// <summary>
		/// �Q�[���I�u�W�F�N�g�̖��O��ύX����
		/// </summary>
		/// <param name="object"> ���O��ύX����I�u�W�F�N�g </param>
		/// <param name="afterName"> �ύX��̖��O </param>
		void ChangeObjectName(GameObjectPtr object, const std::string& afterName);

		/// <summary>
		/// �Q�[���I�u�W�F�N�g���폜����
		/// </summary>
		/// <param name="object"> �폜����Q�[���I�u�W�F�N�g </param>
		void DestroyObject(GameObjectPtr object);

		/// <summary>
		/// �Q�[���G���W������S�ẴQ�[���I�u�W�F�N�g��j������
		/// </summary>
		void ClearGameObjectAll();

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
		GameObjectList GetGameObjectAll() const
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
		/// <returns> �J���������Ǘ�����R���|�[�l���g </returns>
		const Camera& GetCameraInfo() const
		{
			return *cameraInfo;
		}

		/// <summary>
		/// �V�[�������擾����
		/// </summary>
		/// <returns> �V�[���̖��O </returns>
		const char* GetName() const
		{
			return name.c_str();
		}

	private: // ----------------------- �^�̕ʖ����` -----------------------

		// �����t���̃I�u�W�F�N�g���z��
		using SetNumberNameList = std::set<std::string>;

		// <�I�u�W�F�N�g��ޖ�, �����t���I�u�W�F�N�g��>
		using ObjectTypeNameList = std::unordered_map<std::string, SetNumberNameList>;

		/* ���������C���Ŏg���̂ŁA���������ӂ�set�^���g�� */

		// ���ʔԍ���1�������Ȃ��̂ŏd���֎~
		using ObjectIDList = std::unordered_set<int>;

	private: // ---------------------- ���O�E���ʔԍ� ------------------------

		/// <summary>
		/// <para> ���̃I�u�W�F�N�g�Əd�����Ȃ����O���擾���� </para>
		/// <para> (�d�����Ă����ꍇ�́A���ɐ�����t����) </para>
		/// </summary>
		/// <param name="objectName"> �I�u�W�F�N�g�� </param>
		/// <returns> 
		/// <para> �d�����Ȃ��悤�ɕύX�������O </para>
		/// <para> (�d�����Ă��Ȃ��Ȃ炻�̂܂܂̖��O) </para>
		/// </returns>
		std::string GetSingleObjectName(const std::string& typeName) const;

		/// <summary>
		/// ���̃I�u�W�F�N�g�Əd�����Ȃ����ʔԍ���z��ɒǉ�����
		/// </summary>
		/// <returns> �d�����Ȃ����ʔԍ�(����) </returns>
		int AddSingleObjectID();

	private: // ---------------------- �Q�[���I�u�W�F�N�g ----------------------

		// �Q�[���I�u�W�F�N�g�Ǘ��p�z��
		GameObjectList gameObjectList;

		// �I�u�W�F�N�g��ޖ��̊Ǘ��p�z��
		// <�I�u�W�F�N�g�̎�ޖ�, �����t���I�u�W�F�N�g��>
		ObjectTypeNameList objectTypeNameList;

		// �I�u�W�F�N�g���ʔԍ��̊Ǘ��p�z��
		ObjectIDList objectIDList;

		// �Q�[���I�u�W�F�N�g�̍ő吔
		// �]�T�����Ă�悤�ɏ��Ȃ߂ɐݒ�
		const size_t gameObjectMax = INT_MAX - 5;

	private: // ------------------------- ���C���J���� ------------------------

		// ���C���J����
		GameObjectPtr mainCamera;

		// ���C���J�����̏��
		CameraPtr cameraInfo;

	private: // ----------------------------- ��� ----------------------------

		// �V�[���̖��O
		std::string name = "";

	private: // --------------------------- �G���W�� --------------------------

		// �G���W���N���X�̃|�C���^
		Engine* engine = nullptr;
	};

} // namespace PokarinEngine

#endif // !SCENE_H_INCLUDED
