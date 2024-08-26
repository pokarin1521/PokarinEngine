/**
* @file AddWorldPositionNode.cpp
*/
#include "AddWorldPositionNode.h"

#include "../../../GameObject.h"
#include "../../../Time.h"
#include "../../../ImGuiHelper.h"

namespace PokarinEngine
{
	/// <summary>
	/// ノード別の実行処理
	/// </summary>
	/// <returns>
	/// <para> true : 次のノードを実行する </para>
	/// <para> false : 次のノードを実行しない </para>
	/// </returns>
	bool AddWorldPositionNode::RunNode()
	{
		// ゲームオブジェクト
		GameObject& gameObject = GetOnwerObject();

		// 指定された分だけ移動させる
		gameObject.transform->position += inputTranslatePin->data;

		// 次のノードを実行する
		return true;
	}

	/// <summary>
	/// データピンを作成する
	/// </summary>
	void AddWorldPositionNode::CreateDataPin() 
	{
		inputTranslatePin = CreatePin<DataPin<Vector3>>(PinAttribute::Input);
	}

	/// <summary>
	/// データピンを表示する
	/// </summary>
	void AddWorldPositionNode::RenderDataPin()
	{
		// 移動量ピンの表示を開始
		inputTranslatePin->Begin();

		static const float sliderWidth = 50.0f;

		// 移動量を表示
		ImGuiHelper::DragText("Translate", GetID_String(), sliderWidth, inputTranslatePin->data);

		// 移動量ピンの表示を終了
		inputTranslatePin->End();
	}
}