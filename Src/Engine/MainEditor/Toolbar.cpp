/**
* @file Toolbar.cpp
*/
#include "Toolbar.h"

#include "ImGui/imgui.h"

#include "../Engine.h"
#include "../TextureGetter.h"

#include <unordered_map>

namespace PokarinEngine
{
	/// <summary>
	/// �c�[���o�[
	/// </summary>
	namespace Toolbar
	{
		/// <summary>
		/// �c�[���o�[�Ŏg�p����摜�̃t�@�C����
		/// </summary>
		namespace ImageName
		{
			// �Đ��{�^��
			constexpr const char* playButton = "Res/Toolbar/PlayButton.tga";

			// ��~�{�^��
			constexpr const char* stopButton = "Res/Toolbar/StopButton.tga";
		}

		/// <summary>
		/// �c�[���o�[�Ŏg�p����摜�̎��ʔԍ�
		/// </summary>
		namespace ImageID
		{
			// �Đ��E��~�{�^��
			// [�Q�[�����Đ����Ȃ�true, �e�N�X�`�����ʔԍ�]
			std::unordered_map<bool, ImTextureID> playButton;
		}

		/// <summary>
		/// ������
		/// </summary>
		void Initialize()
		{
			// �Đ��{�^���̃e�N�X�`�����ʔԍ���ǉ�
			ImageID::playButton.emplace(false, ImTextureID(*TextureGetter::Get(ImageName::playButton)));

			// ��~�{�^���̃e�N�X�`�����ʔԍ���ǉ�
			ImageID::playButton.emplace(true, ImTextureID(*TextureGetter::Get(ImageName::stopButton)));
		}

		/// <summary>
		/// �X�V
		/// </summary>
		/// <param name="[out] isPlayGame"> �Q�[�����Đ����Ȃ�true </param>
		void Update(bool& isPlayGame)
		{
			ImGui::Begin("Toolbar", nullptr,
				ImGuiWindowFlags_::ImGuiWindowFlags_NoScrollbar |
				ImGuiWindowFlags_::ImGuiWindowFlags_NoScrollWithMouse);
			{
				// �Đ��{�^����X���W
				// �E�B���h�E�̐^�񒆂�����ɐݒ肷��
				float playButtonPosX = ImGui::GetWindowWidth() / 2.0f;

				// �Đ��E��~�{�^���̈ʒu��ݒ�
				ImGui::SetCursorPosX(playButtonPosX);

				// �{�^���̃T�C�Y
				static const ImVec2 buttonSize = { 30, 30 };

				// �Đ����Ȃ��~�{�^���̉摜���A��~���Ȃ�Đ��{�^���̉摜��\������
				if (ImGui::ImageButton(ImageID::playButton[isPlayGame], buttonSize, ImVec2(0, 1), ImVec2(1, 0)))
				{
					// �Đ��󋵂�؂�ւ���
					isPlayGame = !isPlayGame;
				}

				ImGui::End();
			}
		}
	}
}