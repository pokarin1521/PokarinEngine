/**
* @file Hierarchy.cpp
*/
#include "Hierarchy.h"

#include "ImGui/imgui.h"

#include "../Scene.h"
#include "../GameObject.h"

#include "../Input.h"

#include "../Mesh/Mesh.h"

#include "../Configs/MeshConfig.h"

namespace PokarinEngine
{
#pragma region Hierarchy

	/// <summary>
	/// �X�V
	/// </summary>
	/// <param name="currentScene"> ���݂̃V�[�� </param>
	void Hierarchy::Update(const ScenePtr& currentScene)
	{
		// �V�[�����̃Q�[���I�u�W�F�N�g
		GameObjectList gameObjectList = currentScene->GetGameObjectAll();

		// �I�𒆂̃I�u�W�F�N�g�ɑ΂��鑀����\�ɂ���
		ScelectObjectControl();

		// �q�G�����L�[�E�B���h�E
		ImGui::Begin("Hierarchy", nullptr,
			ImGuiWindowFlags_::ImGuiWindowFlags_MenuBar);
		{
			// �q�G�����L�[�E�B���h�E���̑���
			HierarchyControl();

			// ���j���[�\��
			Mene();

			// �I�u�W�F�N�g�쐬�p�|�b�v�A�b�v�̏���
			CreateObjectPopup();

			// �V�[�����̃I�u�W�F�N�g�c���[
			ObjectTree();

			// �q�G�����L�[�E�B���h�E���I��
			ImGui::End();
		}
	}

	/// <summary>
	/// �q�G�����L�[�E�B���h�E���ł̑���
	/// </summary>
	void Hierarchy::HierarchyControl()
	{
		// �E�B���h�E���ŉE�N���b�N��������
		// �I�u�W�F�N�g�쐬�p�|�b�v�A�b�v��W�J����
		if (ImGui::IsWindowHovered() &&
			Input::GetKeyUp(KeyCode::MouseRight))
		{
			ImGui::OpenPopup(createObjectPopupName);
		}
	}

	/// <summary>
	/// ���j���[�̏���
	/// </summary>
	void Hierarchy::Mene()
	{
		// ���j���[�o�[�̒���
		// �Q�[���I�u�W�F�N�g�쐬�p�{�^����z�u����
		ImGui::BeginMenuBar();
		{
			// �Q�[���I�u�W�F�N�g�쐬�|�b�v�A�b�v�p�{�^��
			// ���F�ŕ\��
			if (ImGui::Button("Object +", ImVec4(0,0,0,0)))
			{
				// �{�^��������������
				// �Q�[���I�u�W�F�N�g�쐬�p�|�b�v�A�b�v��W�J����
				ImGui::OpenPopup(createObjectPopupName);
			}

			// �Q�[���I�u�W�F�N�g�쐬�p�|�b�v�A�b�v�̏���
			CreateObjectPopup();

			ImGui::EndMenuBar();
		}
	}

