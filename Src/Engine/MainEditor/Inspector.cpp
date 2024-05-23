/**
* @file Inspector.cpp
*/
#include "Inspector.h"

#include "../Engine.h"
#include "../Color.h"

#include "../Components/Rigidbody.h"

namespace PokarinEngine
{
	/// <summary>
	/// �R���|�[�l���g�ǉ��p
	/// </summary>
	namespace AddComponent
	{
		// -----------------------------
		// �ϐ�
		// -----------------------------

		// �|�b�v�A�b�v�̖��O
		const char* popupName = "AddComponent";

		// ----------------------------------
		// �֐�
		// ----------------------------------

		/// <summary>
		/// �R���|�[�l���g�ǉ��p�{�^��
		/// </summary>
		/// <typeparam name="T">  </typeparam>
		template<class T>
		void AddButton(GameObjectPtr object)
		{

		}

		/// <summary>
		/// �|�b�v�A�b�v�W�J�p�{�^��
		/// </summary>
		/// <returns>
		/// <para> true : �W�J�� </para>
		/// <para> false : �W�J���Ă��Ȃ� </para>
		/// </returns>
		bool OpenPopupButton()
		{
			static bool isOpen = false;

			// �{�^����X�ʒu
			const float posX = 5.0f;

			// �{�^���̑傫��
			const ImVec2 size = { ImGui::GetWindowWidth() , 25.0f };

			// ���̂܂܂��ƈʒu������Ă��܂��̂ŁA
			// X�ʒu��ݒ�
			ImGui::SetCursorPosX(posX);

			// �R���|�[�l���g�ǉ��p�{�^��
			// �����ꂽ��|�b�v�A�b�v�W�J
			if (ImGui::Button("Add Component", size))
			{
				//ImGui::OpenPopup(popupName);
				isOpen = true;
			}

			return isOpen;
		}

		/// <summary>
		/// �|�b�v�A�b�v�W�J���̏���
		/// </summary>
		/// <param name="object"> �Q�[���I�u�W�F�N�g </param>
		void DownMenu(GameObjectPtr object)
		{

			if (ImGui::CollapsingHeader("Add Component", ImGuiTreeNodeFlags_Bullet))
			{
				ImGui::Text("Select Component");
			}

			//AddButton<Rigidbody>(object);
		}
	}

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
					selectObject->GetOwnerScene().ChangeObjectName(
						selectObject, selectObjectName.substr(0, endLine));
				}
			}
		}

		/// <summary>
		/// �X�V
		/// </summary>
		/// <param name="selectObject"> �q�G�����L�[�őI�𒆂̃I�u�W�F�N�g </param>
		void Update(GameObjectPtr selectObject)
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
				for (auto& component : selectObject->componentList)
				{
					// �\������R���|�[�l���g��
					// ��ʂł���悤�ɁA�I�u�W�F�N�g�̎��ʔԍ���t���Ă���(��\��)
					const std::string componentName = component->GetName() + "##" + std::to_string(selectObject->GetID());

					// �܂��݉\�ȃw�b�_�[�ŕ\��
					if (ImGui::CollapsingHeader(componentName.c_str(), ImGuiTreeNodeFlags_DefaultOpen))
					{
						// �w�b�_�[��W�J���ɏ��̕ҏW���ł���悤�ɕ\������
						component->InfoEditor();
					}
				}

				//AddComponent::DownMenu(selectObject);

				ImGui::End();
			}
		}
	}

} // namespace PokarinEngine