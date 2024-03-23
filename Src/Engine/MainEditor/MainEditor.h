/**
* @file MainEditor.h
*/
#ifndef EDITOR_H_INCLUDED
#define EDITOR_H_INCLUDED

#include "ImGui/imgui.h"

#include "Hierarchy.h"
#include "Inspector.h"
#include "SceneView.h"
#include "GameView.h"

#include "../UsingNames/UsingMesh.h"

#include <memory>
#include <vector>

namespace PokarinEngine
{
	// -------------------
	// �O���錾
	// -------------------

	class Engine;

	/// <summary>
	/// �G�f�B�^�Ǘ��N���X
	/// </summary>
	class MainEditor
	{
	public: // --------------- �R���X�g���N�^�E�f�X�g���N�^ -----------------

		MainEditor() = default;
		~MainEditor() = default;

	public: // ------------------------ �֎~���� ----------------------------

		// �R�s�[�R���X�g���N�^�̋֎~
		MainEditor(const MainEditor&) = delete;

		// ����̋֎~
		MainEditor& operator=(const MainEditor&) = delete;

	public: // -------------------------- ���� ------------------------------

		/// <summary>
		/// ������
		/// </summary>
		/// <param name="engine"> �G���W���N���X�̎Q�� </param>
		void Initialize(Engine& engine);

		/// <summary>
		/// �X�V
		/// </summary>
		void Update();

		/// <summary>
		/// �`��
		/// </summary>
		void Render();

		/// <summary>
		/// �I��
		/// </summary>
		void Finalize();

	public: // --------------------- �r���[�̎擾(������) -----------------------

		/// <summary>
		/// �V�[���r���[���擾����
		/// </summary>
		/// <returns> �V�[���r���[ </returns>
		const SceneView& GetSceneView()
		{
			return sceneView;
		}

		/// <summary>
		/// �Q�[���r���[���擾����
		/// </summary>
		/// <returns> �Q�[���r���[ </returns>
		const GameView& GetGameView()
		{
			return gameView;
		}

	private: // ------------------------ ImGui�ݒ�Ǘ� --------------------------

		/// <summary>
		/// ImGui�̐F�ݒ���J�n����
		/// </summary>
		/// <param name="target"> �ݒ�Ώ� </param>
		/// <param name="styleColor"> �F </param>
		void PushStyleColor(ImGuiCol target, const ImVec4& styleColor)
		{
			// �F�ݒ���J�n
			ImGui::PushStyleColor(target, styleColor);

			// �ݒ萔���J�E���g
			pushColorCount++;
		}

		/// <summary>
		/// ImGui�̐F�ݒ���I������
		/// </summary>
		void PopStyleColor()
		{
			// �F�ݒ���I��
			ImGui::PopStyleColor(pushColorCount);

			// �ݒ萔�����Z�b�g
			pushColorCount = 0;
		}

	private: // -------------------------- �o�[�W���� ---------------------------

		// GLSL�̃o�[�W����
		const char* glslVersion = "#version 450";

	private: // --------------------------- �`��p -------------------------------

		// �D�F
		const ImVec4 gray = { 0.2f, 0.2f, 0.2f, 1.0f };

		// ���F
		const ImVec4 black = { 0, 0, 0, 1.0f };

	private: // ------------------------- �`��p�r���[ ---------------------------

		// �V�[���r���[
		SceneView sceneView;

		// �Q�[���r���[
		GameView gameView;

	private: // -------------------------- �E�B���h�E ----------------------------

		// �q�G�����L�[
		Hierarchy hierarchy;

		// �C���X�y�N�^�[
		Inspector inspector;

	private: // ---------------------------- ImGui -------------------------------

		// ImGui�p�R���e�L�X�g
		ImGuiContext* imGuiContext = nullptr;

		// ImGui�̐F�ݒ�̐�
		int pushColorCount = 0;
	};

} // namespace PokarinEngine

#endif // !EDITOR_H_INCLUDED
