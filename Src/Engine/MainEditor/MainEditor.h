/**
* @file MainEditor.h
*/
#ifndef POKARINENGINE_EDITOR_H_INCLUDED
#define POKARINENGINE_EDITOR_H_INCLUDED

#include "SceneView.h"
#include "GameView.h"
#include "Hierarchy.h"
#include "Inspector.h"

#include <memory>
#include <vector>

namespace PokarinEngine
{
	/// <summary>
	/// ���C���G�f�B�^�Ǘ��p�N���X
	/// </summary>
	class MainEditor
	{
	public: // --------------- �R���X�g���N�^�E�f�X�g���N�^ ----------------

		MainEditor() = default;
		~MainEditor() = default;

	public: // -------------------------- �֎~���� -------------------------

		/* ImGui�R���e�L�X�g�̊Ǘ������������Ȃ�̂ŁA�֎~���� */

		// �R�s�[�R���X�g���N�^�̋֎~
		MainEditor(const MainEditor&) = delete;

		// ����̋֎~
		MainEditor& operator=(const MainEditor&) = delete;

	public: // ---------------------------- ���� ---------------------------

		/// <summary>
		/// ������
		/// </summary>
		void Initialize();

		/// <summary>
		/// �X�V
		/// </summary>
		/// <param name="[in] currentScene"> ���݂̃V�[�� </param>
		/// <param name="[out] isPlayGame"> �Q�[���Đ����Ȃ�true </param>
		void Update(const ScenePtr& currentScene, bool& isPlayGame);

		/// <summary>
		/// �`��
		/// </summary>
		void Render();

		/// <summary>
		/// �I��
		/// </summary>
		void Finalize();

	private: // ------------------------ �G�f�B�^�p ------------------------

		/// <summary>
		/// ���C�����j���[
		/// </summary>
		/// <param name="[in] currentScene"> ���݂̃V�[�� </param>
		void MainMenu(const ScenePtr& currentScene);

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

	private: // ------------------------- �E�B���h�E -----------------------

		SceneView sceneView;
		GameView gameView;
		Hierarchy hierarchy;
		Inspector inspector;

	private: // ------------------------- �o�[�W���� -----------------------

		// GLSL�̃o�[�W����
		const char* glslVersion = "#version 450";

	private: // -------------------------- ImGui�p -------------------------

		// ImGui�p�R���e�L�X�g
		ImGuiContext* imGuiContext = nullptr;

		// ImGui�̐F�ݒ�̐�
		int pushColorCount = 0;
	};

} // namespace PokarinEngine

#endif // !POKARINENGINE_EDITOR_H_INCLUDED
