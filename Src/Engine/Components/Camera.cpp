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

	/// <summary>
	/// コンポーネントの情報を保存する
	/// </summary>
	/// <param name="[in] folderName"> 保存先のフォルダ </param>
	void Camera::SaveInfo(const std::string& folderName) const
	{

	}
}