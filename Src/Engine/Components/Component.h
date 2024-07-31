/**
* @file Component.h
*/
#ifndef POKARINENGINE_COMPONENT_H_INCLUDED
#define POKARINENGINE_COMPONENT_H_INCLUDED

#include "Json/UsingNameJson.h"

#include "../UsingNames/UsingComponent.h"
#include "../UsingNames/UsingGameObject.h"

#include <string>

namespace PokarinEngine
{
	/// <summary>
	/// �R���|�[�l���g���N���X
	/// </summary>
	class Component
	{
	public: // -------------------- �R���X�g���N�^�E�f�X�g���N�^ ----------------------

		Component() = default;
		virtual ~Component() = default;

	public: // ---------------------------- ������̎擾 ------------------------------

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

	public: // --------------------------------- ���� --------------------------------

		/// <summary>
		/// �Q�[���I�u�W�F�N�g�ɒǉ��������̏���
		/// </summary>
		/// <param name="[in] owner"> ������ɂȂ�Q�[���I�u�W�F�N�g </param>
		/// <param name="[in] componentName"> �R���|�[�l���g�̖��O </param>
		/// <param name="[in] componentTitle"> �R���|�[�l���g�̕\���p�^�C�g�� </param>
		/// <param name="[in] componentID"> �R���|�[�l���g�̎��ʔԍ� </param>
		void AddComponent(GameObject& owner, const std::string& componentName,
			const std::string& componentTitle, int componentID)
		{
			ownerObject = &owner;
			name = componentName;
			title = componentTitle;
			id = componentID;

			Awake();
		}

		/// <summary>
		/// �ŏ��̍X�V�̒��O�ł̏�����
		/// </summary>
		void Initialize()
		{
			if (isStarted)
			{
				return;
			}

			Start();
			isStarted = true;
		}

		/// <summary>
		/// ���t���[���Ăяo�����
		/// </summary>
		virtual void Update() {}

		/// <summary>
		/// �쐬���̃Q�[�����Đ����̎��ɖ��t���[���Ăяo�����
		/// </summary>
		virtual void Update_PlayGame() {}

		/// <summary>
		/// �Q�[���I�u�W�F�N�g���폜�����Ƃ��ɌĂяo�����
		/// </summary>
		virtual void OnDestroy() {}

	public: // --------------------------------- �폜 --------------------------------

		/// <summary>
		/// �R���|�[�l���g���Q�[���I�u�W�F�N�g����폜����
		/// </summary>
		void Destroy() { isDestroyed = true; }

		/// <summary>
		/// �R���|�[�l���g���폜����Ă��邩�m�F
		/// </summary>
		/// <returns>
		/// <para> true : �폜����Ă��� </para>
		/// <para> false : �폜����Ă��Ȃ� </para>
		/// </returns>
		bool IsDestroyed() const { return isDestroyed; }

	public: // ----------------------------- �G�f�B�^�p ------------------------------

		/// <summary>
		/// �G�f�B�^�ɏ���\������
		/// </summary>
		void RenderInfo();

	public: // --------------------------- �ۑ��E�ǂݍ��� ----------------------------

		/// <summary>
		/// �R���|�[�l���g�̏���Json�^�Ɋi�[����
		/// </summary>
		/// <param name="[out] Json"> �����i�[����Json�^ </param>
		virtual void ToJson(Json& data) const = 0;

		/// <summary>
		/// �R���|�[�l���g�̏���Json�^����擾����
		/// </summary>
		/// <param name="[out] data"> �����i�[���Ă���Json�^ </param>
		virtual void FromJson(const Json& data) = 0;

	public: // --------------------------------- ���O --------------------------------

		/// <summary>
		/// ���O���擾����
		/// </summary>
		/// <returns> �R���|�[�l���g�̖��O </returns>
		const std::string& GetName() const
		{
			return name;
		}

	public: // ------------------------------- ���ʔԍ� ------------------------------

		/// <summary>
		/// ���ʔԍ����擾����
		/// </summary>
		/// <returns> �R���|�[�l���g�̎��ʔԍ� </returns>
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

	private: // ------------------------------- ������ -------------------------------

		/// ������Ǝ��g�ȊO�ɃA�N�Z�X���Ȃ��ꍇ
		/// <summary>
		/// �Q�[���I�u�W�F�N�g�ɒǉ����ꂽ���̏�����
		/// </summary>
		virtual void Awake() {}

		/// ������Ǝ��g�ȊO�ɃA�N�Z�X����ꍇ
		/// <summary>
		/// �ŏ��̍X�V�̒��O�ł̏�����
		/// </summary>
		virtual void Start() {}

	private: // ----------------------------- �G�f�B�^�p ------------------------------

		/// <summary>
		/// ����ҏW�ł���悤�ɕ\������
		/// </summary>
		virtual void InfoEditor() = 0;

		/// <summary>
		/// �R���|�[�l���g����p�|�b�v�A�b�v�̏���
		/// </summary>
		void ControlPopup();

	private: // -------------------------------- ��� ---------------------------------

		// ������ł���Q�[���I�u�W�F�N�g
		GameObject* ownerObject = nullptr;

		// �������ς݂Ȃ�true
		bool isStarted = false;

		// �폜�ς݂Ȃ�true
		bool isDestroyed = false;

		// ���O
		std::string name = "No Name";

		// �\���p�^�C�g��
		std::string title = "No Title";

		// ���ʔԍ�
		int id = 0;
	};

} // namespace PokarinEngine

#endif // !POKARINENGINE_COMPONENT_H_INCLUDED
