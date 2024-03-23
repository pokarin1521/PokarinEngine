/**
* @file Inspector.h
*/
#ifndef INSPECTOR_H_INCLUDED
#define INSPECTOR_H_INCLUDED

#include "ImGui/imgui.h"

#include "../UsingNames/UsingGameObject.h"

#include <string>
#include <memory>

namespace PokarinEngine
{
	// ------------------------
	// �O���錾
	// ------------------------

	class Engine;

	/// <summary>
	/// �C���X�y�N�^�[(�V�[�����̃I�u�W�F�N�g����p�E�B���h�E)
	/// </summary>
	class Inspector
	{
	public: // --------------- �R���X�g���N�^�E�f�X�g���N�^ ------------------

		Inspector() = default;
		~Inspector() = default;

	public: // ---------------------------- ���� -----------------------------

		/// <summary>
		/// ������
		/// </summary>
		void Initialize(Engine& e);

		/// <summary>
		/// �X�V
		/// </summary>
		/// <param name="selectObject"> �q�G�����L�[�őI�𒆂̃I�u�W�F�N�g </param>
		void Update(GameObjectPtr selectObject);

	private: // --------------------- �I�u�W�F�N�g����p ---------------------

		// �I�𒆂̃I�u�W�F�N�g�̖��O
		std::string selectObjectName = "";

	private: // -------------------------- �G���W�� --------------------------

		Engine* engine = nullptr;
	};

} // namespace PokarinEngine

#endif // !INSPECTOR_H_INCLUDED

