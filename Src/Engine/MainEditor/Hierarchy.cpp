/**
* @file Hierarchy.cpp
*/
#include "Hierarchy.h"

#include "ImGui/imgui.h"

#include "../Engine.h"
#include "../GameObject.h"
#include "../Scene.h"

#include "../InputManager.h"

#include "../Settings/MeshSettings.h"

namespace PokarinEngine
{
#pragma region CreateGameObject

	/// <summary>
	/// �Q�[���I�u�W�F�N�g�쐬�p�{�^���̏���
	/// </summary>
	/// <param name="kindName"> �쐬����Q�[���I�u�W�F�N�g�̎�� </param>
	/// <param name="staticMeshFile"> �X�^�e�B�b�N���b�V���̃t�@�C���� </param>
	void Hierarchy::CreateObjectButton(
		const char* typeName, const char* staticMeshFile)
	{
		// �{�^���̐F�𖳐F�ɐݒ�
		ImGui::PushStyleColor(ImGuiCol_::ImGuiCol_Button, ImVec4(0, 0, 0, 0));

		// �I�u�W�F�N�g�쐬�p�{�^��
		if (ImGui::Button(typeName))
		{
			// �{�^���������ꂽ�̂�
			// ���݂̃V�[���ɃI�u�W�F�N�g���쐬
			GameObjectPtr object = engine->GetCurrentScene().CreateGameObject(
				typeName, Vec3(0), Vec3(0), staticMeshFile);
		}

		// �{�^���F�̐ݒ���I��
		ImGui::PopStyleColor();
	}

	/// <summary>
	/// �Q�[���I�u�W�F�N�g�쐬�p�|�b�v�A�b�v��W�J����
	/// </summary>
	void Hierarchy::CreateObjectPopup_Open()
	{
		ImGui::OpenPopup("CreateObject");
	}

	/// <summary>
	/// �Q�[���I�u�W�F�N�g�쐬�p�|�b�v�A�b�v��W�J�������̏���
	/// </summary>
	void Hierarchy::CreateObjectPopup_Run()
	{
		// �Q�[���I�u�W�F�N�g�쐬�p�|�b�v�A�b�v
		if (ImGui::BeginPopup("CreateObject"))
		{
			// 3D�I�u�W�F�N�g�쐬�p���j���[
			if (ImGui::BeginMenu("3D Object"))
			{
				// ���̐����p�{�^��
				CreateObjectButton("Sphere", StaticMeshFile_OBJ::sphere);

				// ���{�b�g�����p�{�^��
				CreateObjectButton("Robot", StaticMeshFile_OBJ::robot);

				ImGui::EndMenu();
			}

			ImGui::EndPopup();
		}
	}

#pragma endregion

#pragma region Event

	/// <summary>
	/// ������
	/// </summary>
	void Hierarchy::Initialize(Engine& e)
	{
		engine = &e;
	}

	/// <summary>
	/// �X�V
	/// </summary>
	void Hierarchy::Update()
	{
		// ���݂̃V�[��
		Scene& currentScene = engine->GetCurrentScene();

		// �V�[�����̃Q�[���I�u�W�F�N�g
		GameObjectList gameObjectList = currentScene.GetGameObjectAll();

		// �I�𒆂̃I�u�W�F�N�g�ɑ΂��鑀����\�ɂ���
		ScelectObjectControl();

		// �q�G�����L�[�E�B���h�E
		ImGui::Begin("Hierarchy", NULL,
			ImGuiWindowFlags_::ImGuiWindowFlags_MenuBar);
		{
			// �E�B���h�E���ŉE�N���b�N��������
			// �I�u�W�F�N�g�쐬�p�|�b�v�A�b�v��W�J����
			if (ImGui::IsWindowHovered() &&
				Input::GetKeyUp(KeyCode::MouseRight))
			{
				CreateObjectPopup_Open();
			}

			// �I�u�W�F�N�g�쐬�p�|�b�v�A�b�v��
			// �W�J�������̏���
			CreateObjectPopup_Run();

			// ���j���[�\��
			Mene();

			// �V�[�����̃I�u�W�F�N�g�c���[
			ObjectTree();

			// �q�G�����L�[�E�B���h�E���I��
			ImGui::End();
		}
	}

#pragma endregion

#pragma region HierarchyWindow

