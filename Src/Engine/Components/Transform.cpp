/**
* @file Transform.cpp
*/
#include "Transform.h"

#include "../Engine.h"

namespace PokarinEngine
{
	/// <summary>
	/// ゲームオブジェクトが削除された時の処理
	/// </summary>
	void Transform::OnDestroy()
	{
		// 親オブジェクトとの親子関係を解除する
		SetParent(nullptr);

		// 子オブジェクトとの親子関係を解除する
		for (auto child : children)
		{
			child->parent = nullptr;
		}
	}

	/// <summary>
	/// pointを中心にY軸回転
	/// </summary>
	/// <param name="point"> 中心の位置 </param>
	/// <param name="rotY"> Y軸回転させる角度(弧度法) </param>
	/// <param name="range"> 中心からの距離 </param>
	void Transform::RotateAroundY(
		const Vector3& point, float rotY, float distance)
	{
		float sinY = std::sin(rotY);
		float cosY = std::cos(rotY);

		position.z = distance * cosY + point.z;
		position.x = distance * sinY + point.x;
	}

	/// <summary>
	/// 親オブジェクトを設定
	/// </summary>
	/// <param name="parent"> 
	/// <para> 親にするゲームオブジェクトのトランスフォーム </para>
	/// <para> nullptrを指定すると親子関係を解除する </para>
	/// </param>
	void Transform::SetParent(Transform* parent)
	{
		// ------------------------------------------
		// 同じ親を指定された場合は、何もしない
		// ------------------------------------------

		if (parent == this->parent)
		{
			return;
		}

		// ------------------------------------------------
		// 既に親がいる場合、その親との関係を解除する
		// ------------------------------------------------

		if (this->parent)
		{
			// 親が持つ子オブジェクト配列
			auto& c = this->parent->children;

			// 自分の位置を検索
			auto itr = std::find(c.begin(), c.end(), this);

			// 自分が子オブジェクトとして登録されている
			if (itr != c.end())
			{
				// 配列から自分を削除
				c.erase(itr);
			}
		}

		// ---------------------------
		// 新たな親子関係を設定
		// ---------------------------

		// 新たな親の存在確認
		if (parent)
		{
			// 親子関係を設定
			parent->children.push_back(this);
		}

		// 親オブジェクトに設定
		this->parent = parent;
	}

	/// <summary>
	/// 親オブジェクトを設定
	/// </summary>
	/// <param name="parent"> 
	/// <para> 親にするゲームオブジェクトのトランスフォーム </para>
	/// <para> nullptrを指定すると親子関係を解除する </para>
	/// </param>
	void Transform::SetParent(const TransformPtr& parent)
	{
		SetParent(parent.get());
	}

	/// <summary>
	/// 値をドラッグ操作用スライダーで表示する
	/// </summary>
	/// <param name="axisName"> 表示する値の名前 </param>
	/// <param name="axis"> 表示する値 </param>
	void DragValue(std::string valueName, float& value, std::string info = "")
	{
		// 識別用ラベル(非表示にしたいので##)
		const std::string label = "##" + info + "." + valueName;

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

		// ドラッグ操作用スライダーの幅を設定
		ImGui::PushItemWidth(dragSliderWidth);
		{
			// ドラッグ操作用スライダーを
			// 名前と同じ行に表示する
			ImGui::SameLine();
			ImGui::DragFloat(label.c_str(), &value, dragSpeed, 0, 0, format);

			// 幅の設定を終了
			ImGui::PopItemWidth();
		}
	}

	/// <summary>
	/// 情報をドラッグ操作用スライダーで表示する
	/// </summary>
	/// <param name="infoName"> 表示する情報の名前 </param>
	/// <param name="info"> 表示する情報 </param>
	void DragInformation(std::string infoName, Vector3& info)
	{
		// ----------------------------
		// 情報の名前を表示
		// ----------------------------

		ImGui::Text(infoName.c_str());

		// ----------------------------
		// それぞれの値を表示
		// ----------------------------

		// 値を表示し始める位置
		const float dragValueX = 90;

		ImGui::SameLine();
		ImGui::SetCursorPosX(dragValueX);
		DragValue("x", info.x, infoName);

		ImGui::SameLine();
		DragValue("y", info.y, infoName);

		ImGui::SameLine();
		DragValue("z", info.z, infoName);
	}

	/// <summary>
	/// エディタに情報を表示する
	/// </summary>
	void Transform::RenderInfo()
	{
		// 折りたたみ可能なヘッダーを表示
		// 最初から展開しておく
		if (ImGui::CollapsingHeader("Transform",
			ImGuiTreeNodeFlags_::ImGuiTreeNodeFlags_DefaultOpen))
		{
			// -----------------------
			// 位置
			// -----------------------

			// 位置
			DragInformation("Position", position);

			// ----------------------------
			// 回転角度(度数法)
			// ----------------------------

			DragInformation("Rotation", rotationDeg);

			// 弧度法に変換
			rotation = Radians(rotationDeg);

			// ----------------------------
			// 拡大率
			// ----------------------------

			DragInformation("Scale", scale);
		}
	}

} // namespace PokarinEngine
