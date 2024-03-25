/**
* @file Window.cpp
*/
#include "Window.h"

#include "ImGui/imgui_impl_glfw.h"
#include "ImGui/imgui_impl_opengl3.h"

#include "Debug.h"

#include <string>
#include <vector>
#include <unordered_map>

namespace PokarinEngine
{
	namespace Window
	{
		// ------------------------------------
		// ウィンドウオブジェクト
		// ------------------------------------

		// ウィンドウ管理用配列<ウィンドウ識別番号, ウィンドウオブジェクト>
		std::unordered_map<WindowID, GLFWwindow*> windowList;

		// ------------------------------------
		// 関数
		// ------------------------------------

		/// ここでしか使わないので、cppのみに書く
		/// <summary>
		/// ウィンドウが作成されているかを取得する
		/// </summary>
		/// <param name="windowID"> ウィンドウ識別番号 </param>
		/// <returns>
		/// <para> true : 指定した番号のウィンドウが作成されている </para>
		/// <para> false : 指定した番号のウィンドウが作成されていない </para>
		/// </returns>
		bool IsCreated(WindowID windowID)
		{
			return windowList.find(windowID) != windowList.end();
		}

		/// ここでしか使わないので、cppのみに書く
		/// <summary>
		/// ウィンドウオブジェクトを作成する
		/// </summary>
		/// <param name="windowID"> ウィンドウ識別番号 </param>
		/// <param name="windowTitle"> ウィンドウタイトル </param>
		/// <returns>
		/// <para> true : ウィンドウオブジェクトの作成に成功、または作成済み </para> 
		/// <para> false : ウィンドウオブジェクトの作成に失敗 </para> 
		/// </returns>
		bool CreateWindow(WindowID windowID)
		{
			// 既にウィンドウがあるならそれを返す
			if (IsCreated(windowID))
			{
				return true;
			}

			// -----------------------------
			// デバッグコンテキストの作成
			// -----------------------------

			glfwWindowHint(GLFW_OPENGL_DEBUG_CONTEXT, GLFW_TRUE);

			// ----------------------------------------
			// ウィンドウをフルスクリーンに設定
			// ----------------------------------------

			glfwWindowHint(GLFW_MAXIMIZED, GL_TRUE);

			// ----------------------------------------
			// ウィンドウを非表示に設定
			// ----------------------------------------

			// 必要に応じて表示させたいので
			// 最初は非表示にしておく
			glfwWindowHint(GLFW_VISIBLE, GL_FALSE);

			// ------------------------
			// 描画ウィンドウの作成
			// ------------------------

			/* GLFWウィンドウオブジェクトは、
			GLFWが作成したウィンドウのデータを管理するオブジェクト

			このオブジェクトを通して、
			ウィンドウの位置やサイズ、キー入力、マウス入力を取得する */

			// メインモニターの情報
			const GLFWvidmode mainMonitor = GetMainMonitor();

			// ウィンドウオブジェクト
			GLFWwindow* window = nullptr;

			// GLFWウィンドウを作成
			// ウィンドウサイズにはメインモニターのサイズを指定しておく
			window = glfwCreateWindow(mainMonitor.width, mainMonitor.height,
				"OpenGL Window", nullptr, nullptr);

			// ウィンドウの作成に失敗
			if (!window)
			{
				glfwTerminate();
				return false;
			}

			// ウィンドウ管理用配列に追加
			windowList.emplace(windowID, window);

			return true;
		}

		/// <summary>
		/// 全てのウィンドウオブジェクトを作成する
		/// </summary>
		/// <returns>
		/// <para> true : 全てのウィンドウオブジェクトの作成に成功 </para> 
		/// <para> false : いずれかのウィンドウオブジェクトの作成に失敗 </para> 
		/// </returns>
		bool CreateAllWindow()
		{
			// 作成可能なウィンドウの数
			int windowMax = static_cast<int>(WindowID::Max);

			// 全てのウィンドウオブジェクトを作成
			for (int i = 0; i < windowMax; ++i)
			{
				WindowID windowID = WindowID(i);

				// 作成に失敗したので中断
				if (!CreateWindow(windowID))
				{
					LOG_ERROR("%d番のウィンドウオブジェクトが作成できませんでした", i);
					return false;
				}
			}

			// 作成に成功
			return true;
		}

		/// <summary>
		/// ウィンドウを開く
		/// </summary>
		/// <param name="windowID"> ウィンドウ識別番号 </param>
		/// <param name="windowTitle"> ウィンドウタイトル </param>
		void OpenWindow(WindowID windowID, const char* windowTitle)
		{
			// ウィンドウオブジェクトがないので中断
			if (!IsCreated(windowID))
			{
				return;
			}

			// ウィンドウオブジェクト
			GLFWwindow* window = &GetWindow(windowID);

			// ウィンドウタイトルを設定
			glfwSetWindowTitle(window, windowTitle);

			// ウィンドウを表示
			glfwShowWindow(window);

			// ウィンドウが最小化されているなら
			// 元の大きさに戻す
			if (glfwGetWindowAttrib(window, GLFW_ICONIFIED))
			{
				glfwRestoreWindow(window);
			}

			// ウィンドウが閉じないように設定
			glfwSetWindowShouldClose(window, false);
		}

		/// <summary>
		/// ウィンドウを閉じる
		/// </summary>
		/// <param name="windowID"> ウィンドウ識別番号 </param>
		void CloseWindow(WindowID windowID)
		{
			// 指定したウィンドウが無い場合は何もしない
			if (!IsCreated(windowID))
			{
				return;
			}

			glfwSetWindowShouldClose(windowList[windowID], true);
		}

