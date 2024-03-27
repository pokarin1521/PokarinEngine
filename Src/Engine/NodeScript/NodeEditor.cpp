/**
* @file NodeEditor.cpp
*/
#include "NodeEditor.h"

#include "ImGui/imgui.h"

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

		nodeEditorContext = ImNodes::EditorContextCreate();
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
		// �E�B���h�E���I�����ꂽ���ɏ�������
		if(ImGui::Begin(name.c_str(), &isOpen))
		{
			// ImGui�E�B���h�E��o�^
			if (!imGuiWindow)
			{
				imGuiWindow = ImGui::GetCurrentWindow();
			}

			ImNodes::EditorContextSet(nodeEditorContext);

			// �m�[�h�G�f�B�^���쐬
			ImNodes::BeginNodeEditor();
			{
				ImNodes::EndNodeEditor();
			}

			ImNodes::EditorContextSet(nullptr);

			ImGui::End();

			// �I������Ă���̂�true��Ԃ�
			return true;
		}


		// �I����ԂɊւ�炸Begin�֐����Ă΂�Ă���̂�
		// End�֐����Ă�
		ImGui::End();

		// �I������ĂȂ��̂�false��Ԃ�
		return false;
	}

} // namespace PokarinEngine