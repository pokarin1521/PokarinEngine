/**
* @file Camera.cpp
*/
#include "Camera.h"

#include "ImGui/imgui.h"

namespace PokarinEngine
{
	/// <summary>
	/// �G�f�B�^�ɏ���\������
	/// </summary>
	void Camera::RenderEditor()
	{
		if (ImGui::CollapsingHeader("Camera"))
		{
			ImGui::Text("Camera Information");
		}
	}
}