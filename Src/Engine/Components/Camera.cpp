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
	/// �R���|�[�l���g�̏���ۑ�����
	/// </summary>
	/// <param name="[in] folderName"> �ۑ���̃t�H���_ </param>
	void Camera::SaveInfo(const std::string& folderName) const
	{

	}
}