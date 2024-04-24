/**
* @file AddRotationNode.cpp
*/
#include "AddRotationNode.h"

#include "ImGui/imgui.h"

#include "../../../GameObject.h"
#include "../../../Time.h"

#include <string>

namespace PokarinEngine
{
	/// <summary>
	/// 実行処理
	/// </summary>
	void AddRotationNode::Run()
	{
		// ゲームオブジェクト
		GameObject& gameObject = GetGameObject();

		gameObject.transform->rotation += addRotation * Time::DeltaTime();
	}

	/// ここでしか使わないので、cppのみに書く
	/// <summary>
	/// ドラッグ操作用スライダーで値を表示する
	/// </summary>
	/// <param name="valueName"> 値の名前 </param>
	/// <param name="value"> 値 </param>
	void DragValue(const std::string& valueName, float* value)
	{
		// 識別用ラベル
		std::string label = "##AddRotation_" + valueName;

		// データ入力用スライダーの幅
		static const float sliderWidth = 50.0f;

		// ドラッグ速度
		static const float dragSpeed = 1.0f;

		// 表示する桁数を指定するフォーマット
		// 小数1桁まで表示する
		static const char* format = "%.1f";

		// スライダーの幅を設定
		ImGui::PushItemWidth(sliderWidth);
		{
			// データ名を表示
			ImGui::Text(valueName.c_str());

			// データ名と同じ行にスライダー表示
			ImGui::SameLine();
			ImGui::DragFloat(label.c_str(), value, dragSpeed, 0, 0, format);

			ImGui::PopItemWidth();
		}
	}

	/// ここでしか使わないので、cppのみに書く
	/// <summary>
	/// ドラッグ操作用スライダーで回転角度を表示する
	/// </summary>
	/// <param name="data"> データ </param>
	void DragRotation(Vector3& rotation)
	{
		// X軸回転角度を表示
		DragValue("X", &rotation.x);

		// 同じ行にY軸回転角度を表示
		ImGui::SameLine();
		DragValue("Y", &rotation.y);

		// 同じ行にZ軸回転角度を表示
		ImGui::SameLine();
		DragValue("Z", &rotation.z);
	}

	/// <summary>
	/// データピンを表示する
	/// </summary>
	void AddRotationNode::RenderDataPin()
	{
		// データピンの設定を開始
		BeginDataPin(inputRotationPin, PinAttribute::Input);

		// 回転角度の加算値を表示
		DragRotation(addRotation);

		// データピンの設定を終了
		EndPin(PinAttribute::Input);
	}
}