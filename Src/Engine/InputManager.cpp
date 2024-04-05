/**
* @file InputManager.cpp
*/
#include "InputManager.h"

namespace PokarinEngine
{
	/// <summary>
	/// 入力に関する名前空間
	/// </summary>
	namespace Input
	{
		/// <summary>
		/// キーを押しているか取得する
		/// </summary>
		/// <param name="key"> キー </param>
		/// <returns> キーを押している間trueになる </returns>
		bool GetKey(KeyCode key)
		{
			return ImGui::IsKeyDown(ImGuiKey(key));
		}

		/// <summary>
		/// キーを押したか取得する
		/// </summary>
		/// <param name="key"> キー </param>
		/// <returns> キーを押した瞬間だけtrueになる </returns>
		bool GetKeyDown(KeyCode key)
		{
			return ImGui::IsKeyPressed(ImGuiKey(key), false);
		}

		/// <summary>
		/// キーを離したか取得する
		/// </summary>
		/// <param name="key"> キー </param>
		/// <returns> キーを離した瞬間だけtrueになる </returns>
		bool GetKeyUp(KeyCode key)
		{
			return ImGui::IsKeyReleased(ImGuiKey(key));
		}

		/// <summary>
		/// マウス関係
		/// </summary>
		namespace Mouse
		{
			/// <summary>
			/// ダブルクリックしたか取得する
			/// </summary>
			/// <param name="mouseButton"> マウスボタン </param>
			/// <returns>
			/// <para> true : ダブルクリックした </para>
			/// <para> false : ダブルクリックしてない </para>
			/// </returns>
			bool IsDoubleClick(MouseButton mouseButton)
			{
				return ImGui::IsMouseDoubleClicked(ImGuiMouseButton(mouseButton));
			}

			/// <summary>
			/// マウスカーソルが画面端にいるか取得する
			/// </summary>
			/// <param name="windowID"> ウィンドウ識別番号 </param>
			/// <returns>
			/// <para> true : マウスカーソルが画面端にいる </para>
			/// <para> false : マウスカーソルが画面端にいない </para>
			/// </returns>
			bool IsScreenEdge(WindowID windowID)
			{
				// メインモニターの情報
				const GLFWvidmode mainMonitor = Window::GetMainMonitor();

				// マウスカーソルの位置
				Vector2 mousePos = GetScreenPos(windowID);

				// マウスカーソルの大きさ
				Vector2 cursorSize = { 2, 35 };

				// マウスカーソルの大きさを考慮した上で、
				// マウスカーソルがメインモニターの端にいるかを返す
				return mousePos.x <= 0 || mousePos.x + cursorSize.x >= mainMonitor.width ||
					mousePos.y <= 0 || mousePos.y + cursorSize.y >= mainMonitor.height;
			}

			/// <summary>
			/// マウスカーソルの座標を取得する
			/// </summary>
			/// <param name="windowID"> ウィンドウ識別番号 </param>
			/// <returns> スクリーン座標系のマウスカーソル座標 </returns>
			Vector2 GetScreenPos(WindowID windowID)
			{
				double w = 0, h = 0;
				glfwGetCursorPos(&Window::GetWindow(windowID), &w, &h);
				return Vector2(static_cast<float>(w), static_cast<float>(h));
			}

			/// <summary>
			/// マウスカーソルの座標を取得する
			/// </summary>
			/// <param name="windowID"> ウィンドウ識別番号 </param>
			/// <returns> ビュー座標系のカーソル座標 </returns>
			Vector2 GetViewPos(WindowID windowID)
			{
				// -----------------------------------------
				// スクリーン座標系のカーソル座標を取得
				// -----------------------------------------

				// 戻り値の型に合わせるために、floatにキャスト
				const Vector2 pos = { GetScreenPos(windowID) };

				// ------------------------------------------------------
				// UILayerの座標系に合わせるために、
				// スクリーン座標系からビュー座標系(Z = -1)に変換する 
				// ------------------------------------------------------

				// ウィンドウサイズを取得
				int w = 0, h = 0;

				// 型を合わせるために、キャスト
				const Vector2 framebufferSize = { static_cast<float>(w),static_cast<float>(h) };

				// アスペクト比を取得
				const float aspectRatio = framebufferSize.x / framebufferSize.y;

				// ビュー座標系のカーソル座標を返す
				// (Y座標が逆になるので、-1を掛ける)
				return { (pos.x / framebufferSize.x * 2 - 1) * aspectRatio,
						 (pos.y / framebufferSize.y * 2 - 1) * -1 };
			}

			/// <summary>
			/// スクリーン座標系のマウスカーソル座標を設定する
			/// </summary>
			/// <param name="windowID"> ウィンドウ識別番号 </param>
			/// <param name="mousePos"> マウスカーソルの座標 </param>
			void SetScreenPos(WindowID windowID, Vector2 mousePos)
			{
				glfwSetCursorPos(&Window::GetWindow(windowID), mousePos.x, mousePos.y);
			}

		} // namespace Mouse

	} // namespace Input

} // namespace PokarinEngine