/**
* @GameObject.h
*/
#ifndef POKARINENGINE_GAMEOBJECT_H_INCLUDED
#define POKARINENGINE_GAMEOBJECT_H_INCLUDED

#include "Json/UsingNameJson.h"

#include "Components/Rigidbody.h"
#include "Components/Transform.h"
#include "Components/ComponentAdder.h"

#include "Mesh/StaticMesh.h"

#include "Math/Vector.h"
#include "Color.h"

#include "UsingNames/UsingNodeEditor.h"
#include "UsingNames/UsingCollider.h"

#include "Configs/MeshConfig.h"

#include <string>
#include <vector>
#include <unordered_set>
#include <memory>
#include <type_traits>

namespace PokarinEngine
{
	// ----------------
	// �O���錾
	// ----------------

	class Scene;
	class Camera;

	/// int�^�Ƃ��Ă��g���̂ŁA���ʂ̗񋓌^
	/// <summary>
	/// �`��̗D��x
	/// </summary>
	enum RenderQueue
	{
		renderQueue_geometry = 2000,	// ��ʓI�Ȑ}�`
		renderQueue_transparent = 3000,	// �������Ȑ}�`
		renderQueue_overlay = 4000,		// UI, �S��ʃG�t�F�N�g�Ȃ�
		renderQueue_max = 5000,			// �L���[�̍ő�l
	};

	/// <summary>
	/// �Q�[���ɓo�ꂷ��l�X�ȃI�u�W�F�N�g��\����{�N���X
	/// </summary>
	class GameObject
	{
	public: // ---------------- �R���X�g���N�^�E�f�X�g���N�^ ----------------

		GameObject() = default;
		~GameObject() = default;

	public: // -------------------------- �֎~���� --------------------------

		/* ���ʂł��Ȃ��Ȃ�̂ŁA�֎~���� */

		// �R�s�[�R���X�g���N�^�̋֎~
		GameObject(GameObject&) = delete;

		// ����̋֎~
		GameObject& operator=(GameObject&) = delete;

	public: // ----------------------- �R���|�[�l���g -----------------------

		/// <summary>
		/// �Q�[���I�u�W�F�N�g�ɃR���|�[�l���g��ǉ����� 
		/// </summary>
		/// <typeparam name="T"> �R���|�[�l���g�N���X </typeparam>
		/// <returns> 
		/// <para> first : �ǉ������R���|�[�l���g�̃|�C���^ </para>
		/// <para> second : �ǉ��ł�����true, �ǉ��ς݂Œǉ��ł��Ȃ�������false </para> 
		/// </returns>
		template <class T>
		std::shared_ptr<T> AddComponent()
		{
			// �R���C�_�[�Ȃ�true
			constexpr bool isCollider = std::is_base_of_v<Collider, T>;

			// ---------------------------------------------------
			// �R���|�[�l���g���ǉ��ς݂��m�F����
			// ---------------------------------------------------

			// �R���C�_�[�͕����ǉ��\�Ȃ̂Ŋm�F���Ȃ�
			if (!isCollider)
			{
				// �ǉ��ς݂̃R���|�[�l���g���m�F���邽�߂Ɏ擾�����݂�
				auto component = GetComponent<T>();

				// �ǉ��ς݂̃R���|�[�l���g�Ȃ�
				// �擾�����R���|�[�l���g��Ԃ�
				if (component)
				{
					return component;
				}
			}

			// -------------------------------------------
			// �R���|�[�l���g��ǉ�����
			// -------------------------------------------

			// �R���|�[�l���g�쐬
			auto component = std::make_shared<T>();

			// �R���|�[�l���g�̖��O
			const std::string componentName = ComponentAdder::GetName<T>();

			// �R���|�[�l���g�̕\���p�^�C�g��
			// ��ʂł���悤�ɁA���O�̌��ɃQ�[���I�u�W�F�N�g�̎��ʔԍ���t����
			// (�G�f�B�^�ł͔�\���ɂȂ�悤�ɁA##��t����)
			const std::string componentTitle = componentName + "##" + std::to_string(id);

			// �R���|�[�l���g���ʔԍ�
			const int componentID = GetSingleComponentID();

			// �ǉ����̏��������s
			component->AddComponent(*this, componentName, componentTitle, componentID);

			// �R���C�_�[�Ȃ̂����肷��
			// T�����肷��x�[�X�N���X�Ɠ����A�܂��͊��N���X�Ȃ�true��Ԃ�
			if constexpr (isCollider)
			{
				// �R���C�_�[�̒ǉ�
				colliderList.push_back(component);
			}

			// �R���|�[�l���g�̒ǉ�
			componentList.push_back(component);

			// ���������p�R���|�[�l���g�Ȃ�ێ�
			if constexpr (std::is_base_of_v<Rigidbody, T>)
			{
				rigidbody = component;
			}

			// �J�����R���|�[�l���g�Ȃ�V�[���ɒǉ�����
			if constexpr (std::is_base_of_v<Camera, T>)
			{
				ownerScene->AddCamera(component);
			}

			return component;
		}

