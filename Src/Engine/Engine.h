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

#include "MainEditor/MainEditor.h"
#include "NodeScript/NodeScript.h"

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

	public: // ------------------------------ ���� -------------------------------

		/// <summary>
		/// �����̌������茋��
		/// </summary>
		struct RaycastHit
		{
			// �ŏ��Ɍ����ƌ��������R���C�_�[
			ColliderPtr collider;

			// �ŏ��̌�_�̍��W
			Vector3 point;

			// �ŏ��̌�_�܂ł̋���
			float distance;
		};

		// ��������̑ΏۂɂȂ邩�ǂ������ׂ�q��^
		// �߂�l�́A��������̑ΏۂɂȂ邩�ǂ���
		using RaycastPredicate = std::function<bool(
			const ColliderPtr& collider, float distance)>;

		/// <summary>
		/// �}�E�X���W���甭�˂����������擾����
		/// </summary>
		/// <returns> �}�E�X���W���甭�˂������� </returns>
		Collision::Ray GetRayFromMousePosition() const;

		/// <summary>
		/// �����ƃR���C�_�[�̌�������
		/// </summary>
		/// <param name="[in] ray"> ���� </param>
		/// <param name="[out] hitInfo"> �����ƍŏ��Ɍ��������R���C�_�[�̏�� </param>
		/// <param name="pred"> ����������s���R���C�_�[��I�ʂ���q�� </param>
		/// <returns>
		/// <para> true : �R���C�_�[�ƌ������� </para>
		/// <para> false : �R���C�_�[�ƌ������Ȃ����� </para>
		/// </returns>
		bool Raycast(const Collision::Ray& ray, RaycastHit& hitInfo,
			const RaycastPredicate& pred) const;

	private: // ------------------------- �R���C�_�[ --------------------------

		// ���[���h���W�n�̃R���C�_�[��\���\����
		struct WorldCollider
		{
			/// <summary>
			/// ���[���h�R���C�_�[�̍��W��ύX����
			/// </summary>
			/// <param name="v"> �ړ��� </param>
			void AddPosition(const Vector3& v)
			{
				// �R���C�_�[�̎�����̍��W��ύX
				origin->GetOwner().transform->position += v;

				// �R���C�_�[�̍��W��ύX
				world->AddPosition(v);
			}

			// �I���W�i���̃R���C�_�[
			ColliderPtr origin;

			// �R���C�_�[
			ColliderPtr world;
		};

		// ���[���h���W�n�R���C�_�[�̔z��
		using WorldColliderList = std::vector<WorldCollider>;

		/// <summary>
		/// �ђʃx�N�g�����Q�[���I�u�W�F�N�g�ɔ��f����
		/// </summary>
		/// <param name="worldColliders"> ���[���h���W�n�̃R���C�_�[�z�� </param>
		/// <param name="gameObject"> �Q�[���I�u�W�F�N�g </param>
		/// <param name="penetration"> �ђʃx�N�g�� </param>
		void ApplyPenetration(WorldColliderList* worldColliders,
			GameObject& gameObject, const Vector3& penetration);

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

	private: // ---------------- �Q�[���I�u�W�F�N�g�̏Փ� ------------------

		/// <summary>
		/// �Q�[���I�u�W�F�N�g�̏Փˏ���
		/// </summary>
		void HandleGameObjectCollision();

		/// <summary>
		/// �R���C�_�[�P�ʂ̏Փ˔���
		/// </summary>
		/// <param name="colliderA"> 
		/// ����Ώۂ̃��[���h�R���C�_�[�z��(�Փ˂��鑤) </param>
		/// <param name="colliderB"> 
		/// ����Ώۂ̃��[���h�R���C�_�[�z��(�Փ˂���鑤) </param>
		void HandleWorldColliderCollision(
			WorldColliderList* collidersA, WorldColliderList* collidersB);

	private: // ---------------------- �`��p�r���[ ---------------------

		/// <summary>
		/// �`��p�r���[�ɃJ��������̕`�����ێ�������
		/// </summary>
		/// <param name="camera"> �g�p����J���� </param>
		/// <param name="renderView"> �`�����ێ�����`��p�r���[ </param>
		void DrawRenderView(const Transform& camera, const RenderView& view);

	private: // --------------------- �}�`�f�[�^ -------------------------

		// �}�`�f�[�^�Ǘ��I�u�W�F�N�g
		MeshBufferPtr meshBuffer;

	private: // --------------------- �L���b�V�� -------------------------

		// �e�N�X�`���p�L���b�V��
		// <�t�@�C����, �e�N�X�`���̃|�C���^>
		std::unordered_map<std::string, TexturePtr> textureCache;

	private: // ----------------------- �V�[�� ---------------------------

		// ���݂̃V�[��
		ScenePtr currentScene;

		// �V�[���z��
		SceneList scenes;

	private: // --------------------------- ���� ----------------------------

		// �O��X�V���̎���
		double previousTime = 0;

		// �O��X�V����̌o�ߎ���
		float deltaTime = 0;

	private: // ------------------------- �G�f�B�^ --------------------------

		// ���C���G�f�B�^����p
		MainEditor mainEditor;
	};

} // namespace PokarinEngine

#endif // !ENGINE_H_INCLUDED
