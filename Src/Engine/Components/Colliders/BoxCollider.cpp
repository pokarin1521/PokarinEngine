/**
* @file BoxCollider.cpp
*/
#include "BoxCollider.h"

#include "../../GameObject.h"

namespace PokarinEngine
{
	/// <summary>
	/// 座標変換したコライダーを取得する
	/// </summary>
	/// <returns> 座標変換したコライダー </returns>
	ColliderPtr BoxCollider::GetTransformedCollider() const
	{
		// 自身の座標変換行列
		Matrix4x4 transformMatrix = GetTransformMatrix();

		// 持ち主であるゲームオブジェクトの座標変換行列
		Matrix4x4 objectMatrix = GetOwnerObject().transform->GetTransformMatrix();

		// 座標変換用コライダー
		auto collider = std::make_shared<BoxCollider>();

		// 中心座標を取得する
		collider->box.center = Matrix4x4::Translate(transformMatrix);

		// 大きさを取得する
		collider->box.size = Matrix4x4::Scale(transformMatrix);

		// 軸ベクトルと大きさを変換する
		for (int i = 0; i < Vector3::size; ++i)
		{
			// 軸ベクトルを変換
			// 向きベクトルなので、回転行列で変換する
			collider->box.axis[i] = Matrix4x4::Rotate(objectMatrix) * box.axis[i];
		}

		return collider;
	}

	/// <summary>
	/// 座標変換行列を取得する
	/// </summary>
	/// <returns> 座標変換行列 </returns>
	Matrix4x4 BoxCollider::GetTransformMatrix() const
	{
		// 持ち主であるゲームオブジェクトの位置・回転・拡大率
		TransformPtr transform = GetOwnerObject().transform;

		// 持ち主であるゲームオブジェクトの座標変換行列
		Matrix4x4 transformMatrix = transform->GetTransformMatrix();

		// 座標変換した中心座標
		const Vector3 center = Vector3(transformMatrix * Vector4(box.center, 1));

		// 拡大率を反映した大きさ
		Vector3 size = box.size * Matrix4x4::Scale(transformMatrix);

		// 座標変換行列を作成して設定する
		return Matrix4x4::CreateTransformMatrix(center, transform->rotation, size);
	}

	/// <summary>
	/// コンポーネントの情報をJson型に格納する
	/// </summary>
	/// <param name="[out] Json"> 情報を格納するJson型 </param>
	void BoxCollider::ToJson(Json& data) const
	{

	}

	/// <summary>
	/// コンポーネントの情報をJson型から取得する
	/// </summary>
	/// <param name="[out] data"> 情報を格納しているJson型 </param>
	void BoxCollider::FromJson(const Json& data)
	{

	}

	/// <summary>
	/// 情報を編集できるように表示する
	/// </summary>
	void BoxCollider::InfoEditor()
	{
		// 中心座標を表示
		box.center.RenderDrag("Center", GetID_String(), sliderWidth);

		// 大きさを表示
		box.size.RenderDrag("Size", GetID_String(), sliderWidth);
	}
}