		/// <summary>
		/// �Q�[���I�u�W�F�N�g�������Ă���R���|�[�l���g���擾
		/// </summary>
		/// <typeparam name="T"> �擾����R���|�[�l���g </typeparam>
		/// <returns> ���������R���|�[�l���g </returns>
		template <class T>
		std::shared_ptr<T> GetComponent() const
		{
			// �R���|�[�l���g
			for (auto& e : componentList)
			{
				// �R���|�[�l���g��T�^
				// �܂���T�̔h���N���X�̏ꍇ�ɕϊ�����������
				// (���s�����ꍇ�A��̃|�C���^������)
				auto p = std::dynamic_pointer_cast<T>(e);

				// �R���|�[�l���g��������
				if (p)
				{
					// �������R���|�[�l���g��Ԃ�
					return p;
				}
			}

			// �R���|�[�l���g��������Ȃ������̂ŁA
			// nullptr��Ԃ�
			return nullptr;
		}

	public: // ------------------ �Q�[���I�u�W�F�N�g�̊Ǘ� ------------------

		/// <summary>
		/// �Q�[���I�u�W�F�N�g���폜����
		/// </summary>
		void Destroy() { isDestroyed = true; }

		/// <summary>
		/// �Q�[���I�u�W�F�N�g���폜�ς݂��擾����
		/// </summary>
		/// <returns> 
		/// <para> true : �폜����Ă��� </para>
		/// <para> false : �폜����Ă��Ȃ� </para>
		/// </returns>
		bool IsDestroyed() const { return isDestroyed; }

	public: // -------------------- �R���|�[�l���g�̊Ǘ� --------------------

		/// <summary>
		/// �폜�\��(�폜�����������s)�̃R���|�[�l���g�����S�ɍ폜����
		/// </summary>
		void RemoveDestroyedComponent();

	public: // ------------------- �Q�[���I�u�W�F�N�g���� -------------------

		/// <summary>
		/// ������
		/// </summary>
		/// <param name="[in] scene"> ������ł���V�[�� </param>
		/// <param name="[in] objectID"> ���ʔԍ� </param>
		/// <param name="[in] objectName"> ���O </param>
		/// <param name="[in] position"> �ʒu </param>
		/// <param name="[in] rotation"> ��]�p�x </param>
		void Initialize(Scene& scene, int objectID, const std::string& objectName,
			const Vector3& position, const Vector3& rotation);

		/// <summary>
		/// �X�V
		/// </summary>
		/// <param name="[in] isPlayGame"> �쐬���̃Q�[�����Đ����Ȃ�true </param>
		void Update(bool isPlayGame);

		/// <summary>
		/// �폜
		/// </summary>
		void OnDestroy();

		/// <summary>
		/// �R���C�_�[��`�悷��
		/// </summary>
		void DrawCollider() const;

	public: // ---------------------------- �G�f�B�^ ----------------------------

		/// <summary>
		/// �m�[�h�G�f�B�^���J��
		/// </summary>
		void OpenNodeEditor() const;

		/// <summary>
		/// �R���|�[�l���g���G�f�B�^�ɕ\������
		/// </summary>
		void RenderComponent();

	public: // --------------------------- ���̎擾 ---------------------------

		/// <summary>
		/// ������ł���V�[�����擾����
		/// </summary>
		/// <returns> ���̃Q�[���I�u�W�F�N�g�̎�����ł���V�[�� </returns>
		const Scene& GetOwnerScene() const
		{
			return *ownerScene;
		}

