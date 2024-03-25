/**
* @file AudioSettings.h
*/
#ifndef AUDIOSETTINGS_H_INCLUDED
#define AUDIOSETTINGS_H_INCLUDED

/**
* これも先生のやつだよ
* 残しておくけど
* ちゃんと作り直せよ
*/

/**
* 音声再生プレイヤー番号
*/
namespace AudioPlayer
{
	// BGMの再生に使うプレイヤー番号
	inline constexpr int bgm = 0;

}

/**
* BGM設定
*/
namespace BGM
{
	inline constexpr char test_title[] = "Res/Audio/BGM/Title/Test_Title.mp3";
	inline constexpr char test_play[] = "Res/Audio/BGM/Play/Test_Play.mp3";
}

/**
* 効果音設定
*/
namespace SE
{
	// ボタンクリック
	inline constexpr char test_buttonClick[] = "Res/Audio/SE/Click/Test_Click.wav";
}

#endif // !AUDIOSETTINGS_H_INCLUDED
