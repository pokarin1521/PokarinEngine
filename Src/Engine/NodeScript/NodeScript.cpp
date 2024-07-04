/**
* @file NodeScript.cpp
*/
#include "NodeScript.h"

#include "ImGui/imgui.h"
#include "ImGui/imgui_internal.h"
#include "ImGui/imgui_impl_glfw.h"
#include "ImGui/imgui_impl_opengl3.h"

#include "ImGui/imnodes.h"
#include "ImGui/imnodes_internal.h"

#include "NodeEditor.h"

#include "../Configs/ImGuiConfig.h"
#include "../ImGuiFontSetter.h"

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
	/// �R���e�L�X�g�Ǘ��p�N���X
	/// </summary>
	class ContextManager
	{
	public: // ------------------------ �R���e�L�X�g���� -------------------------

		/// <summary>
		/// �R���e�L�X�g���쐬����
		/// </summary>
		static void CreateContext()
		{
			imGuiContext = ImGui::CreateContext();
			imNodesContext = ImNodes::CreateContext();
		}

		/// <summary>
		/// �R���e�L�X�g���폜����
		/// </summary>
		static void DestroyContext()
		{
			// ImNodes�R���e�L�X�g���폜
			if (imNodesContext)
			{
				ImNodes::SetCurrentContext(imNodesContext);
				ImNodes::DestroyContext(imNodesContext);
			}

			// ImGui�R���e�L�X�g���폜
			if (imGuiContext)
			{
				ImGui::SetCurrentContext(imGuiContext);

				ImGui_ImplGlfw_Shutdown();
				ImGui_ImplOpenGL3_Shutdown();
				ImGui::DestroyContext(imGuiContext);
			}
		}

		/// <summary>
		/// �R���e�L�X�g���g�p����
		/// </summary>
		static void UsingContext()
		{
			ImGui::SetCurrentContext(imGuiContext);
			ImNodes::SetCurrentContext(imNodesContext);
		}

	public: // ---------------------------- �t�H�[�J�X ---------------------------

		/// <summary>
		/// �m�[�h�G�f�B�^���t�H�[�J�X����
		/// </summary>
		/// <param name="[in] nodeEditor"> �t�H�[�J�X����m�[�h�G�f�B�^ </param>
		static void FocusEditor(const NodeEditorPtr& nodeEditor)
		{
			imGuiContext->NavWindow = &nodeEditor->GetImGuiWindow();
		}

	private:

		// ImGui�p�R���e�L�X�g
		inline static ImGuiContext* imGuiContext = nullptr;

		// ImNodes�p�R���e�L�X�g
		inline static ImNodesContext* imNodesContext = nullptr;
	};

	/// <summary>
	/// �m�[�h�G�f�B�^�Ǘ��p�N���X
	/// </summary>
	class NodeEditorManager
	{
	public: // -------------------- �m�[�h�G�f�B�^�̍X�V ----------------------

		/// <summary>
		/// �m�[�h�G�f�B�^���X�V����
		/// </summary>
		/// <param name="[in] dockSpaceID"> �h�b�N�X�y�[�X�̎��ʔԍ� </param>
		static void UpdateEditor(const ImGuiID& dockSpaceID)
		{
			// �m�[�h�X�N���v�g�E�B���h�E������ꂽ��
			// �S�Ẵm�[�h�G�f�B�^�E�B���h�E�����
			if (glfwWindowShouldClose(&Window::GetWindow(WindowID::NodeScript)))
			{
				openEditorList.clear();
				return;
			}

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
		}

	public: // -------------------- �m�[�h�G�f�B�^�Ǘ��p ----------------------

		/// <summary>
		/// �m�[�h�G�f�B�^��ǉ�����
		/// </summary>
		/// <param name="[in] nodeEditor"> �ǉ�����m�[�h�G�f�B�^ </param>
		static void AddEditor(const NodeEditorPtr& nodeEditor)
		{
			openEditorList.emplace(nodeEditor);
		}

		/// <summary>
		/// ���Ă���m�[�h�G�f�B�^��z�񂩂�폜����
		/// </summary>
		static void EraseClosedEditor()
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
				ContextManager::FocusEditor(selectEditor);
			}

			// �폜�����m�[�h�G�f�B�^�̏��L�������
			closedEditor.reset();
		}

		/// <summary>
		/// �m�[�h�G�f�B�^���폜����
		/// </summary>
		/// <param name="[in] nodeEditor"> �폜����m�[�h�G�f�B�^ </param>
		static void EraseEditor(const NodeEditorPtr& nodeEditor)
		{
			openEditorList.erase(nodeEditor);
		}

	private: // -------------------- �^�̕ʖ����` -----------------------

		using OpenEditorList = std::unordered_set<NodeEditorPtr>;
		using ClosedEditorList = std::vector<NodeEditorPtr>;

	private: // -------------------- �m�[�h�G�f�B�^ -----------------------

		// �J���Ă���m�[�h�G�f�B�^�z��
		inline static OpenEditorList openEditorList;

		// ���Ă���m�[�h�G�f�B�^�z��
		inline static NodeEditorPtr closedEditor;

		// �I�𒆂̃m�[�h�G�f�B�^
		inline static NodeEditorPtr selectEditor;
	};

	/// <summary>
	/// ImGui�̐F�ݒ�p�N���X
	/// </summary>
	class SetUpColor
	{
	public: // ----------------------- �F�ݒ� --------------------------

		/// <summary>
		/// ImGui�̐F�ݒ���J�n����
		/// </summary>
		/// <param name="[in] style"> �F��ݒ肵�������� </param>
		/// <param name="[in] styleColor"> �ݒ肷��F </param>
		static void Push(ImGuiCol style, const Color& styleColor)
		{
			// ImVec4�^�ɕϊ�
			const ImVec4 color = { styleColor.r,styleColor.g,styleColor.b,styleColor.a };

			// �F��ݒ�
			ImGui::PushStyleColor(style, color);

			// �ݒ萔���J�E���g
			pushColorCount++;
		}

		/// <summary>
		/// ImGui�̐F�ݒ���I������
		/// </summary>
		static void Pop()
		{
			ImGui::PopStyleColor(pushColorCount);
			pushColorCount = 0;
		}

	private: // -------------------------- ��� --------------------------

		// ImGui�̐F�ݒ�̉�
		inline static int pushColorCount = 0;
	};

	/// <summary>
	/// �m�[�h�G�f�B�^���Ǘ�����@�\
	/// </summary>
	namespace NodeScript
	{
		/// <summary>
		/// ������
		/// </summary>
		void Initialize()
		{
			// ---------------------------
			// �R���e�L�X�g�쐬
			// ---------------------------

			// �m�[�h�X�N���v�g�p�̃E�B���h�E���g�p����
			Window::SetCurrentWindow(WindowID::NodeScript);

			// ImGui�̃o�[�W�������m�F
			IMGUI_CHECKVERSION();

			// �R���e�L�X�g�쐬
			ContextManager::CreateContext();

			// �R���e�L�X�g���g�p����
			ContextManager::UsingContext();

			// ----------------------------------------
			// ImGui�̕ۑ���t�@�C����ݒ�
			// ----------------------------------------

			// ImGui�̐ݒ�p
			ImGuiIO& io = ImGui::GetIO();

			// �ۑ����ݒ�
			io.IniFilename = ImGuiConfig::File::setting;

			// ---------------------------------------
			// �h�b�L���O�E�B���h�E�̗L����
			// ---------------------------------------

			io.ConfigFlags |= ImGuiConfigFlags_DockingEnable;

			// -------------------------------------------------------
			// ImGui�E�B���h�E�̏���ۑ����Ȃ��悤�ɐݒ�
			// -------------------------------------------------------

			// �X�V���ɐݒ肵�Ă���̂ŁA�ۑ����Ă����K�v���Ȃ�
			io.IniFilename = nullptr;

			// ------------------------
			// �t�H���g��ݒ�
			// ------------------------

			ImGuiFontSetter::SetFont(io);

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

			// �R���e�L�X�g�̎g�p
			ContextManager::UsingContext();

			// ImGui�̍X�V
			ImGui_ImplGlfw_NewFrame();
			ImGui_ImplOpenGL3_NewFrame();
			ImGui::NewFrame();

			// ------------------------------------
			// ImGui�̊ۂݐݒ�
			// ------------------------------------

			// �^�u�̊ۂ݂𖳂���
			ImGui::PushStyleVar(ImGuiStyleVar_::ImGuiStyleVar_TabRounding, 0);

			// ------------------------------------
			// ImGui�̐F�ݒ�
			// ------------------------------------

			SetUpColor::Push(ImGuiCol_::ImGuiCol_Tab, Color::gray);
			SetUpColor::Push(ImGuiCol_::ImGuiCol_TitleBgActive, Color::gray);

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

			// --------------------------------------
			// �m�[�h�G�f�B�^���X�V
			// --------------------------------------

			NodeEditorManager::UpdateEditor(dockSpaceID);

			// --------------------------------------
			// ImGui�̊ۂݐݒ���I������
			// --------------------------------------

			ImGui::PopStyleVar();

			// -------------------------------------
			// ImGui�̐F�ݒ���I������
			// -------------------------------------

			SetUpColor::Pop();

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
			// �R���e�L�X�g��ݒ�
			ContextManager::UsingContext();

			// ImGui�̕`��
			ImGui::Render();
			ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
		}

		/// <summary>
		/// �I��
		/// </summary>
		void Finalize()
		{
			ContextManager::DestroyContext();
		}

		/// <summary>
		/// �m�[�h�G�f�B�^���J��
		/// </summary>
		/// <param name="[in] nodeEditor"> �m�[�h�G�f�B�^ </param>
		void OpenNodeEditor(const NodeEditorPtr& nodeEditor)
		{
			// �m�[�h�G�f�B�^���J��
			nodeEditor->OpenEditor();

			// �Ǘ��p�z��ɒǉ�
			NodeEditorManager::AddEditor(nodeEditor);

			// �J�����m�[�h�G�f�B�^���t�H�[�J�X����
			ContextManager::FocusEditor(nodeEditor);
		}

		/// <summary>
		/// �m�[�h�G�f�B�^�����
		/// </summary>
		/// <param name="[in] nodeEditor"> �m�[�h�G�f�B�^ </param>
		void CloseNodeEditor(const NodeEditorPtr& nodeEditor)
		{
			NodeEditorManager::EraseEditor(nodeEditor);
		}

	} // namespace NodeScript

} // namespace PokarinEngine