	/// <summary>
	/// ���j���[�̏���
	/// </summary>
	void Hierarchy::Mene()
	{
		// ���j���[�o�[�̒���
		// �Q�[���I�u�W�F�N�g�쐬�p�{�^����z�u����
		ImGui::BeginMenuBar();
		{
			// �{�^���̐F��ݒ�
			ImGui::PushStyleColor(ImGuiCol_::ImGuiCol_Button, ImVec4(0, 0, 0, 0));

			// �Q�[���I�u�W�F�N�g�쐬�|�b�v�A�b�v�p�{�^��
			if (ImGui::Button("Object +"))
			{
				// �{�^��������������
				// �Q�[���I�u�W�F�N�g�쐬�p�|�b�v�A�b�v��W�J����
				CreateObjectPopup_Open();
			}

			// �{�^���̐F�ݒ���I��
			ImGui::PopStyleColor();

			// �Q�[���I�u�W�F�N�g�쐬�p�|�b�v�A�b�v
			CreateObjectPopup_Run();

			ImGui::EndMenuBar();
		}
	}

	/// <summary>
	/// �Q�[���I�u�W�F�N�g�\���p�c���[�m�[�h�̏���
	/// </summary>
	void Hierarchy::ObjectTree()
	{
		// ���݂̃V�[��
		Scene& currentScene = engine->GetCurrentScene();

		// �V�[�����̃Q�[���I�u�W�F�N�g
		GameObjectList gameObjectList = currentScene.GetGameObjectAll();

		// �c���[�m�[�h�̋@�\
		ImGuiTreeNodeFlags treeFlags = ImGuiTreeNodeFlags_::ImGuiTreeNodeFlags_DefaultOpen;

		// �V�[�����̃I�u�W�F�N�g��\�����邽�߂̃c���[�m�[�h
		if (ImGui::TreeNodeEx(currentScene.GetName(), treeFlags))
		{
			// �V�[�����̃I�u�W�F�N�g��\��
			for (int i = 0; i < gameObjectList.size(); ++i)
			{
				// �Q�[���I�u�W�F�N�g
				const GameObject& gameObject = *gameObjectList[i];

				// �Q�[���I�u�W�F�N�g�̖��O
				std::string objectName = gameObject.GetName();

				// �d�����Ȃ��悤�Ɏ��ʔԍ���ǉ�
				// ##�Ŕ�\���ɂ���
				objectName += "##" + std::to_string(gameObject.GetID());

				// �I���\�ȍ��ڂƂ��ĕ\��
				// �I�𒆂Ȃ狭���\������悤�Ɉ����Őݒ�
				if (ImGui::Selectable(objectName.c_str(), selectObjectIndex == i))
				{
					// �I�𒆃I�u�W�F�N�g�̗v�f�ԍ���ݒ�
					selectObjectIndex = i;

					// �I�𒆂̃I�u�W�F�N�g��ݒ�
					selectObject = gameObjectList[i];
				}

				// �I�u�W�F�N�g�̍��ڂ��_�u���N���b�N������
				// �E�B���h�E��\��
				if (ImGui::IsItemHovered() &&
					Input::Mouse::IsDoubleClick(MouseButton::Left))
				{
					// �e�X�g�p��
					// �_�u���N���b�N�����I�u�W�F�N�g�̖��O���^�C�g���ɂ���
					Window::OpenWindow(WindowID::NodeScript, gameObject.GetName());
					gameObject.OpenNodeEditor();
				}
			}

			ImGui::TreePop();
		}
	}

	/// <summary>
	/// �I�𒆂̃I�u�W�F�N�g�ɑ΂��鐧����܂Ƃ߂��֐�
	/// </summary>
	void Hierarchy::ScelectObjectControl()
	{
		// �I�𒆂̃I�u�W�F�N�g�������ꍇ�͉������Ȃ�
		if (!selectObject)
		{
			return;
		}

		// ���݂̃V�[��
		Scene& currentScene = engine->GetCurrentScene();

		// ------------------------------------------------------
		// �uCtrl + D�v�őI�𒆂̃I�u�W�F�N�g�𕡐�����
		// ------------------------------------------------------

		if (Input::GetKey(KeyCode::LeftCtrl) &&
			Input::GetKeyDown(KeyCode::D))
		{
			// �I�𒆂̃I�u�W�F�N�g�𕡐�����
			currentScene.CopyGameObject(selectObject);
		}

		// -------------------------------------------------------
		// �uDelete�v�őI�𒆂̃I�u�W�F�N�g���폜����
		// -------------------------------------------------------

		if (Input::GetKey(KeyCode::Delete))
		{
			// �I�𒆂̃I�u�W�F�N�g���폜
			currentScene.DestroyObject(selectObject);

			// ��I����Ԃɂ���
			selectObjectIndex = unselected;
			selectObject.reset();
		}
	}

#pragma endregion

} // namespace PokarinEngine
