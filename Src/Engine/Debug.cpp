/**
* @file Debug.cpp
*/
#include "Debug.h"

#include <stdio.h>
#include <stdarg.h>
#include <string>

#pragma warning(push)
#pragma warning(disable:4005)
#include <Windows.h>
#pragma warning(pop)

namespace PokarinEngine
{
	namespace Debug
	{
		/// <summary>
		/// 書式付き文字列をデバッグウィンドウに表示する
		/// </summary>
		/// <param name="[in] type"> メッセージの種類 </param>
		/// <param name="[in] func"> 関数名 </param>
		/// <param name="[in] format"> 書式付き文字列 </param>
		void Log(Type type, const char* func, const char* format, ...)
		{
			// メッセージ格納用
			char buffer[1024] = "";
			char* p = buffer;

			// メッセージのサイズ
			size_t size = sizeof(buffer) - 1; // 末尾に\nを追加するため

			// ログの種類と関数名を設定

			// メッセージの種類
			static const char* const typeNames[] = { "エラー", "警告", "情報" };

			// ログの種類と関数名をメッセージとして格納
			// 格納したメッセージの文字数を取得
			size_t n = snprintf(p, size, "[%s] %s: ",
				typeNames[static_cast<int>(type)], func);

			// 連続してメッセージを書き出したいので、
			// 先頭をメッセージの文字数ずらす
			p += n;

			// 残りサイズを反映
			size -= n;

			// メッセージを設定
			va_list ap;

			// va_list型の変数を初期化
			va_start(ap, format);

			// vsnprintf関数は「出力したバイト数」を返す
			// この値を出力先を指すポインタpに加算することで
			// 出力結果が連続するようにする
			p += vsnprintf(p, size, format, ap);

			// va_list型の変数を破棄
			va_end(ap);

			// 末尾に改行を追加
			p[0] = '\n';
			p[1] = '\0';

			// 作成した文字列をデバッグウィンドウに表示
			OutputDebugString(buffer);
		}

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
			const GLchar* message, const void* userParam)
		{
			std::string s;

			if (length < 0)
			{
				s = message;
			}
			else
			{
				s.assign(message, message + length);
			}

			s += '\n'; // メッセージには改行がないので追加する

			// DebugCallbackに渡されるメッセージには「エラー種別」が含まれているので、
			// LOG_ERRORやLOG_WARNINGを使うと、エラー種別が二重に出力されて分かりづらい
			// LOGならエラー種別を出力しないため、問題が起きない
			LOG(s.c_str());
		}

	} // namespace Debug

} // namespace PokarinEngine