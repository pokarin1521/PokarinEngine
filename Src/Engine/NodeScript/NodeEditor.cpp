/**
* @file NodeEditor.cpp
*/
#include "NodeEditor.h"

#include "ImGui/imgui.h"
#include "ImGui/imnodes.h"

#include "../GameObject.h"

#include "NodeScript.h"

namespace PokarinEngine
{
	/// <summary>
	/// �R���X�g���N�^
	/// </summary>
	/// <param name="object"> ������ɂȂ�Q�[���I�u�W�F�N�g </param>
	NodeEditor::NodeEditor(GameObject* ownerObject)
	{
		// �������ݒ�
		owner = ownerObject;

		// �m�[�h�G�f�B�^�̖��O��ݒ�
		// ������₷���悤�Ɂu������̖��O + #������̎��ʔԍ��v�ɂ���
		name = owner->GetName();
		name += " #" + std::to_string(owner->GetID());
	}

	/// <summary>
	/// �X�V
	/// </summary>
	/// <returns>
	/// <para> true : �m�[�h�G�f�B�^�̃E�B���h�E���I������Ă��� </para>
	/// <para> false : �m�[�h�G�f�B�^�̃E�B���h�E���I������Ă��Ȃ� </para>
	/// </returns>
	bool NodeEditor::Update()
	{
		// �E�B���h�E���J���ĂȂ��Ȃ牽�����Ȃ�
		if (!isOpen)
		{
			return false;
		}

		// ImGui�E�B���h�E���쐬
		ImGui::Begin(name.c_str(), &isOpen);
		{
			// ImGui�E�B���h�E��o�^
			if (!imGuiWindow)
			{
				imGuiWindow = ImGui::GetCurrentWindow();
			}

			// �t�H�[�J�X����ĂȂ��Ȃ璆�f
			if (!ImGui::IsWindowFocused())
			{
				ImGui::End();
				return false;
			}

			// �m�[�h�G�f�B�^���쐬
			ImNodes::BeginNodeEditor();
			{
				ImNodes::EndNodeEditor();
			}

			ImGui::End();
		}

		return true;
	}

} // namespace PokarinEngine