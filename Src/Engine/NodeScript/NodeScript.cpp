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
	/// �N���X
	/// </summary>
	namespace
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
			void CreateContext()
			{
				imGuiContext = ImGui::CreateContext();
				imNodesContext = ImNodes::CreateContext();
			}

			/// <summary>
			/// �R���e�L�X�g���폜����
			/// </summary>
			void DestroyContext()
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
			void UsingContext() const
			{
				ImGui::SetCurrentContext(imGuiContext);
				ImNodes::SetCurrentContext(imNodesContext);
			}

		public: // ---------------------------- �t�H�[�J�X ---------------------------

			/// <summary>
			/// �m�[�h�G�f�B�^���t�H�[�J�X����
			/// </summary>
			/// <param name="nodeEditor"> �t�H�[�J�X����m�[�h�G�f�B�^ </param>
			void FocusEditor(NodeEditorPtr nodeEditor) const
			{
				imGuiContext->NavWindow = &nodeEditor->GetImGuiWindow();
			}

		private:

			// ImGui�p�R���e�L�X�g
			ImGuiContext* imGuiContext = nullptr;

			// ImNodes�p�R���e�L�X�g
			ImNodesContext* imNodesContext = nullptr;
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
			void UpdateEditor(const ImGuiID& dockSpaceID, const ContextManager& contextManager)
			{
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
					EraseClosedEditor(contextManager);
				}
			}

		public: // -------------------- �m�[�h�G�f�B�^�Ǘ��p ----------------------

			/// <summary>
			/// �m�[�h�G�f�B�^��ǉ�����
			/// </summary>
			/// <param name="nodeEditor"> �ǉ�����m�[�h�G�f�B�^ </param>
			void AddEditor(NodeEditorPtr nodeEditor)
			{
				openEditorList.emplace(nodeEditor);
			}

			/// <summary>
			/// ���Ă���m�[�h�G�f�B�^��z�񂩂�폜����
			/// </summary>
			void EraseClosedEditor(const ContextManager& contextManager)
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
					contextManager.FocusEditor(selectEditor);
				}

				// �폜�����m�[�h�G�f�B�^�̏��L�������
				closedEditor.reset();
			}

		private: // -------------------- �^�̕ʖ����` -----------------------

			using OpenEditorList = std::unordered_set<NodeEditorPtr>;
			using ClosedEditorList = std::vector<NodeEditorPtr>;

		private: // -------------------- �m�[�h�G�f�B�^ -----------------------

			// �J���Ă���m�[�h�G�f�B�^�z��
			OpenEditorList openEditorList;

			// ���Ă���m�[�h�G�f�B�^�z��
			NodeEditorPtr closedEditor;

			// �I�𒆂̃m�[�h�G�f�B�^
			NodeEditorPtr selectEditor;
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
			/// <param name="style"> �F��ݒ肵�������� </param>
			/// <param name="styleColor"> �ݒ肷��F </param>
			void Push(ImGuiCol style, const Color& styleColor)
			{
				ImGui::PushStyleColor(style, styleColor);
				pushColorCount++;
			}

			/// <summary>
			/// ImGui�̐F�ݒ���I������
			/// </summary>
			void Pop()
			{
				ImGui::PopStyleColor(pushColorCount);
				pushColorCount = 0;
			}

		private: // -------------------------- ��� --------------------------

			// ImGui�̐F�ݒ�̉�
			int pushColorCount = 0;
		};
	}

	/// <summary>
	/// �ϐ�
	/// </summary>
	namespace
	{
		// �R���e�L�X�g�Ǘ��p
		ContextManager contextManager;

		// �m�[�h�G�f�B�^�Ǘ��p
		NodeEditorManager nodeEditorManager;

		// ImGui�̐F�ݒ�p
		SetUpColor setUpColor;
	}

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

			// �m�[�h�X�N���v�g�p�̃E�B���h�E���g�p����悤�ɐݒ�
			glfwMakeContextCurrent(&Window::GetWindow(WindowID::NodeScript));

			// ImGui�̃o�[�W�������m�F
			IMGUI_CHECKVERSION();

			// �R���e�L�X�g�쐬
			contextManager.CreateContext();

			// �R���e�L�X�g���g�p����
			contextManager.UsingContext();

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

			// �R���e�L�X�g�̎g�p
			contextManager.UsingContext();

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

			setUpColor.Push(ImGuiCol_::ImGuiCol_Tab, BasicColor::gray);
			setUpColor.Push(ImGuiCol_::ImGuiCol_TitleBgActive, BasicColor::gray);

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

			nodeEditorManager.UpdateEditor(dockSpaceID, contextManager);

			// --------------------------------------
			// ImGui�̊ۂݐݒ���I������
			// --------------------------------------

			ImGui::PopStyleVar();

			// -------------------------------------
			// ImGui�̐F�ݒ���I������
			// -------------------------------------

			setUpColor.Pop();

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
			contextManager.UsingContext();

			// ImGui�̕`��
			ImGui::Render();
			ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
		}

		/// <summary>
		/// �I��
		/// </summary>
		void Finalize()
		{
			contextManager.DestroyContext();
		}

		/// <summary>
		/// �m�[�h�G�f�B�^���J��
		/// </summary>
		/// <param name="nodeEditor"> �m�[�h�G�f�B�^ </param>
		void OpenNodeEditor(const NodeEditorPtr nodeEditor)
		{
			// �m�[�h�G�f�B�^���J��
			nodeEditor->OpenEditor();

			// �Ǘ��p�z��ɒǉ�
			nodeEditorManager.AddEditor(nodeEditor);

			// �J�����m�[�h�G�f�B�^���t�H�[�J�X����
			contextManager.FocusEditor(nodeEditor);
		}

	} // namespace NodeScript

} // namespace PokarinEngine