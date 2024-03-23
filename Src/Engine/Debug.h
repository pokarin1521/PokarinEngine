/**
* @file Debug.h
*/
#ifndef DEBUG_H_INCLUDED
#define DEBUG_H_INCLUDED

#include "glad/glad.h"

#include <GLFW/glfw3.h>

namespace PokarinEngine
{
	/// <summary>
	/// デバッグ用の機能を格納する名前空間
	/// </summary>
	namespace Debug
	{
		// メッセージの種類
		enum class Type
		{
			error,	 // エラー
			warning, // 警告
			info,	 // その他の情報
		};

		/// <summary>
		/// 書式付き文字列をデバッグウィンドウに表示する
		/// </summary>
		/// <param name="type"> メッセージの種類 </param>
		/// <param name="func"> 関数名 </param>
		/// <param name="format"> 書式付き文字列 </param>
		void Log(Type type, const char* func, const char* format, ...);

		/// <summary>
		/// OpenGLからのメッセージを処理するコールバック関数
		/// </summary>
		/// <param name="source"> メッセージの発信者(OpenGL、Windows、シェーダなど) </param>
		/// <param name="type"> メッセージの種類(エラー、警告など) </param>
		/// <param name="id"> メッセージを一位に識別する値 </param>
		/// <param name="serity"> メッセージの重要度(高、中、低、最低) </param>
		/// <param name="length"> メッセージの文字数. 負数ならメッセージは0終端されている </param>
		/// <param name="message"> メッセージ本体 </param>
		/// <param name="userParam"> コールバック設定時に指定したポインタ </param>
		void APIENTRY Callback(GLenum source, GLenum type,
			GLuint id, GLenum serity, GLsizei length,
			const GLchar* message, const void* userParam);

	} // namespace Debug

	// Releseビルドの時だけ定義されるマクロ
	// デバッグ機能なし
#ifndef NDEBUG

// このマクロを定義するとデバッグ出力が有効になる
#define ENABLE_DEBUG_LOG 

#endif // !NDEBUG

// デバッグ出力マクロ
#ifdef ENABLE_DEBUG_LOG

// 実行中に支障をきたすレベルの問題
#define LOG_ERROR(...)	 Debug::Log(Debug::Type::error, __func__, __VA_ARGS__)

// 設定ミスなどの軽微な問題
#define LOG_WARNING(...) Debug::Log(Debug::Type::warning, __func__, __VA_ARGS__)

// 問題ではないが、情報として出力したいメッセージ
// (確認のために、読み込んだファイル名を出力するなど)
#define LOG(...)		 Debug::Log(Debug::Type::info, __func__, __VA_ARGS__)

// デバッグ出力をすると、ゲームの実行速度が遅くなるため
// デバッグ時以外では無効にする
#else

// マクロの中身を((void)0)に置き換える
// LOG_ERROR(); -> (void)0; になる
// 空文にしないのは、単独の「;」に対して警告が発生するため
#define LOG_ERROR(...)	 ((void)0)
#define LOG_WARNING(...) ((void)0)
#define LOG(...)		 ((void)0)

#endif // ENABLE_DEBUG_LOG

} // namespace PokarinEngine

#endif // !DEBUG_H_INCLUDED
