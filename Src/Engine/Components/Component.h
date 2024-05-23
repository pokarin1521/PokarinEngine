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

		// ���t���[���Ăяo�����
		virtual void Update() {}

		// �쐬���̃Q�[�����Đ����̎���
		// ���t���[���Ăяo�����
		virtual void Update_PlayGame() {}

		// �Q�[���I�u�W�F�N�g���폜�����Ƃ��ɌĂяo�����
		virtual void OnDestroy() {}

	public: // ----------------------- �G�f�B�^�p --------------------------

		// ����ҏW�ł���悤�ɕ\������
		virtual void InfoEditor() = 0;

	public: // ------------------------ ���O�̎擾 -------------------------

		// ���O���擾����
		virtual const std::string& GetName() = 0;

	public: // --------------------------- ���O ----------------------------

		// �R���|�[�l���g�̖��O
		inline static const std::string name = "No Name";

	protected: // -------------------- ������̎擾 ------------------------

		/// <summary>
		/// ������ł���Q�[���I�u�W�F�N�g���擾����
		/// </summary>
		/// <returns> ������ł���Q�[���I�u�W�F�N�g </returns>
		const GameObject& GetOwnerObject() const { return *ownerObject; }

		/// <summary>
		/// ������ł���Q�[���I�u�W�F�N�g���擾����
		/// </summary>
		/// <returns> ������ł���Q�[���I�u�W�F�N�g </returns>
		GameObject& GetOwnerObject() { return *ownerObject; }

	private: // ----------------- �R���|�[�l���g�̏�� ---------------------

		// ������ł���Q�[���I�u�W�F�N�g
		GameObject* ownerObject = nullptr;

		// �������ς݂Ȃ�true
		bool isStarted = false;

		// �폜�ς݂Ȃ�true
		bool isDestroyed = false;
	};

} // namespace PokarinEngine

#endif // !COMPONENT_H_INCLUDED
