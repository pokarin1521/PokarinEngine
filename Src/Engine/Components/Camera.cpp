/**
* @file Camera.cpp
*/
#include "Camera.h"

#include "ImGui/imgui.h"

namespace PokarinEngine
{
	/// <summary>
	/// 描画
	/// </summary>
	void Camera::Draw()
	{

	}

	/// <summary>
	/// 情報を編集できるように表示する
	/// </summary>
	void Camera::InfoEditor()
	{
		ImGui::Text("Camera Information");
	}

	/// <summary>
	/// コンポーネントの情報をJson型に格納する
	/// </summary>
	/// <param name="[out] Json"> 情報を格納するJson型 </param>
	void Camera::ToJson(Json& data) const
	{

	}

	/// <summary>
	/// コンポーネントの情報をJson型から取得する
	/// </summary>
	/// <param name="[out] data"> 情報を格納しているJson型 </param>
	void Camera::FromJson(const Json& data)
	{

	}
}