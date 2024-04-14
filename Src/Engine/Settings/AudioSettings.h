/**
* @file AudioSettings.h
*/
#ifndef AUDIOSETTINGS_H_INCLUDED
#define AUDIOSETTINGS_H_INCLUDED

#include <string>

/// <summary>
/// 音声再生プレイヤー番号 
/// </summary>
namespace AudioPlayer
{
	// BGMの再生に使うプレイヤー番号
	inline constexpr int bgm = 0;

}

/// <summary>
/// BGMファイル名
/// </summary>
namespace BGM
{
	inline constexpr const char* test_title = "Res/Audio/BGM/Title/Test_Title.mp3";
	inline constexpr const char* test_play = "Res/Audio/BGM/Play/Test_Play.mp3";
}

/// <summary>
/// 効果音ファイル名
/// </summary>
namespace SE
{
	// ボタンクリック
	inline constexpr const char* test_buttonClick = "Res/Audio/SE/Click/Test_Click.wav";
}

#endif // !AUDIOSETTINGS_H_INCLUDED
