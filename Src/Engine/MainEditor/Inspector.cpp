/**
* @file Inspector.cpp
*/
#include "Inspector.h"

#include "../Engine.h"

namespace PokarinEngine
{
	/// <summary>
	/// �C���X�y�N�^�[(�V�[�����̃I�u�W�F�N�g����p�E�B���h�E)
	/// </summary>
	namespace Inspector
	{
		/// �����ł����g��Ȃ��̂ŁAcpp�݂̂ɏ���
		/// <summary>
		/// �I�u�W�F�N�g����\������
		/// </summary>
		/// <param name="selectObject"> �q�G�����L�[�őI�𒆂̃I�u�W�F�N�g </param>
		void RenderName(GameObjectPtr selectObject)
		{
			// �I�𒆃I�u�W�F�N�g�̖��O
			std::string selectObjectName = selectObject->GetName();

			// �I�u�W�F�N�g������͂���e�L�X�g��
				// ##�Ńe�L�X�g���\������
			if (ImGui::InputText("##ObjectName", selectObjectName.data(), selectObject->GetNameSize(),
				ImGuiInputTextFlags_::ImGuiInputTextFlags_EnterReturnsTrue))
			{
				// �V�������O�����͂���Ă���
				if (selectObjectName[0] != '\0' &&
					selectObjectName != selectObject->GetName())
				{
					// �I�[�ʒu
					size_t endLine = selectObjectName.find_first_of('\0');

					// ���O��ύX
					// �]�v�ȋ󔒂�����ƍ���̂ŁA�ŏ���\0�܂ł�n��
					selectObject->GetScene().ChangeObjectName(
						selectObject, selectObjectName.substr(0, endLine));
				}
			}
		}

		/// <summary>
		/// �X�V
		/// </summary>
		/// <param name="selectObject"> �q�G�����L�[�őI�𒆂̃I�u�W�F�N�g </param>
		void Inspector::Update(GameObjectPtr selectObject)
		{
			// �C���X�y�N�^�[�E�B���h�E
			ImGui::Begin("Inspector");
			{
				// �q�G�����L�[�ŃI�u�W�F�N�g��I�����Ă��Ȃ��Ȃ�
				// �����\�����Ȃ�
				if (!selectObject)
				{
					ImGui::End();
					return;
				}

				// �I�u�W�F�N�g����\��
				RenderName(selectObject);

				// ��؂��������
				ImGui::Separator();

				// �R���|�[�l���g��\��
				for (ComponentPtr component : selectObject->componentList)
				{
					component->RenderInfo();
				}

				ImGui::End();
			}
		}
	}

} // namespace PokarinEngine