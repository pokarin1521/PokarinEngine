/**
* @file Window.h
*/
#ifndef WINDOW_H_INCLUDED
#define WINDOW_H_INCLUDED

#include "glad/glad.h"
#include "Math/Vector.h"

#include <GLFW/glfw3.h>

namespace PokarinEngine
{
	/// <summary>
	/// ウィンドウ識別番号
	/// </summary>
	enum class WindowID
	{
		Main,	     // メイン
		NodeScript,  // ノードスクリプト用
		Max,         // ウィンドウの数
	};

	/// <summary>
	/// ウィンドウ管理用
	/// </summary>
	namespace Window
	{
		/// <summary>
		/// 全てのウィンドウオブジェクトを作成する
		/// </summary>
		/// <returns>
		/// <para> true : 全てのウィンドウオブジェクトの作成に成功 </para> 
		/// <para> false : いずれかのウィンドウオブジェクトの作成に失敗 </para> 
		/// </returns>
		bool CreateAllWindow();

		/// <summary>
		/// ウィンドウを開く
		/// </summary>
		/// <param name="windowID"> ウィンドウ識別番号 </param>
		/// <param name="windowTitle"> ウィンドウタイトル </param>
		void OpenWindow(WindowID windowID, const char* windowTitle);

		/// <summary>
		/// ウィンドウを閉じる
		/// </summary>
		/// <param name="windowID"> ウィンドウ識別番号 </param>
		void CloseWindow(WindowID windowID);

		/// <summary>
		/// ウィンドウの使用を開始する
		/// </summary>
		/// <param name="windowID"> 使用するウィンドウ識別番号 </param>
		/// <returns>
		/// <para> true : 指定したウィンドウがアクティブ </para>
		/// <para> false : 指定したウィンドウが非アクティブ </para>
		/// </returns>
		bool Begin(WindowID windowID);

		/// <summary>
		/// ウィンドウの使用を終了する
		/// </summary>
		void End();

		/// <summary>
		/// ウィンドウが閉じているかを取得する
		/// </summary>
		/// <param name="windowID"> ウィンドウ識別番号 </param>
		/// <returns>
		/// <para> true : ウィンドウが閉じている </para>
		/// <para> false : ウィンドウが閉じていない </para>
		/// </returns>
		bool IsClosed(WindowID windowID);

		/// <summary>
		/// ウィンドウを取得する
		/// </summary>
		/// <param name="windowID"> ウィンドウ識別番号 </param>
		/// <returns> 指定したウィンドウのウィンドウオブジェクト </returns>
		GLFWwindow& GetWindow(WindowID windowID);

		/// <summary>
		/// GLFWウィンドウの大きさを取得する
		/// </summary>
		/// <param name="windowID"> ウィンドウ識別番号 </param>
		/// <returns> 指定したウィンドウの大きさ </returns>
		Vector2 GetWindowSize(WindowID windowID);

		/// <summary>
		/// GLFWウィンドウの大きさを取得する
		/// </summary>
		/// <param name="[in] windowID"> ウィンドウ識別番号 </param>
		/// <param name="[out] w"> ウィンドウの幅 </param>
		/// <param name="[out] h"> ウィンドウの高さ </param>
		void GetWindowSize(WindowID windowID, GLsizei& w, GLsizei& h);

		/// <summary>
		/// ウィンドウのアスペクト比を取得する
		/// </summary>
		/// <param name="windowID"> ウィンドウ識別番号 </param>
		/// <returns> ウィンドウのアスペクト比</returns>
		float GetAspectRatio(WindowID windowID);

		/// <summary>
		/// メインモニターの情報を取得する
		/// </summary>
		/// <returns> メインモニターの情報 </returns>
		const GLFWvidmode& GetMainMonitor();

	} // namespace Window

} // namespace PokarinEngine

#endif // !WINDOW_H_INCLUDED
