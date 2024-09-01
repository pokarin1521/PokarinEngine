/**
* @file AddWorldRotationNode.cpp
*/
#include "AddWorldRotationNode.h"

#include "ImGui/imgui.h"

#include "../../../GameObject.h"
#include "../../../Math/Angle.h"
#include "../../../Time.h"
#include "../../../ImGuiHelper.h"

#include <string>

namespace PokarinEngine
{
	/// <summary>
	/// ノード別の実行処理
	/// </summary>
	/// <returns>
	/// <para> true : 次のノードを実行する </para>
	/// <para> false : 次のノードを実行しない </para>
	/// </returns>
	bool AddWorldRotationNode::RunNode()
	{
		// ゲームオブジェクト
		GameObject& gameObject = GetOnwerObject();

		// 回転量を弧度法に変換し加算する
		gameObject.transform->rotation += DegToRad(inputDegreePin->data);

		// 次のノードを実行する
		return true;
	}

	/// <summary>
	/// データピンを作成する
	/// </summary>
	void AddWorldRotationNode::CreateDataPin()
	{
		inputDegreePin = CreatePin<DataPin<Vector3>>("inputDegreePin", PinAttribute::Input);
	}

	/// <summary>
	/// データピンを表示する
	/// </summary>
	void AddWorldRotationNode::RenderDataPin()
	{
		// データピンの表示を開始
		inputDegreePin->Begin();

		// スライダーの幅
		static const float sliderWidth = 50.0f;

		// 回転量(度数法)を表示
		ImGuiHelper::DragText("Degree", GetID_String(), sliderWidth, inputDegreePin->data);

		// データピンの表示を終了
		inputDegreePin->End();
	}
}