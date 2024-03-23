/**
* @GameObject.h
*/
#ifndef GAMEOBJECT_H_INCLUDED
#define GAMEOBJECT_H_INCLUDED

#include "Components/Transform.h"
#include "Components/Colliders/AabbCollider.h"
#include "Texture.h"

#include "VecMath.h"
#include "Color.h"

#include "UsingNames/UsingMesh.h"
#include "UsingNames/UsingNodeEditor.h"

#include <string>
#include <vector>
#include <memory>
#include <type_traits>

namespace PokarinEngine
{
	// ----------------
	// �O���錾
	// ----------------

	// ---------- �N���X ----------

	class Engine;
	class Scene;

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
		// �G���W���N���X�ɏ����J
		friend Engine;

		// �V�[���N���X�ɏ����J
		friend Scene;

	public: // ---------------- �R���X�g���N�^�E�f�X�g���N�^ ----------------

		/// <summary>
		/// �R���X�g���N�^
		/// </summary>
		GameObject() = default;
		virtual ~GameObject() = default;

	public: // -------------------------- �֎~���� --------------------------

		// �R�s�[�̋֎~
		GameObject(GameObject&) = delete;

		// ����̋֎~
		GameObject& operator=(GameObject&) = delete;

	public: // ----------------------- �R���|�[�l���g -----------------------

		/// <summary>
		/// �Q�[���I�u�W�F�N�g�ɃR���|�[�l���g�A�R���C�_�[��ǉ����� 
		/// </summary>
		/// <typeparam name="T"> �R���|�[�l���g�N���X </typeparam>
		/// <returns> �ǉ������R���|�[�l���g�̃|�C���^ </returns>
		template<typename T>
		std::shared_ptr<T> AddComponent()
		{
			// �R���|�[�l���g�쐬
			auto p = std::make_shared<T>();

			// �R���|�[�l���g�̏��L�҂�o�^
			p->owner = this;

			// �R���C�_�[�Ȃ̂����肷��
			// T�����肷��x�[�X�N���X�Ɠ����A�܂��͊��N���X�Ȃ�true��Ԃ�
			if constexpr (std::is_base_of_v<Collider, T>)
			{
				// �R���C�_�[�̒ǉ�
				colliders.push_back(p);
			}

			// �R���|�[�l���g�̒ǉ�
			components.push_back(p);

			// �R���|�[�l���g�ǉ����̏��������s
			p->Awake();

			return p;
		}

		/// <summary>
		/// �Q�[���I�u�W�F�N�g�������Ă���R���|�[�l���g���擾
		/// </summary>
		/// <typeparam name="T"> �擾����R���|�[�l���g </typeparam>
		/// <returns> ���������R���|�[�l���g </returns>
		template<typename T>
		std::shared_ptr<T> GetComponent()
		{
			// �R���|�[�l���g
			for (auto& e : components)
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
		/// �Q�[���I�u�W�F�N�g����R���|�[�l���g���폜����
		/// </summary>
		void RemoveDestroyedComponent();

	public: // ------------------- �Q�[���I�u�W�F�N�g���� -------------------

		/// <summary>
		/// ������
		/// </summary>
		virtual void Initialize();

		/// <summary>
		/// �X�V
		/// </summary>
		/// <param name="deltaTime"> �O��̍X�V����̌o�ߎ���(�b) </param>
		virtual void Update(float deltaTime);

		/// <summary>
		/// �Փˎ��̏���
		/// </summary>
		/// <param name="self"> �Փ˂����R���|�[�l���g(����) </param>
		/// <param name="other"> �Փ˂����R���|�[�l���g(����) </param>
		virtual void OnCollision(const ComponentPtr& self, const ComponentPtr& other);

		/// <summary>
		/// �폜
		/// </summary>
		virtual void OnDestroy();

	public: // ------------------------- �m�[�h�G�f�B�^ -------------------------

		/// <summary>
		/// �m�[�h�G�f�B�^���J��
		/// </summary>
		void OpenNodeEditor() const;

	public: // --------------------------- ���̎擾 ---------------------------

		/// <summary>
		/// �G���W���̋@�\���擾����
		/// </summary>
		/// <returns></returns>
		const Engine& GetEngine() const
		{
			return *engine;
		}

		/// <summary>
		/// �Q�[���I�u�W�F�N�g�̖��O���擾����
		/// </summary>
		/// <returns> �Q�[���I�u�W�F�N�g�̖��O </returns>
		const char* GetName() const
		{
			return name.c_str();
		}

		/// <summary>
		/// �I�u�W�F�N�g���̍ő啶�������擾����
		/// </summary>
		/// <returns> �I�u�W�F�N�g���̍ő啶���� </returns>
		size_t GetNameSize() const
		{
			return nameSize;
		}

		/// <summary>
		/// ���ʔԍ����擾����
		/// </summary>
		/// <returns> ���ʔԍ� </returns>
		int GetID() const
		{
			return id;
		}

	public: // -------------------------- ��{�̏�� ---------------------------

		// ���W�E��]�E�g�嗦�̐���p�R���|�[�l���g
		TransformPtr transform;

		// ���̂̐F
		Color color = { 1,1,1,1 };

		// true : ����ƂȂ镨�̂̏�ɏ���Ă���
		// false : ����ƂȂ镨�̂̏�ɏ���Ă��Ȃ�
		bool isGrounded = false;

	public: // ------------------------- �`��n�̏�� --------------------------

		// �\������X�^�e�B�b�N���b�V��
		StaticMeshPtr staticMesh;

		// �Q�[���I�u�W�F�N�g�ŗL�̃}�e���A���z��
		MaterialList materials;

		// �`��̗D��x
		int renderQueue = renderQueue_geometry;

	public: // ------------------------ �R���|�[�l���g -------------------------

		// �R���|�[�l���g�Ǘ��p�z��
		std::vector<ComponentPtr> components;

	public: // -------------------------- �R���C�_�[ ---------------------------

		// �R���C�_�[�Ǘ��p�z��
		std::vector<ColliderPtr> colliders;

	private: // -------------------------- �G���W�� ----------------------------

		// �G���W���N���X�̃|�C���^
		Engine* engine = nullptr;

	public: // ----------------------- �m�[�h�G�f�B�^ -------------------------

		// �m�[�h�G�f�B�^
		NodeEditorPtr nodeEditor;

	private: // --------------------------- �Ǘ��p -----------------------------

		// �폜���ꂽ��true
		bool isDestroyed = false;

		// ���ʔԍ�
		int id = 0;

	private: // ---------------------------- ���O ------------------------------

		// �I�u�W�F�N�g�̖��O
		std::string name = "";

		// �I�u�W�F�N�g�̎�ޖ�
		std::string typeName = "";

		// �I�u�W�F�N�g���̍ő啶����
		size_t nameSize = 32;
	};

} // namespace PokarinEngine

#endif // !GAMEOBJECT_H_INCLUDED