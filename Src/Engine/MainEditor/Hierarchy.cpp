/**
* @file Hierarchy.cpp
*/
#include "Hierarchy.h"

#include "ImGui/imgui.h"

#include "../Engine.h"
#include "../Scene.h"

#include "../InputManager.h"

#include "../Configs/MeshConfig.h"

namespace PokarinEngine
{
	/// <summary>
	/// �ϐ�
	/// </summary>
	namespace
	{
		// ��I�����̗v�f�ԍ�
		const int unselected = -1;

		// �I�𒆃I�u�W�F�N�g�̗v�f�ԍ�
		int selectObjectIndex = unselected;

		// �I�𒆂̃I�u�W�F�N�g
		GameObjectPtr selectObject;

		// �G���W���N���X�̃|�C���^
		Engine* engine = nullptr;
	}

	/// <summary>
	/// �I�u�W�F�N�g�쐬�p
	/// </summary>
	namespace
	{
		/// <summary>
		/// �I�u�W�F�N�g�쐬�p�N���X
		/// </summary>
		class CreateObject
		{
		public: // -------------------- �|�b�v�A�b�v���� --------------------

			/// <summary>
			/// �Q�[���I�u�W�F�N�g�쐬�p�|�b�v�A�b�v���J��
			/// </summary>
			void OpenPopup()
			{
				ImGui::OpenPopup(popupName);
			}

			/// <summary>
			/// �Q�[���I�u�W�F�N�g�쐬�p�|�b�v�A�b�v�̏���
			/// </summary>
			void Popup()
			{
				// -----------------------------------
				// �|�b�v�A�b�v�J�n
				// -----------------------------------

				// �J���Ă��Ȃ���Ή��������ɏI��
				if (!ImGui::BeginPopup(popupName))
				{
					return;
				}

				// -----------------------------------
				// �|�b�v�A�b�v���̏���
				// -----------------------------------

				// 3D�I�u�W�F�N�g�쐬�p���j���[
				if (ImGui::BeginMenu("3D Object"))
				{
					// ���̐����p�{�^��
					Button("Sphere", StaticMeshFile_OBJ::sphere);

					// ���{�b�g�����p�{�^��
					Button("Robot", StaticMeshFile_OBJ::robot);

					ImGui::EndMenu();
				}

				// --------------------------------
				// �|�b�v�A�b�v�̏I��
				// --------------------------------

				ImGui::EndPopup();
			}

		private: // --------------------- �쐬�p�{�^�� ----------------------

			/// <summary>
			/// �Q�[���I�u�W�F�N�g�쐬�p�{�^���̏���
			/// </summary>
			/// <param name="kindName"> �쐬����Q�[���I�u�W�F�N�g�̎�� </param>
			/// <param name="staticMeshFile"> �X�^�e�B�b�N���b�V���̃t�@�C���� </param>
			void Button(const char* typeName, const char* staticMeshFile)
			{
				// �{�^���̐F�𖳐F�ɐݒ�
				ImGui::PushStyleColor(ImGuiCol_::ImGuiCol_Button, ImVec4(0, 0, 0, 0));

				// �I�u�W�F�N�g�쐬�p�{�^��
				if (ImGui::Button(typeName))
				{
					// �{�^���������ꂽ�̂�
					// ���݂̃V�[���ɃI�u�W�F�N�g���쐬
					GameObjectPtr object = engine->GetCurrentScene().CreateGameObject(
						typeName, Vector3(0), Vector3(0), staticMeshFile);
				}

				// �{�^���F�̐ݒ���I��
				ImGui::PopStyleColor();
			}

		private: // --------------------- �|�b�v�A�b�v ----------------------

			// �I�u�W�F�N�g�쐬�p�|�b�v�A�b�v�̖��O
			const char* popupName = "CreateObject";
		};

		// �I�u�W�F�N�g�쐬�p
		CreateObject createObject;
	}

	/// <summary>
	/// �q�G�����L�[(�V�[�����̃I�u�W�F�N�g�Ǘ��p�E�B���h�E)
	/// </summary>
	namespace Hierarchy
	{
		/// �����ł����g��Ȃ��̂ŁAcpp�݂̂ɏ���
		/// <summary>
		/// �q�G�����L�[�E�B���h�E���ł̑���
		/// </summary>
		void HierarchyControl()
		{
			// �E�B���h�E���ŉE�N���b�N��������
			// �I�u�W�F�N�g�쐬�p�|�b�v�A�b�v��W�J����
			if (ImGui::IsWindowHovered() &&
				Input::GetKeyUp(KeyCode::MouseRight))
			{
				createObject.OpenPopup();
			}
		}

		/// �����ł����g��Ȃ��̂ŁAcpp�݂̂ɏ���
		/// <summary>
		/// ���j���[�̏���
		/// </summary>
		void Mene()
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
					createObject.OpenPopup();
				}

				// �{�^���̐F�ݒ���I��
				ImGui::PopStyleColor();

				// �Q�[���I�u�W�F�N�g�쐬�p�|�b�v�A�b�v�̏���
				createObject.Popup();

				ImGui::EndMenuBar();
			}
		}

		/// �����ł����g��Ȃ��̂ŁAcpp�݂̂ɏ���
		/// <summary>
		/// �Q�[���I�u�W�F�N�g�\���p�c���[�m�[�h�̏���
		/// </summary>
		void ObjectTree()
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
						Window::OpenWindow(WindowID::NodeScript, "NodeScript");
						gameObject.OpenNodeEditor();
					}
				}

				ImGui::TreePop();
			}
		}

		/// �����ł����g��Ȃ��̂ŁAcpp�݂̂ɏ���
		/// <summary>
		/// �I�𒆂̃I�u�W�F�N�g�ɑ΂��鑀����܂Ƃ߂��֐�
		/// </summary>
		void ScelectObjectControl()
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

		/// <summary>
		/// ������
		/// </summary>
		void Initialize(Engine& e)
		{
			engine = &e;
		}

		/// <summary>
		/// �X�V
		/// </summary>
		void Update()
		{
			// ���݂̃V�[��
			Scene& currentScene = engine->GetCurrentScene();

			// �V�[�����̃Q�[���I�u�W�F�N�g
			GameObjectList gameObjectList = currentScene.GetGameObjectAll();

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
				createObject.Popup();

				// �V�[�����̃I�u�W�F�N�g�c���[
				ObjectTree();

				// �q�G�����L�[�E�B���h�E���I��
				ImGui::End();
			}
		}

		/// <summary>
		/// �I�𒆂̃I�u�W�F�N�g�̗v�f�ԍ����擾����
		/// </summary>
		/// <returns> �E�B���h�E���őI�𒆂̃I�u�W�F�N�g�̗v�f�ԍ� </returns>
		GameObjectPtr GetSelectObject()
		{
			return selectObject;
		}

	} // namespace Hierarchy

} // namespace PokarinEngine
