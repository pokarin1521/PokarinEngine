/**
* @file Pin.cpp
*/
#include "Pin.h"

namespace PokarinEngine
{
	/// <summary>
	/// ピンの表示を開始する
	/// </summary>
	void Pin::Begin()
	{
		// ピンの形
		ImNodesPinShape pinShape = ImNodesPinShape(GetType());

		// 入力用ピン
		if (attribute == PinAttribute::Input)
		{
			ImNodes::BeginInputAttribute(id, pinShape);
		}
		// 出力用ピン
		else
		{
			ImNodes::BeginOutputAttribute(id, pinShape);
		}
	}

	/// <summary>
	/// ピンの表示を終了する
	/// </summary>
	void Pin::End()
	{
		// 入力用ピン
		if (attribute == PinAttribute::Input)
		{
			ImNodes::EndInputAttribute();
		}
		// 出力用ピン
		else
		{
			ImNodes::EndOutputAttribute();
		}
	}
}