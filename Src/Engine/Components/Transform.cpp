/**
* @file Transform.cpp
*/
#include "Transform.h"

#include "ImGui/imgui.h"

#include "../Engine.h"
#include "../EditorInfoRenderer.h"

#include"../Math/VectorToJson.h"

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
		// 座標を制限
		// -------------------------------

		// 座標の最大値
		// Unityを参考に10万で設定する
		static const float positionMax = 100000;

		// 座標を±10万の範囲になるように制限する
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
		// 座標と同じにしておく
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
		EditorInfoRenderer::DragVector3(position, "Position", id_string, sliderWidth, startX);

		// ----------------------------
		// 回転角度(度数法)
		// ----------------------------

		// 回転角度(度数法)
		Vector3 rotationDeg = Degrees(rotation);

		// 分かりやすいように度数法で表示
		EditorInfoRenderer::DragVector3(rotationDeg, "Rotation", id_string, sliderWidth, startX);

		// 弧度法に変換
		rotation = Radians(rotationDeg);

		// ----------------------------
		// 拡大率
		// ----------------------------

		EditorInfoRenderer::DragVector3(scale, "Scale", id_string, sliderWidth, startX);
	}

	/// <summary>
	/// コンポーネントの情報を保存する
	/// </summary>
	/// <param name="[in] folderName"> 保存先のフォルダ </param>
	void Transform::SaveInfo(const std::string& folderName) const
	{
		// ------------------------------------
		// 情報をJsonに格納する
		// ------------------------------------

		// 情報を格納するJson
		Json data;

		// 名前を格納する
		//data["ID"] = GetName();

		// 情報を格納する
		VectorToJson(position, data["Position"]);
		VectorToJson(rotation, data["Rotation"]);
		VectorToJson(scale, data["Scale"]);

		//// -------------------------------------
		//// ファイルに保存する
		//// -------------------------------------

		//std::string fileName = folderName + "/" + std::to_string(GetID());

		//std::ofstream file;
	}

} // namespace PokarinEngine
