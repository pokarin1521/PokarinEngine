/**
* @file Camera.cpp
*/
#include "Camera.h"

#include "ImGui/imgui.h"

namespace PokarinEngine
{
	/// <summary>
	/// エディタに情報を表示する
	/// </summary>
	void Camera::RenderInfo()
	{
		if (ImGui::CollapsingHeader("Camera"))
		{
			ImGui::Text("Camera Information");
		}
	}
}