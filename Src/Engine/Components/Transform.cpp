/**
* @file Transform.cpp
*/
#include "Transform.h"

#include "ImGui/imgui.h"
#include "Json/Json.h"

#include "../Math/Angle.h"

#include <fstream>

namespace PokarinEngine
{
	/// <summary>
	/// 更新
	/// </summary>
	void Transform::Update()
	{
		// 位置・回転角度・拡大率を制限する
		ClampInfo();

		// 変換行列を更新する
		UpdateMatrix();
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
	/// <param name="[in,out] parent"> 親にするゲームオブジェクトのTransform </param>
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
			// 子オブジェクトに設定していた自身を削除する
			parent->children.erase(this);
		}

		// ---------------------------
		// 新たな親子関係を設定
		// ---------------------------

		if (_parent)
		{
			// 子オブジェクトとして自身を追加する
			_parent->children.emplace(this);
		}

		// 親オブジェクトに設定
		parent = _parent;
	}

	/// <summary>
	/// 位置・回転角度・拡大率の値を制限する
	/// </summary>
	void Transform::ClampInfo()
	{
		// 位置・回転角度(度数法)・拡大率の最大値
		// Unityを参考に10万で設定する
		static const float clampMax = 100000;

		// 回転角度(弧度法)の最大値
		static const float radiansClampMax = DegToRad(clampMax);

		// 位置・回転角度・拡大率を±10万の範囲になるように制限する
		for (int i = 0; i < Vector3::size; ++i)
		{
			position[i] = std::clamp(position[i], -clampMax, clampMax);
			rotation[i] = std::clamp(rotation[i], -radiansClampMax, radiansClampMax);
			scale[i] = std::clamp(scale[i], -clampMax, clampMax);
		}
	}

	/// <summary>
	/// 変換行列を更新する
	/// </summary>
	void Transform::UpdateMatrix()
	{
		// ----------------------------------
		// 自身の座標変換行列を求める
		// ----------------------------------

		// オブジェクトの座標
		// 左手座標系の値なので右手座標系にする
		Vector3 rightPosition = position;
		rightPosition.z *= -1;

		// 自身の座標変換行列
		transformMatrix = Matrix4x4::CreateTransformMatrix(
			rightPosition, rotation, scale);

		// 自身の法線変換行列
		normalMatrix = Matrix3x3::CreateRotationMatrix(rotation);

		// ------------------------------------
		// 親の座標変換行列を反映する
		// ------------------------------------

		// 親をたどっていく
		for (Transform* p = parent; p; p = p->GetParent())
		{
			// 親の座標変換行列
			Matrix4x4 parentTransformMatrix = p->transformMatrix;

			// 親の法線変換行列
			Matrix3x3 parentNormalMatrix = p->normalMatrix;

			// 親の変換行列を掛け合わせる
			transformMatrix *= parentTransformMatrix;
			normalMatrix *= parentNormalMatrix;
		}
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

		// 位置
		DragText("Position", position);

		// ----------------------------
		// 回転角度(度数法)
		// ----------------------------

		// 回転角度(度数法)
		Vector3 rotationDeg = RadToDeg(rotation);

		// 分かりやすいように度数法で表示
		DragText("Rotation", rotationDeg);

		// 弧度法に変換
		rotation = DegToRad(rotationDeg);

		// ----------------------------
		// 拡大率
		// ----------------------------

		DragText("Scale", scale);
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
	/// <param name="[in] data"> 情報を格納しているJson型 </param>
	void Transform::FromJson(const Json& data)
	{
		position.FromJson(data["Position"]);
		rotation.FromJson(data["Rotation"]);
		scale.FromJson(data["Scale"]);
	}

} // namespace PokarinEngine
