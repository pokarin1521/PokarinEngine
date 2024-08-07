/**
* @file GameView.cpp
*/
#include "GameView.h"

#include "../Scene.h"

#include "../Color.h"
#include "../FramebufferObject.h"

namespace PokarinEngine
{
	/// <summary>
	/// �X�V
	/// </summary>
	/// <param name="currentScene"> ���݂̃V�[�� </param>
	void GameView::Update(const ScenePtr& currentScene)
	{
		// ---------------------------
		// �E�B���h�E���쐬
		// ---------------------------

		// FBO����e�N�X�`�����擾
		ImTextureID texture = ImTextureID(currentScene->GetTextureID());

		// �E�B���h�E�̊ۂ݂𖳂���
		ImGui::PushStyleVar(ImGuiStyleVar_::ImGuiStyleVar_WindowPadding, ImVec2(0, 0));

		// �V�[���r���[�p�E�B���h�E
		ImGui::Begin("Game", nullptr,
			ImGuiWindowFlags_::ImGuiWindowFlags_NoScrollbar |
			ImGuiWindowFlags_::ImGuiWindowFlags_NoScrollWithMouse);
		{
			// �V�[���r���[�p�E�B���h�E�̑傫��
			// �`�掞�ɐݒ肷��
			ImVec2 size = ImGui::GetWindowSize();

			// �V�[���r���[�p�E�B���h�E�̑傫���ŕ`��
			// ���̂܂܂��Ɖ摜�����]���Ă��܂��̂ŁA
			// ��������E��ɕ`�悷��悤�ɐݒ�
			ImGui::Image(texture, size, ImVec2(0, 1), ImVec2(1, 0));

			// �E�B���h�E�̊ۂݐݒ���I��
			ImGui::PopStyleVar();

			// �E�B���h�E���I��
			ImGui::End();
		}

		// -------------------------------
		// �J���[�o�b�t�@���N���A
		// -------------------------------

		//fbo->ClearColor(Color::black);
	}

} // namespace PokarinEngine