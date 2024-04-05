/**
* @file Component.h
*/
#ifndef COMPONENT_H_INCLUDED
#define COMPONENT_H_INCLUDED

#include "ImGui/imgui.h"

#include <string>
#include <memory>

namespace PokarinEngine
{
	// ------------------
	// �O���錾
	// ------------------

	class GameObject;
	class Component;

	// -----------------------
	// �^�̕ʖ����`
	// -----------------------

	using GameObjectPtr = std::shared_ptr<GameObject>;
	using ComponentPtr = std::shared_ptr<Component>;

	/// <summary>
	/// �R���|�[�l���g���N���X
	/// </summary>
	class Component
	{
		// �G���W���N���X�ɏ����J
		friend GameObject;

	public: // -------------------- �R���X�g���N�^�E�f�X�g���N�^ ----------------------

		Component() = default;
		virtual ~Component() = default;

	public: // ---------------------- �R���|�[�l���g�̏��Ǘ� ------------------------

		/// <summary>
		/// �R���|�[�l���g�̏��L�҂��擾����(�ǂݎ���p)
		/// </summary>
		/// <returns> ���L�҂̎Q�� </returns>
		const GameObject& GetOwner() const { return *owner; }

		/// <summary>
		/// �R���|�[�l���g�̏��L�҂��擾����
		/// </summary>
		/// <returns> ���L�҂̎Q�� </returns>
		GameObject& GetOwner() { return *owner; }

		/// <summary>
		/// �R���|�[�l���g���Q�[���I�u�W�F�N�g����폜����
		/// </summary>
		void Destroy() { isDestroyed = true; }

		/// <summary>
		/// �R���|�[�l���g���j�󂳂�Ă��邩�m�F
		/// </summary>
		/// <returns>
		/// <para> true : �j�󂳂�Ă��� </para>
		/// <para> false : �j�󂳂�Ă��Ȃ� </para>
		/// </returns>
		bool IsDestroyed() const { return isDestroyed; }

	public: // ------------------------ �R���|�[�l���g���� -------------------------

		// �Q�[���I�u�W�F�N�g�ɒǉ����ꂽ���ɌĂяo�����
		// �����ȊO���Q�Ƃ��Ȃ��ꍇ�́A���̊֐��ŏ���������
		virtual void Awake() {}

		// �ŏ���Update�̒��O�ŌĂяo�����
		// �����ȊO���Q�Ƃ���ꍇ�́A���̊֐��ŏ���������
		virtual void Start() {}

		// ���t���[���P��Ăяo�����
		virtual void Update(float deltaTime) {}

		// �Փ˂��N�����Ƃ��ɌĂяo�����
		virtual void OnCollision(const ComponentPtr& self, const ComponentPtr& other) {}

		// �Q�[���I�u�W�F�N�g���폜�����Ƃ��ɌĂяo�����
		virtual void OnDestroy() {}

	public: // ----------------------- �G�f�B�^�p --------------------------

		// �G�f�B�^�ɏ���\������
		virtual void RenderEditor() = 0;

	private: // ----------------- �R���|�[�l���g�̏�� ---------------------

		GameObject* owner = nullptr;	// ���̃R���|�[�l���g�̏��L��

		bool isStarted = false;			// Start�����s���ꂽ��true�ɂȂ�
		bool isDestroyed = false;		// Destroy�����s���ꂽ��true�ɂȂ�
	};

} // namespace PokarinEngine

#endif // !COMPONENT_H_INCLUDED