		/// <summary>
		/// ウィンドウの使用を開始する
		/// </summary>
		/// <param name="windowID"> 使用するウィンドウ識別番号 </param>
		/// <returns>
		/// <para> true : 指定したウィンドウがアクティブ </para>
		/// <para> false : 指定したウィンドウが非アクティブ </para>
		/// </returns>
		bool Begin(WindowID windowID)
		{
			// ウィンドウがないので中断
			if (!IsCreated(windowID))
			{
				return false;
			}

			// ウィンドウ
			GLFWwindow* window = windowList[windowID];

			// 使用するウィンドウを設定
			glfwMakeContextCurrent(windowList[windowID]);

			// OSからの要求を処理する
			// キーボードやマウスなどの状態を取得するには、定期的に呼び出す必要がある
			glfwPollEvents();

			// ウィンドウがアクティブになったらtrue
			return glfwGetWindowAttrib(window, GLFW_FOCUSED);
		}

		/// <summary>
		/// ウィンドウの使用を終了する
		/// </summary>
		void End()
		{
			/* フレームバッファは絵を描画するためのメモリ
			表示側と描画側の2つを用意し、描画が終わったら表示側と交換というのを繰り返す
			裏で描画することで描画中の絵を表示するのを防いでいる
			表示側をフロントバッファ、描画側をバックバッファという */

			// 現在使用中のウィンドウ
			GLFWwindow* window = glfwGetCurrentContext();

			// 使用中のウィンドウがなければ中断
			if (!window)
			{
				return;
			}

			// フロントバッファとバックバッファの入れ替え
			glfwSwapBuffers(window);

			// ウィンドウが閉じられたら非表示にする
			if (glfwWindowShouldClose(window))
			{
				glfwHideWindow(window);
			}

			// ウィンドウの使用を終了
			glfwMakeContextCurrent(nullptr);
		}

		/// <summary>
		/// ウィンドウが閉じているかを取得する
		/// </summary>
		/// <param name="windowID"> ウィンドウ識別番号 </param>
		/// <returns>
		/// <para> true : ウィンドウが閉じている </para>
		/// <para> false : ウィンドウが閉じていない </para>
		/// </returns>
		bool IsClosed(WindowID windowID)
		{
			// ウィンドウが作成されていないならtrue
			if (!IsCreated(windowID))
			{
				return true;
			}

			// ウィンドウが非表示ならtrue
			return !glfwGetWindowAttrib(&GetWindow(windowID), GLFW_VISIBLE);
		}

		/// <summary>
		/// ウィンドウを取得する
		/// </summary>
		/// <param name="windowID"> ウィンドウ識別番号 </param>
		/// <returns> 
		/// <para> 指定したウィンドウのウィンドウオブジェクト </para>
		/// <para> 指定した番号のウィンドウが無い場合は、メインウィンドウを返す </para>
		/// </returns>
		GLFWwindow& GetWindow(WindowID windowID)
		{
			// 指定した番号のウィンドウが無い場合は
			// メインウィンドウを返す
			if (!IsCreated(windowID))
			{
				// メインウィンドウが無い場合は作成する
				if (!IsCreated(WindowID::Main))
				{
					CreateWindow(WindowID::Main);
				}

				return *windowList[WindowID::Main];
			}

			// 指定した番号のウィンドウ
			return *windowList[windowID];
		}

		/// <summary>
		/// GLFWウィンドウの大きさを取得する
		/// </summary>
		/// <param name="windowID"> ウィンドウ識別番号 </param>
		/// <returns> 指定したウィンドウの大きさ </returns>
		Vec2 GetWindowSize(WindowID windowID)
		{
			// 指定したウィンドウが無い場合は0を返す
			if (!IsCreated(windowID))
			{
				return { 0, 0 };
			}

			// フレームバッファの大きさを取得
			int w, h;
			glfwGetFramebufferSize(windowList[windowID], &w, &h);

			// vec2型で返す
			return { static_cast<float>(w), static_cast<float>(h) };
		}

		/// <summary>
		/// GLFWウィンドウの大きさを取得する
		/// </summary>
		/// <param name="[in] windowID"> ウィンドウ識別番号 </param>
		/// <param name="[out] w"> ウィンドウの幅 </param>
		/// <param name="[out] h"> ウィンドウの高さ </param>
		void GetWindowSize(WindowID windowID, GLsizei& w, GLsizei& h)
		{
			if (IsCreated(windowID))
			{
				glfwGetFramebufferSize(windowList[windowID], &w, &h);
			}
		}

		/// <summary>
		/// ウィンドウのアスペクト比を取得する
		/// </summary>
		/// <param name="windowID"> ウィンドウ識別番号 </param>
		/// <returns> ウィンドウのアスペクト比</returns>
		float GetAspectRatio(WindowID windowID)
		{
			// フレームバッファの大きさを取得
			const Vec2 size = GetWindowSize(windowID);

			// 0除算は避ける
			if (size.y == 0)
			{
				return 0;
			}

			// アスペクト比を返す
			return size.x / size.y;
		}

		/// <summary>
		/// メインモニターの情報を取得する
		/// </summary>
		/// <returns> メインモニターの情報 </returns>
		const GLFWvidmode& GetMainMonitor()
		{
			return *glfwGetVideoMode(glfwGetPrimaryMonitor());
		}

	} // namespace Window

} // namespace PokarinEngine