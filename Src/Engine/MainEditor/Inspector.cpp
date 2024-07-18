/**
* @file Inspector.cpp
*/
#include "Inspector.h"

#include "../Engine.h"
#include "../GameObject.h"
#include "../Color.h"

#include "../Components/ComponentAdder.h"

namespace PokarinEngine
{
#pragma region Inspector

	/// <summary>
	/// �I�u�W�F�N�g����\������
	/// </summary>
	void Inspector::RenderName()
	{
		// �I�𒆃I�u�W�F�N�g�̖��O
		std::string selectObjectName = selectObject->name;

		// �I�u�W�F�N�g������͂���e�L�X�g��
		// ##�Ńe�L�X�g���\������
		if (ImGui::InputText("##ObjectName", selectObjectName.data(), selectObject->nameSize,
			ImGuiInputTextFlags_::ImGuiInputTextFlags_EnterReturnsTrue))
		{
			// �V�������O�����͂���Ă���
			if (selectObjectName[0] != '\0' &&
				selectObjectName != selectObject->name)
			{
				// �I�[�ʒu
				size_t endLine = selectObjectName.find_first_of('\0');

				// ���O��ύX
				// �]�v�ȋ󔒂�����ƍ���̂ŁA�ŏ���\0�܂ł�n��
				selectObject->name = selectObjectName.substr(0, endLine);
			}
		}
	}

	/// <summary>
	/// �X�V
	/// </summary>
	/// <param name="[in,out] hierarchySelect"> �q�G�����L�[�őI�𒆂̃I�u�W�F�N�g </param>
	void Inspector::Update(GameObjectPtr& hierarchySelect)
	{
		// �C���X�y�N�^�[�E�B���h�E
		ImGui::Begin("Inspector");
		{
			// �I�𒆂̃I�u�W�F�N�g���X�V
			selectObject = hierarchySelect;

			// �q�G�����L�[�ŃI�u�W�F�N�g��I�����Ă��Ȃ��Ȃ�
			// �����\�����Ȃ�
			if (!selectObject)
			{
				ImGui::End();
				return;
			}

			// �I�u�W�F�N�g����\��
			RenderName();

			// ��؂��������
			ImGui::Separator();

			// �R���|�[�l���g��\��
			selectObject->RenderComponent();

			// �R���|�[�l���g�ǉ��p�|�b�v�A�b�v
			AddComponentPopup();

			ImGui::End();
		}
	}

#pragma endregion

#pragma region AddComponent

	/// �����ł����g��Ȃ��̂ŁAcpp�݂̂ɏ���
	/// <summary>
	/// �|�b�v�A�b�v�W�J�p�{�^���̐ݒ�
	/// </summary>
	void PopupButtonSetting()
	{
		// �{�^����X�ʒu
		const float posX = 5.0f;

		// �{�^���̑傫��
		const ImVec2 size = { ImGui::GetWindowWidth() , 25.0f };

		// ���̂܂܂��ƈʒu������Ă��܂��̂ŁA
		// X�ʒu��ݒ�
		ImGui::SetCursorPosX(posX);
	}

	/// �����ł����g��Ȃ��̂ŁAcpp�݂̂ɏ���
	/// <summary>
	/// �|�b�v�A�b�v�̐ݒ�
	/// </summary>
	void PopupSetting()
	{
		// �|�b�v�A�b�v�̕�
		// �E�B���h�E���ɍ��킹��
		const float width = ImGui::GetWindowWidth();

		// �|�b�v�A�b�v�̑傫����ݒ�
		// �����͕\�����鐔�ɉ����ĕς��悤�ɂ��邽�߂ɁA0�ɐݒ肷��
		ImGui::SetNextWindowSize(ImVec2(width, 0));

		// �|�b�v�A�b�v�̈ʒu
		// �|�b�v�A�b�v�W�J�p�{�^���̉��ɕ\������
		const ImVec2 pos = { ImGui::GetItemRectMin().x, ImGui::GetItemRectMax().y };

		// �|�b�v�A�b�v�̈ʒu��ݒ�
		ImGui::SetNextWindowPos(pos);
	}

	/// <summary>
	/// �R���|�[�l���g�ǉ��p�|�b�v�A�b�v�̏���
	/// </summary>
	void Inspector::AddComponentPopup()
	{
		// �|�b�v�A�b�v��
		static const char* popupName = "AddComponent Popup";

		// �|�b�v�A�b�v�W�J�p�{�^���̐ݒ�
		PopupButtonSetting();

		// �|�b�v�A�b�v�W�J�p�{�^��
		if (ImGui::Button("Add Component"))
		{
			ImGui::OpenPopup(popupName);
		}

		// �|�b�v�A�b�v�̐ݒ�
		PopupSetting();

		// �|�b�v�A�b�v�W�J��
		// �R���|�[�l���g�ǉ��p���X�g��\��
		if (ImGui::BeginPopup(popupName, ImGuiWindowFlags_::ImGuiWindowFlags_NoMove))
		{
			ComponentAdder::RenderList(selectObject);

			ImGui::EndPopup();
		}
	}

#pragma endregion

} // namespace PokarinEngine