/**
* @file Camera.cpp
*/
#include "Camera.h"

#include "ImGui/imgui.h"

namespace PokarinEngine
{
	/// <summary>
	/// ����ҏW�ł���悤�ɕ\������
	/// </summary>
	void Camera::InfoEditor()
	{
		ImGui::Text("Camera Information");
	}

	/// <summary>
	/// �R���|�[�l���g�̏���Json�^�Ɋi�[����
	/// </summary>
	/// <param name="[out] Json"> �����i�[����Json�^ </param>
	void Camera::ComponentToJson(Json& data) const
	{

	}
}