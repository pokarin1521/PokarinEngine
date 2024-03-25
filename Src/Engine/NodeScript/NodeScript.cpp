/**
* @file NodeScript.cpp
*/
#include "NodeScript.h"

#include "ImGui/imgui.h"
#include "ImGui/imgui_internal.h"
#include "ImGui/imgui_impl_glfw.h"
#include "ImGui/imgui_impl_opengl3.h"
#include "ImGui/imnodes.h"

#include "NodeEditor.h"

#include "../Window.h"
#include "../InputManager.h"

#include "../Color.h"

#include <GLFW/glfw3.h>
#include <unordered_map>
#include <unordered_set>
#include <vector>

namespace PokarinEngine
{
	/// <summary>
	/// �m�[�h�G�f�B�^���Ǘ�����@�\
	/// </summary>
	namespace NodeScript
	{
		// ---------------------------------
		// �^�̕ʖ����`
		// ---------------------------------

		using OpenEditorList = std::unordered_set<NodeEditorPtr>;
		using ClosedEditorList = std::vector<NodeEditorPtr>;

		// ---------------------------------------
		// �ϐ�
		// ---------------------------------------

		// ImGui�p�R���e�L�X�g
		ImGuiContext* imGuiContext = nullptr;

		// �J���Ă���m�[�h�G�f�B�^�z��
		OpenEditorList openEditorList;

		// ���Ă���m�[�h�G�f�B�^�z��
		NodeEditorPtr closedEditor;

		// �I�𒆂̃m�[�h�G�f�B�^
		NodeEditorPtr selectEditor;

		// ImGui�̐F�ݒ�̉�
		int pushColorCount = 0;

		// -------------------------------------------
		// �֐�
		// -------------------------------------------

		/// �����ł����g��Ȃ��̂ŁAcpp�݂̂ɏ���
		/// <summary>
		/// ImGui�̐F�ݒ���J�n����
		/// </summary>
		/// <param name="style"> �F��ݒ肵�������� </param>
		/// <param name="styleColor"> �ݒ肷��F </param>
		void PushStyleColor(ImGuiCol style, const Color& styleColor)
		{
			ImGui::PushStyleColor(style, styleColor);
			pushColorCount++;
		}

		/// �����ł����g��Ȃ��̂ŁAcpp�݂̂ɏ���
		/// <summary>
		/// ImGui�̐F�ݒ���I������
		/// </summary>
		void PopStyleColor()
		{
			ImGui::PopStyleColor(pushColorCount);
			pushColorCount = 0;
		}

		/// �����ł����g��Ȃ��̂ŁAcpp�݂̂ɏ���
		/// <summary>
		/// ���Ă���m�[�h�G�f�B�^��z�񂩂�폜����
		/// </summary>
		void EraseClosedEditor()
		{
			// ���Ă���m�[�h�G�f�B�^��z�񂩂�폜
			openEditorList.erase(closedEditor);

			// �J���Ă���m�[�h�G�f�B�^���Ȃ��Ȃ����ꍇ
			if (openEditorList.empty())
			{
				// ���L�������
				selectEditor.reset();

				// �\������Ӗ����Ȃ��̂�
				// �E�B���h�E�����
				Window::CloseWindow(WindowID::NodeScript);
			}

			// �I�𒆂̃m�[�h�G�f�B�^������ꂽ�ꍇ
			if (closedEditor == selectEditor)
			{
				// �擪�̃m�[�h�G�f�B�^��I�𒆂ɂ���
				selectEditor = *openEditorList.begin();
			}

			if (selectEditor)
			{
				// �I�𒆃m�[�h�G�f�B�^�̃E�B���h�E���t�H�[�J�X����
				imGuiContext->NavWindow = &selectEditor->GetImGuiWindow();
			}

			// �폜�����m�[�h�G�f�B�^�̏��L�������
			closedEditor.reset();
		}

		/// <summary>
		/// ������
		/// </summary>
		void Initialize()
		{
			// ---------------------------
			// �R���e�L�X�g�쐬
			// ---------------------------

			// �m�[�h�X�N���v�g�p�̃E�B���h�E���g�p����悤�ɐݒ�
			glfwMakeContextCurrent(&Window::GetWindow(WindowID::NodeScript));

			// ImGui�̃o�[�W�������m�F
			IMGUI_CHECKVERSION();

			// ImGui�R���e�L�X�g�쐬
			imGuiContext = ImGui::CreateContext();

			// �쐬�����R���e�L�X�g���g�p����p�ɐݒ�
			ImGui::SetCurrentContext(imGuiContext);

			// ImNodes�R���e�L�X�g���쐬
			ImNodes::CreateContext();

			// ---------------------------------------
			// �h�b�L���O�E�B���h�E�̗L����
			// ---------------------------------------

			ImGuiIO& io = ImGui::GetIO();
			io.ConfigFlags |= ImGuiConfigFlags_::ImGuiConfigFlags_DockingEnable;

			// ------------------------
			// ImGui�̏�����
			// ------------------------

			// GLFW
			ImGui_ImplGlfw_InitForOpenGL(&Window::GetWindow(WindowID::NodeScript), true);

			// GLSL�̃o�[�W�������w��
			ImGui_ImplOpenGL3_Init("#version 450");
		}

