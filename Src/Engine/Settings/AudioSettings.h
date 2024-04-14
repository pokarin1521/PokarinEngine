/**
* @file AudioSettings.h
*/
#ifndef AUDIOSETTINGS_H_INCLUDED
#define AUDIOSETTINGS_H_INCLUDED

#include <string>

/// <summary>
/// �����Đ��v���C���[�ԍ� 
/// </summary>
namespace AudioPlayer
{
	// BGM�̍Đ��Ɏg���v���C���[�ԍ�
	inline constexpr int bgm = 0;

}

/// <summary>
/// BGM�t�@�C����
/// </summary>
namespace BGM
{
	inline constexpr const char* test_title = "Res/Audio/BGM/Title/Test_Title.mp3";
	inline constexpr const char* test_play = "Res/Audio/BGM/Play/Test_Play.mp3";
}

/// <summary>
/// ���ʉ��t�@�C����
/// </summary>
namespace SE
{
	// �{�^���N���b�N
	inline constexpr const char* test_buttonClick = "Res/Audio/SE/Click/Test_Click.wav";
}

#endif // !AUDIOSETTINGS_H_INCLUDED
