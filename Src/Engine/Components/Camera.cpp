/**
* @file Camera.cpp
*/
#include "Camera.h"

#include "ImGui/imgui.h"

namespace PokarinEngine
{
	/// <summary>
	/// 情報を編集できるように表示する
	/// </summary>
	void Camera::InfoEditor()
	{
		ImGui::Text("Camera Information");
	}
}