		/// <summary>
		/// ������ł���V�[�����擾����
		/// </summary>
		/// <returns> ���̃Q�[���I�u�W�F�N�g��������ł���V�[�� </returns>
		Scene& GetOwnerScene()
		{
			return *ownerScene;
		}

		/// <summary>
		/// ���ʔԍ����擾����
		/// </summary>
		/// <returns> ���ʔԍ� </returns>
		int GetID() const
		{
			return id;
		}

		/// <summary>
		/// ���ʔԍ��𕶎���Ŏ擾����
		/// </summary>
		/// <returns> ���ʔԍ�(������) </returns>
		std::string GetID_String() const
		{
			return std::to_string(id);
		}

		/// <summary>
		/// ���������p�R���|�[�l���g�������Ă��邩�擾����
		/// </summary>
		/// <returns>
		/// <para> true : �����Ă��� </para>
		/// <para> false : �����Ă��Ȃ� </para>
		/// </returns>
		bool HasRigidbody()
		{
			return rigidbody != nullptr;
		}

	public: // ----------------------------- �ۑ� ------------------------------

		/// <summary>
		/// �Q�[���I�u�W�F�N�g�̏���Json�^�Ɋi�[����
		/// </summary>
		/// <param name="[out] data"> �����i�[����Json�^ </param>
		void ToJson(Json& data) const;

	public: // --------------------------- �ǂݍ��� ----------------------------

		/// <summary>
		/// �Q�[���I�u�W�F�N�g�̏���Json�^����擾����
		/// </summary>
		/// <param name="[in] data"> �����i�[���Ă���Json�^ </param>
		void FromJson(const Json& data);

	public: // -------------------------- ��{�̏�� ---------------------------

		// �ʒu�E��]�E�g�嗦�̐���p�R���|�[�l���g
		TransformPtr transform;

		// ���̂̐F
		Color color = Color::white;

		// �n�ʂɒ��n���Ă���Ȃ�true
		bool isGrounded = false;

		// �R���C�_�[�Ǘ��p�z��
		std::vector<ColliderPtr> colliderList;

		// �I�u�W�F�N�g�̖��O
		std::string name = "";

		// �I�u�W�F�N�g���̍ő啶����
		const size_t nameSize = 32;

	public: // ------------------------- �`��n�̏�� --------------------------

		// �\������X�^�e�B�b�N���b�V��
		StaticMeshPtr staticMesh;

		// �Q�[���I�u�W�F�N�g�ŗL�̃}�e���A���z��
		MaterialList materialList;

		// �`��̗D��x
		int renderQueue = renderQueue_geometry;

	private: // ------------------- �R���|�[�l���g���ʔԍ� ---------------------

		/// <summary>
		/// �R���|�[�l���g���ʔԍ����擾����
		/// </summary>
		/// <returns> �d�����Ȃ����ʔԍ� </returns>
		int GetSingleComponentID();

	private: // ---------------------------- �X�V ------------------------------

		/// <summary>
		/// �Q�[���I�u�W�F�N�g�ɂ���R���|�[�l���g���X�V����
		/// </summary>
		/// <param name="[in] isPlayGame"> �쐬���̃Q�[�����Đ����Ȃ�true </param>
		void UpdateComponent(bool isPlayGame);

	private: // --------------------------- �Ǘ��p -----------------------------

		// �폜���ꂽ��true
		bool isDestroyed = false;

		// ���ʔԍ�
		int id = 0;

	private: // --------------------------- �V�[�� -----------------------------

		// ������ł���V�[��
		Scene* ownerScene = nullptr;

	private: // ----------------------- �m�[�h�G�f�B�^ -------------------------

		// �m�[�h�G�f�B�^
		NodeEditorPtr nodeEditor;

	private: // ----------------------- �R���|�[�l���g -------------------------

		// �R���|�[�l���g�Ǘ��p�z��
		std::vector<ComponentPtr> componentList;

		// �R���|�[�l���g���ʔԍ��̊Ǘ��p�z��
		std::unordered_set<int> componentIDList;

		// ���������p�R���|�[�l���g
		RigidbodyPtr rigidbody;
	};

} // namespace PokarinEngine

#endif // !POKARINENGINE_GAMEOBJECT_H_INCLUDED