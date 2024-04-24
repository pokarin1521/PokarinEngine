/**
* @file InputManager.h
*/
#ifndef INPUTMANAGER_H_INCLUDED
#define INPUTMANAGER_H_INCLUDED

#include "glad/glad.h"
#include "ImGui/imgui.h"
#include "Math/Vector.h"
#include "Window.h"

#include <GLFW/glfw3.h>
#include <memory>

namespace PokarinEngine
{
	/// <summary>
	/// キー指定用
	/// </summary>
	enum class KeyCode
	{
		// ------------------------
		// キーボード
		// ------------------------

		LeftCtrl = ImGuiKey_LeftCtrl,
		RightCtrl = ImGuiKey_RightCtrl,
		C = ImGuiKey_C,
		D = ImGuiKey_D,
		Delete = ImGuiKey_Delete,
		LeftAlt = ImGuiKey_LeftAlt,

		// ------------------------
		// マウス
		// ------------------------

		MouseLeft = ImGuiKey_MouseLeft,
		MouseRight = ImGuiKey_MouseRight,
		MouseMiddle = ImGuiKey_MouseMiddle,
	};

	/// <summary>
	/// マウスボタン指定用
	/// </summary>
	enum class MouseButton
	{
		Left = ImGuiMouseButton_Left,
		Right = ImGuiMouseButton_Right,
		Middle = ImGuiMouseButton_Middle,
	};

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
		bool GetKey(KeyCode key);

		/// <summary>
		/// キーを押したか取得する
		/// </summary>
		/// <param name="key"> キー </param>
		/// <returns> キーを押した瞬間だけtrueになる </returns>
		bool GetKeyDown(KeyCode key);

		/// <summary>
		/// キーを離したか取得する
		/// </summary>
		/// <param name="key"> キー </param>
		/// <returns> キーを離した瞬間だけtrueになる </returns>
		bool GetKeyUp(KeyCode key);

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
			/// <para> false : ダブルクリックしていない </para>
			/// </returns>
			bool IsDoubleClick(MouseButton mouseButton);

			/// <summary>
			/// マウスカーソルが画面端にいるか取得する
			/// </summary>
			/// <param name="WindowNum"> ウィンドウ識別番号 </param>
			/// <returns>
			/// <para> true : マウスカーソルが画面端にいる </para>
			/// <para> false : マウスカーソルが画面端にいない </para>
			/// </returns>
			bool IsScreenEdge(WindowID windowID);

			/// <summary>
			/// マウスカーソルの座標を取得する
			/// </summary>
			/// <param name="windowID"> ウィンドウ識別番号 </param>
			/// <returns> スクリーン座標系のマウスカーソル座標 </returns>
			Vector2 GetScreenPos(WindowID windowID);

			/// <summary>
			/// マウスカーソルの座標を取得する
			/// </summary>
			/// <param name="windowID"> ウィンドウ識別番号 </param>
			/// <returns> ビュー座標系のカーソル座標 </returns>
			Vector2 GetViewPos(WindowID windowID);

			/// <summary>
			/// スクリーン座標系のマウスカーソル座標を設定する
			/// </summary>
			/// <param name="windowID"> ウィンドウ識別番号 </param>
			/// <param name="mousePos"> マウスカーソルの座標 </param>
			void SetScreenPos(WindowID windowID, Vector2 mousePos);

		} // namespace Mouse

	} // namespace Input

} // namespace PokarinEngine

#endif // !INPUTMANAGER_H_INCLUDED
