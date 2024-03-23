/**
* @file Inspector.cpp
*/
#include "Inspector.h"

#include "../Engine.h"

namespace PokarinEngine
{
	/// <summary>
	/// ������
	/// </summary>
	void Inspector::Initialize(Engine& e)
	{
		engine = &e;
	}

	/// <summary>
	/// �X�V
	/// </summary>
	/// <param name="selectObject"> �q�G�����L�[�őI�𒆂̃I�u�W�F�N�g </param>
	void Inspector::Update(GameObjectPtr selectObject)
	{
		// ���݂̃V�[��
		Scene& currentScene = engine->GetCurrentScene();

		// �V�[�����̃Q�[���I�u�W�F�N�g
		GameObjectList gameObjectList = currentScene.GetGameObjectAll();

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

			// �I�𒆂̃I�u�W�F�N�g�̖��O���擾
			selectObjectName = selectObject->GetName();

			// ���O��������������悤��
			// �I�u�W�F�N�g���̍ő啶������ݒ�
			selectObjectName.resize(selectObject->GetNameSize());

			// �I�u�W�F�N�g������͂���e�L�X�g��
			// ##�Ńe�L�X�g���\������
			if (ImGui::InputText("##ObjectName", selectObjectName.data(), selectObject->GetNameSize(),
				ImGuiInputTextFlags_::ImGuiInputTextFlags_EnterReturnsTrue | 
				ImGuiInputTextFlags_::ImGuiInputTextFlags_None))
			{
				// �V�������O�����͂���Ă���
				if (selectObjectName[0] != '\0' &&
					selectObjectName != selectObject->GetName())
				{
					// �I�[�ʒu
					size_t endLine = selectObjectName.find_first_of('\0');

					// ���O��ύX
					// �]�v��\0������ƍ���̂ŁA�ŏ���\0�܂ł�n��
					currentScene.ChangeObjectName(selectObject, selectObjectName.substr(0, endLine));
				}
			}

			// ��؂��������
			ImGui::Separator();

			// �R���|�[�l���g��\��
			for (ComponentPtr component : selectObject->components)
			{
				component->RenderEditor();
			}

			ImGui::End();
		}
	}

} // namespace PokarinEngine