		/// <summary>
		/// �X�V
		/// </summary>
		void Update()
		{
			// ----------------------------------------
			// ImGui�̍X�V
			// ----------------------------------------

			// �g�p����R���e�L�X�g��ݒ�
			ImGui::SetCurrentContext(imGuiContext);

			// ImGui�̍X�V
			ImGui_ImplGlfw_NewFrame();
			ImGui_ImplOpenGL3_NewFrame();
			ImGui::NewFrame();

			// ------------------------------------
			// ImGui�̐F�ݒ�
			// ------------------------------------

			PushStyleColor(ImGuiCol_::ImGuiCol_Tab, BasicColor::gray);
			PushStyleColor(ImGuiCol_::ImGuiCol_TitleBgActive, BasicColor::gray);

			// -----------------------------------------------------------------------
			// �E�B���h�E�S�̂ŃE�B���h�E���h�b�L���O�o����悤�ɂ���
			// -----------------------------------------------------------------------

			// �h�b�L���O�̋@�\
			ImGuiDockNodeFlags dockFlags = ImGuiDockNodeFlags_::ImGuiDockNodeFlags_NoSplit |
				ImGuiDockNodeFlags_::ImGuiDockNodeFlags_NoUndocking |
				ImGuiDockNodeFlagsPrivate_::ImGuiDockNodeFlags_NoWindowMenuButton |
				ImGuiDockNodeFlagsPrivate_::ImGuiDockNodeFlags_NoCloseButton;

			// �h�b�L���O�X�y�[�X�̎��ʔԍ�
			ImGuiID dockSpaceID = ImGui::DockSpaceOverViewport(ImGui::GetMainViewport(), dockFlags);

			/* �����ȉ��ō쐬�����ImGui�E�B���h�E���h�b�L���O�\�ɂȂ� */

			// ------------------------------------
			// �m�[�h�G�f�B�^���X�V
			// ------------------------------------

			// �J���Ă���m�[�h�G�f�B�^
			for (auto& nodeEditor : openEditorList)
			{
				// ImGui�E�B���h�E���ŏ�����h�b�L���O�����悤�ɐݒ�
				ImGui::DockBuilderDockWindow(nodeEditor->GetName(), dockSpaceID);


				// �m�[�h�G�f�B�^�̍X�V
				if (nodeEditor->Update())
				{
					// �I�𒆂̃m�[�h�G�f�B�^��ݒ�
					selectEditor = nodeEditor;
				}

				// �m�[�h�G�f�B�^������ꂽ��
				// ��ō폜�ł���悤�ɕϐ��ɓ����
				if (!nodeEditor->IsOpen())
				{
					closedEditor = nodeEditor;
				}
			}

			// ----------------------------------------------------------
			// ���Ă���m�[�h�G�f�B�^������̂Ȃ�폜����
			// ----------------------------------------------------------

			if (closedEditor)
			{
				EraseClosedEditor();
			}

			// -------------------------------------
			// ImGui�̐F�ݒ���I������
			// -------------------------------------

			PopStyleColor();

			// ----------------------------------------
			// �J���[�o�b�t�@���N���A����
			// ----------------------------------------

			// �J���[�o�b�t�@���N���A����Ƃ��̐F��ݒ�
			glClearColor(0, 0, 0, 0);

			// �o�b�N�o�b�t�@���N���A
			// ����̓J���[�o�b�t�@���w��
			glClear(GL_COLOR_BUFFER_BIT);
		}

		/// <summary>
		/// �`��
		/// </summary>
		void Render()
		{
			// ImGui�̕`��
			ImGui::Render();
			ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
		}

		/// <summary>
		/// �I��
		/// </summary>
		void Finalize()
		{
			if (ImNodes::GetCurrentContext())
			{
				ImNodes::DestroyContext();
			}

			if (imGuiContext)
			{
				ImGui::SetCurrentContext(imGuiContext);

				ImGui_ImplGlfw_Shutdown();
				ImGui_ImplOpenGL3_Shutdown();
				ImGui::DestroyContext(imGuiContext);
			}
		}

		/// <summary>
		/// �m�[�h�G�f�B�^���J��
		/// </summary>
		/// <param name="nodeEditor"> �m�[�h�G�f�B�^ </param>
		void OpenNodeEditor(const NodeEditorPtr nodeEditor)
		{
			// �Ǘ��p�z��ɒǉ�
			// �ǉ��ς݂Ȃ�false�ɂȂ�
			openEditorList.emplace(nodeEditor);

			// ���ɒǉ�����Ă���̂ŁA�t�H�[�J�X����
			imGuiContext->NavWindow = &nodeEditor->GetImGuiWindow();
		}

	} // namespace NodeScript

} // namespace PokarinEngine