/**
* @file EitorInforSetter.cpp
*/
#include "EditorInfoRenderer.h"

#include "ImGui/imgui.h"

namespace PokarinEngine
{
	/// <summary>
	/// エディタに情報を表示するための機能
	/// </summary>
	namespace EditorInfoRenderer
	{
		/// ここでしか使わないので、cppのみに書く
		/// <summary>
		/// 値をドラッグ操作用スライダーで表示する
		/// </summary>
		/// <param name="[in,out] value"> 表示する値 </param>
		/// <param name="[in] valueName"> 表示する値の名前 </param>
		/// <param name="[in] title"> 表示する情報のタイトル </param>
		/// <param name="[in] id_string"> 識別番号の文字列 </param>
		void DragValue(float& value, const std::string& valueName, 
			const std::string& title, const std::string& id_string)
		{
			// 識別用ラベル(非表示にしたいので##)
			const std::string label = "##" + title + id_string + "." + valueName;

			// ドラッグ操作用スライダーのImGuiウィンドウ幅に対する割合
			static const float sliderRatio = 6.0f;

			// ドラッグ操作用スライダーの幅
			// ImGuiウィンドウの幅に合わせる
			const float dragSliderWidth = ImGui::GetWindowWidth() / sliderRatio;

			// ドラッグ操作の速度
			static const float dragSpeed = 0.2f;

			// 表示する桁数を指定するフォーマット
			// 小数2桁まで表示する
			static const char* format = "%.2f";

			// 値の名前を表示
			ImGui::Text(valueName.c_str());

			// ドラッグ操作用スライダーを
			// 名前と同じ行に表示する
			ImGui::SameLine();
			ImGui::DragFloat(label.c_str(), &value, dragSpeed, 0, 0, format);
		}

		/// <summary>
		///	Vector3型の情報をドラッグ操作用スライダーで表示する
		/// </summary>
		/// <param name="[in,out] info"> 表示する情報 </param>
		/// <param name="[in] title"> 表示するタイトル </param>
		/// <param name="[in] id_string"> 識別番号の文字列 </param>
		/// <param name="[in] width"> 表示するスライダーの幅 </param>
		/// <param name="[in] startX"> 表示の開始位置(X座標) </param>
		void DragVector3(Vector3& info, const std::string& title, const std::string& id_string,
			const float width, const float startX)
		{
			// ----------------------------
			// 情報の名前を表示
			// ----------------------------

			// 名前と同じ行に値を表示できるようにする
			ImGui::Text(title.c_str());
			ImGui::SameLine();

			// ----------------------------
			// それぞれの値を表示
			// ----------------------------

			// 開始位置を設定
			if (startX != 0)
			{
				ImGui::SetCursorPosX(startX);
			}

			// 幅を設定
			ImGui::PushItemWidth(width);
			{
				DragValue(info.x, "X", title, id_string);

				ImGui::SameLine();
				DragValue(info.y, "Y", title, id_string);

				ImGui::SameLine();
				DragValue(info.z, "Z", title, id_string);

				ImGui::PopItemWidth();
			}
		}
	}
}