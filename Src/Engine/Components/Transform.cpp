/**
* @file Transform.cpp
*/
#include "Transform.h"

#include "ImGui/imgui.h"
#include "Json/Json.h"

#include "../Engine.h"

#include <fstream>

// Vector3型のfor文
#define ForVector3() for(int i = 0; i < 3; ++i)

namespace PokarinEngine
{
	/// <summary>
	/// 更新
	/// </summary>
	void Transform::Update()
	{
		// -------------------------------
		// 位置を制限
		// -------------------------------

		// 位置の最大値
		// Unityを参考に10万で設定する
		static const float positionMax = 100000;

		// 位置を±10万の範囲になるように制限する
		ForVector3()
		{
			// プラス方向の制限
			if (position[i] > positionMax)
			{
				position[i] = positionMax;
			}

			// マイナス方向の制限
			if (position[i] < -positionMax)
			{
				position[i] = -positionMax;
			}
		}

		// -------------------------------
		// 回転角度を制限
		// -------------------------------

		// 回転角度の最大値
		static const float rotationMax = Radians(360.0f);

		// 回転角度を±360度の範囲になるように制限する
		ForVector3()
		{
			if (std::abs(rotation[i]) > rotationMax)
			{
				// 周回数
				int laps = static_cast<int>(rotation[i] / rotationMax);

				// 周回数に応じて回転角度の最大値を減らすことで、
				// 制限したうえでの適切な数値を求める
				rotation[i] -= rotationMax * laps;
			}
		}

		// -------------------------------
		// 拡大率
		// -------------------------------

		// 拡大率の最大値
		// 位置と同じにしておく
		static const float scaleMax = 100000;

		// 拡大率を±10万の範囲になるように制限する
		ForVector3()
		{
			// プラス方向の制限
			if (scale[i] > scaleMax)
			{
				scale[i] = scaleMax;
			}

			// マイナス方向の制限
			if (scale[i] < -scaleMax)
			{
				scale[i] = -scaleMax;
			}
		}
	}

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
	/// 親オブジェクトを設定する
	/// </summary>
	/// <param name="[out] parent"> 
	/// <para> 親にするゲームオブジェクトのトランスフォーム </para>
	/// <para> nullptrを指定すると親子関係を解除する </para>
	/// </param>
	void Transform::SetParent(Transform* _parent)
	{
		// ------------------------------------------
		// 同じ親を指定された場合は、何もしない
		// ------------------------------------------

		if (_parent == parent)
		{
			return;
		}

		// ------------------------------------------------
		// 既に親がいる場合、その親との関係を解除する
		// ------------------------------------------------

		if (parent)
		{
			// 親が持つ子オブジェクト配列
			auto& c = parent->children;

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
		if (_parent)
		{
			// 親子関係を設定
			_parent->children.push_back(this);
		}

		// 親オブジェクトに設定
		this->parent = parent;
	}

	/// <summary>
	/// 親オブジェクトを設定する
	/// </summary>
	/// <param name="[out] parent"> 
	/// <para> 親にするゲームオブジェクトのトランスフォーム </para>
	/// <para> nullptrを指定すると親子関係を解除する </para>
	/// </param>
	void Transform::SetParent(const TransformPtr& parent)
	{
		SetParent(parent.get());
	}

	/// <summary>
	/// 情報を編集できるように表示する
	/// </summary>
	void Transform::InfoEditor()
	{
		// --------------------------------------
		// 表示の開始位置と幅を設定
		// --------------------------------------

		// ドラッグ操作用スライダーのImGuiウィンドウ幅に対する割合
		static const float sliderRatio = 6.0f;

		// ドラッグ操作用スライダーの幅
		// ImGuiウィンドウの幅に合わせる
		const float sliderWidth = ImGui::GetWindowWidth() / sliderRatio;

		// 値表示の開始位置
		static const float startX = 90.0f;

		// -----------------------
		// 位置
		// -----------------------

		// 識別番号の文字列
		const std::string id_string = GetID_String();

		// 位置
		position.RenderDrag("Position", id_string, sliderWidth, startX);

		// ----------------------------
		// 回転角度(度数法)
		// ----------------------------

		// 回転角度(度数法)
		Vector3 rotationDeg = Degrees(rotation);

		// 分かりやすいように度数法で表示
		rotationDeg.RenderDrag("Rotation", id_string, sliderWidth, startX);

		// 弧度法に変換
		rotation = Radians(rotationDeg);

		// ----------------------------
		// 拡大率
		// ----------------------------

		scale.RenderDrag("Scale", id_string, sliderWidth, startX);
	}

	/// <summary>
	/// コンポーネントの情報をJson型に格納する
	/// </summary>
	/// <param name="[out] Json"> 情報を格納するJson型 </param>
	void Transform::ToJson(Json& data) const
	{
		// ------------------------------------
		// 情報をJsonに格納する
		// ------------------------------------

		position.ToJson(data["Position"]);
		rotation.ToJson(data["Rotation"]);
		scale.ToJson(data["Scale"]);
	}

	/// <summary>
	/// コンポーネントの情報をJson型から取得する
	/// </summary>
	/// <param name="[out] data"> 情報を格納しているJson型 </param>
	void Transform::FromJson(const Json& data)
	{
		position.FromJson(data["Position"]);
		rotation.FromJson(data["Rotation"]);
		scale.FromJson(data["Scale"]);
	}

} // namespace PokarinEngine
