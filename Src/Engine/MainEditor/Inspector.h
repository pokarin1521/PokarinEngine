/**
* @file Inspector.h
*/
#ifndef POKARINENGINE_INSPECTOR_H_INCLUDED
#define POKARINENGINE_INSPECTOR_H_INCLUDED

#include "ImGui/imgui.h"

#include "../UsingNames/UsingGameObject.h"

#include <string>
#include <memory>

namespace PokarinEngine
{
	/// <summary>
	/// �C���X�y�N�^�[(�V�[�����̃I�u�W�F�N�g����p�E�B���h�E)
	/// </summary>
	class Inspector
	{
	public: // ---------------- �R���X�g���N�^�E�f�X�g���N�^ -----------------

		Inspector() = default;
		~Inspector() = default;

	public: // ---------------------------- ���� -----------------------------

		/// <summary>
		/// �X�V
		/// </summary>
		/// <param name="[in,out] hierarchySelect"> �q�G�����L�[�őI�𒆂̃I�u�W�F�N�g </param>
		void Update(GameObjectPtr& hierarchySelect);

	private: // ------------------- �I�u�W�F�N�g���̕\�� ---------------------

		/// <summary>
		/// �I�u�W�F�N�g����\������
		/// </summary>
		void RenderName();

	private: // ------------------- �R���|�[�l���g�ǉ��p ---------------------

		/// <summary>
		/// �R���|�[�l���g�ǉ��p�|�b�v�A�b�v�̏���
		/// </summary>
		void AddComponentPopup();

	private: // ------------------- �I�𒆂̃I�u�W�F�N�g ---------------------

		// �I�𒆂̃Q�[���I�u�W�F�N�g
		GameObjectPtr selectObject;
	};

} // namespace PokarinEngine

#endif // !POKARINENGINE_INSPECTOR_H_INCLUDED

