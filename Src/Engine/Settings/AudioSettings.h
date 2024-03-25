/**
* @file AudioSettings.h
*/
#ifndef AUDIOSETTINGS_H_INCLUDED
#define AUDIOSETTINGS_H_INCLUDED

/**
* ������搶�̂����
* �c���Ă�������
* �����ƍ�蒼����
*/

/**
* �����Đ��v���C���[�ԍ�
*/
namespace AudioPlayer
{
	// BGM�̍Đ��Ɏg���v���C���[�ԍ�
	inline constexpr int bgm = 0;

}

/**
* BGM�ݒ�
*/
namespace BGM
{
	inline constexpr char test_title[] = "Res/Audio/BGM/Title/Test_Title.mp3";
	inline constexpr char test_play[] = "Res/Audio/BGM/Play/Test_Play.mp3";
}

/**
* ���ʉ��ݒ�
*/
namespace SE
{
	// �{�^���N���b�N
	inline constexpr char test_buttonClick[] = "Res/Audio/SE/Click/Test_Click.wav";
}

#endif // !AUDIOSETTINGS_H_INCLUDED
