/**
* @file Component.cpp
*/
#include "Component.h"

#include "IconFont/IconFont.h"

#include "../GameObject.h"
#include "../Scene.h"

#include "Json/Json.h"

namespace PokarinEngine
{
	/// <summary>
	/// �R���|�[�l���g����p�|�b�v�A�b�v�̏���
	/// </summary>
	void Component::ControlPopup()
	{
		// transform�͍폜���Ȃ��悤�ɂ���
		// ����폜�����Ȃ��̂ŉ������Ȃ�
		if (ownerObject->transform.get() == this)
		{
			return;
		}

		// ���C���J�����͌���폜���Ȃ��悤�ɂ���
		if (&ownerObject->GetOwnerScene().GetMainCameraInfo() == this)
		{
			return;
		}

		// �|�b�v�A�b�v�W�J�p�{�^�����w�b�_�[�̉E�[�ɕ\��
		const float posX = ImGui::GetItemRectSize().x - 20.0f;
		ImGui::SameLine(posX);

		// �|�b�v�A�b�v��
		// ��ʗp�Ɏ��ʔԍ���ǉ�
		const std::string popupName = "ComponentControl##" + std::to_string(id);

		// �|�b�v�A�b�v�W�J�p�{�^���̖��O
		// ���Ԃ̃A�C�R����\��(��ʗp�Ɏ��ʔԍ���ǉ�)
		const std::string popupButton = ICON_FA_GEAR"##" + std::to_string(id);

		// �F���d�Ȃ��Ėڗ��̂ŁA�{�^���̔w�i�F�𓧖��ɂ���
		ImGui::PushStyleColor(ImGuiCol_::ImGuiCol_Button, ImVec4(0, 0, 0, 0));

		// �|�b�v�A�b�v�W�J�p�{�^��
		// ���Ԃŕ\��
		if (ImGui::Button(popupButton.c_str()))
		{
			ImGui::OpenPopup(popupName.c_str());
		}

		// �{�^���̐F�ݒ���I��
		ImGui::PopStyleColor();

		// �|�b�v�A�b�v�W�J���Ƀ{�^����\������
		if (ImGui::BeginPopup(popupName.c_str()))
		{
			// �R���|�[�l���g�폜�p�{�^��
			if (ImGui::Button("Delete Component"))
			{
				Destroy();
			}

			ImGui::EndPopup();
		}
	}

	/// <summary>
	/// �G�f�B�^�ɏ���\������
	/// </summary>
	void Component::RenderInfo()
	{
		// �܂��݉\�ȃw�b�_�[�ŕ\��
		bool isOpen = ImGui::CollapsingHeader(title.c_str(),
			ImGuiTreeNodeFlags_AllowItemOverlap | ImGuiTreeNodeFlags_DefaultOpen);

		// ����p�|�b�v�A�b�v
		ControlPopup();

		// �w�b�_�[�W�J���ɃR���|�[�l���g�̏���\������
		if (isOpen)
		{
			InfoEditor();
		}
	}

	/// <summary>
	/// �R���|�[�l���g�̏���ۑ�����
	/// </summary>
	/// <param name="[in] folderName"> �t�H���_�� </param>
	void Component::SaveComponent(const std::string& folderName) const
	{
		Json data;

		// ���O��ۑ�����
		data["Name"] = name;

		// �e�R���|�[�l���g�̏���ۑ�����
		SaveInfo(folderName);
	}
}