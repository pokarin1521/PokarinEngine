/**
* @file Camera.cpp
*/
#include "Camera.h"

#include "ImGui/imgui.h"

namespace PokarinEngine
{
	/// <summary>
	/// �`��
	/// </summary>
	void Camera::Draw()
	{

	}

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
	void Camera::ToJson(Json& data) const
	{

	}

	/// <summary>
	/// �R���|�[�l���g�̏���Json�^����擾����
	/// </summary>
	/// <param name="[out] data"> �����i�[���Ă���Json�^ </param>
	void Camera::FromJson(const Json& data)
	{

	}
}