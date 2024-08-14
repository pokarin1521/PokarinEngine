/**
* @file AddWorldPositionNode.cpp
*/
#include "AddWorldPositionNode.h"

#include "../../../GameObject.h"
#include "../../../Time.h"

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

		// 加算値の分だけ移動させる
		gameObject.transform->position += translate;

		// 次のノードを実行する
		return true;
	}

	/// <summary>
	/// データピンを表示する
	/// </summary>
	void AddWorldPositionNode::RenderDataPin()
	{
		// データピンの表示を開始
		BeginDataPin(inputPositionPin, PinAttribute::Input);

		// スライダーの幅
		static const float sliderWidth = 50.0f;

		// 移動量を表示
		translate.RenderDrag("Translate", GetID_String(), sliderWidth);

		// データピンの表示を終了
		EndPin(PinAttribute::Input);
	}
}