	/// <summary>
	/// �Q�[���I�u�W�F�N�g�\���p�c���[�m�[�h�̏���
	/// </summary>
	void Hierarchy::ObjectTree()
	{
		// �V�[�����̃Q�[���I�u�W�F�N�g
		GameObjectList gameObjectList = currentScene->GetGameObjectAll();

		// �c���[�m�[�h�̋@�\
		ImGuiTreeNodeFlags treeFlags = ImGuiTreeNodeFlags_::ImGuiTreeNodeFlags_DefaultOpen;

		// �V�[�����̃I�u�W�F�N�g��\�����邽�߂̃c���[�m�[�h
		if (ImGui::TreeNodeEx(currentScene->GetName(), treeFlags))
		{
			// �V�[�����̃I�u�W�F�N�g��\��
			for (int i = 0; i < gameObjectList.size(); ++i)
			{
				// �Q�[���I�u�W�F�N�g
				const GameObject& gameObject = *gameObjectList[i];

				// �Q�[���I�u�W�F�N�g�̖��O
				std::string objectName = gameObject.name;

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
					Window::OpenWindow(WindowID::NodeScript, "NodeScript");
					gameObject.OpenNodeEditor();
				}
			}

			ImGui::TreePop();
		}
	}

	/// <summary>
	/// �I�𒆂̃I�u�W�F�N�g�ɑ΂��鑀����܂Ƃ߂��֐�
	/// </summary>
	void Hierarchy::ScelectObjectControl()
	{
		// �I�𒆂̃I�u�W�F�N�g�������ꍇ�͉������Ȃ�
		if (!selectObject)
		{
			return;
		}

		// ------------------------------------------------------
		// �uCtrl + D�v�őI�𒆂̃I�u�W�F�N�g�𕡐�����
		// ------------------------------------------------------

		if (Input::GetKey(KeyCode::LeftCtrl) &&
			Input::GetKeyDown(KeyCode::D))
		{
			// �I�𒆂̃I�u�W�F�N�g�𕡐�����
			currentScene->CopyGameObject(selectObject);
		}

		// -------------------------------------------------------
		// �uDelete�v�őI�𒆂̃I�u�W�F�N�g���폜����
		// -------------------------------------------------------

		if (Input::GetKey(KeyCode::Delete))
		{
			// �I�𒆂̃I�u�W�F�N�g���폜
			currentScene->DestroyObject(selectObject);

			// ��I����Ԃɂ���
			selectObjectIndex = unselected;
			selectObject.reset();
		}
	}

	/// <summary>
	/// �I�𒆂̃I�u�W�F�N�g���擾����
	/// </summary>
	/// <returns> �E�B���h�E���őI�𒆂̃I�u�W�F�N�g </returns>
	GameObjectPtr& Hierarchy::GetSelectObject()
	{
		return selectObject;
	}

#pragma endregion

#pragma region CreateObject

	/// <summary>
	/// �Q�[���I�u�W�F�N�g�쐬�p�|�b�v�A�b�v�̏���
	/// </summary>
	void Hierarchy::CreateObjectPopup()
	{
		// -----------------------------------
		// �|�b�v�A�b�v�J�n
		// -----------------------------------

		// �J���Ă��Ȃ���Ή��������ɏI��
		if (!ImGui::BeginPopup(createObjectPopupName))
		{
			return;
		}

		// -----------------------------------
		// �|�b�v�A�b�v���̏���
		// -----------------------------------

		// 3D�I�u�W�F�N�g�쐬�p���j���[
		if (ImGui::BeginMenu("3D Object"))
		{
			// �����̐����p�{�^��
			CreateObjectButton("Cube", StaticMeshFile::cube);

			// ���̐����p�{�^��
			CreateObjectButton("Sphere", StaticMeshFile::sphere);

			// �����p�{�^��
			CreateObjectButton("Plane", StaticMeshFile::plane);

			// ���{�b�g�����p�{�^��
			CreateObjectButton("Robot", StaticMeshFile::robot);

			ImGui::EndMenu();
		}

		// --------------------------------
		// �|�b�v�A�b�v�̏I��
		// --------------------------------

		ImGui::EndPopup();
	}

	/// <summary>
	/// �Q�[���I�u�W�F�N�g�쐬�p�{�^���̏���
	/// </summary>
	/// <param name="[in] kindName"> �쐬����Q�[���I�u�W�F�N�g�̎�� </param>
	/// <param name="[in] staticMeshFile"> �X�^�e�B�b�N���b�V���̃t�@�C���� </param>
	void Hierarchy::CreateObjectButton(const char* typeName, const char* staticMeshFile)
	{
		// �I�u�W�F�N�g�쐬�p�{�^��
		if (ImGui::Button(typeName, Color::black))
		{
			// �{�^���������ꂽ�̂�
			// ���݂̃V�[���ɃI�u�W�F�N�g���쐬
			GameObjectPtr object = currentScene->CreateGameObject(
				typeName, Vector3(0), Vector3(0));

			// �X�^�e�B�b�N���b�V����ݒ�
			object->staticMesh = Mesh::GetStaticMesh(staticMeshFile);
		}
	}

#pragma endregion

} // namespace PokarinEngine
