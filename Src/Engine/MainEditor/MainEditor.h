/**
* @file MainEditor.h
*/
#ifndef EDITOR_H_INCLUDED
#define EDITOR_H_INCLUDED

#include "SceneView.h"
#include "GameView.h"
#include "Hierarchy.h"
#include "Inspector.h"

#include <memory>
#include <vector>

namespace PokarinEngine
{
	// -------------------
	// �O���錾
	// -------------------

	class Engine;
	class SceneView;
	class GameView;

	/// <summary>
	/// ���C���G�f�B�^�Ǘ��p
	/// </summary>
	class MainEditor
	{
	public: // --------------- �R���X�g���N�^�E�f�X�g���N�^ ----------------

		MainEditor() = default;
		~MainEditor() = default;

	public: // ---------------------------- ���� ---------------------------

		/// <summary>
		/// ������
		/// </summary>
		/// <param name="[in] e"> �G���W���N���X�̎Q�� </param>
		void Initialize(Engine& e);

		/// <summary>
		/// �X�V
		/// </summary>
		/// <param name="[out] isPlayGame"> �Q�[���Đ����Ȃ�true </param>
		void Update(bool& isPlayGame);

		/// <summary>
		/// �`��
		/// </summary>
		void Render();

		/// <summary>
		/// �I��
		/// </summary>
		void Finalize();

	public: // ------------------------ �r���[�̎擾 -----------------------

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

	private: // ------------------------ �G�f�B�^�p ------------------------

		/// <summary>
		/// ���C�����j���[
		/// </summary>
		void MainMenu();

	private: // ------------------------- �F�ݒ�p -------------------------

		/// <summary>
		/// ImGui�̐F�ݒ���J�n����
		/// </summary>
		/// <param name="[in] style"> �F��ݒ肵�������� </param>
		/// <param name="[in] styleColor"> �ݒ肷��F </param>
		void PushColor(ImGuiCol style, const Color& styleColor);

		/// <summary>
		/// ImGui�̐F�ݒ���I������
		/// </summary>
		void PopColor();

	private: // ------------------------ �E�B���h�E ------------------------

		// �V�[���r���[
		SceneView sceneView;

		// �Q�[���r���[
		GameView gameView;

		// �q�G�����L�[
		Hierarchy hierarchy;

		// �C���X�y�N�^�[
		Inspector inspector;

	private: // ------------------------- �o�[�W���� -----------------------

		// GLSL�̃o�[�W����
		const char* glslVersion = "#version 450";

	private: // -------------------------- ImGui�p -------------------------

		// ImGui�p�R���e�L�X�g
		ImGuiContext* imGuiContext = nullptr;

		// ImGui�̐F�ݒ�̐�
		int pushColorCount = 0;

	private: // ------------------------- �G���W���p -----------------------

		// �G���W��
		Engine* engine = nullptr;
	};

} // namespace PokarinEngine

#endif // !EDITOR_H_INCLUDED
