/**
* @file Collider.cpp
*/
#include "Collider.h"

#include "Json/Json.h"

#include "../../GameObject.h"
#include "../../Color.h"
#include "../../Shader/Shader.h"
#include "../../Mesh/Mesh.h"
#include "../../ImGuiHelper.h"

namespace PokarinEngine
{
	/// <summary>
	/// ゲームオブジェクトに追加された時の初期化
	/// </summary>
	void Collider::Awake()
	{
		// コライダーの種類
		const Type type = GetType();

		// 種類に合わせたメッシュを取得する
		switch (type)
		{
		case Type::Box:

			staticMesh = Mesh::GetStaticMesh("Res/MeshData/Collider/Box/Box.obj");

			break;

		case Type::Sphere:

			staticMesh = Mesh::GetStaticMesh("Res/MeshData/Collider/Sphere/Sphere.obj");

			break;
		}
	}

	/// <summary>
	/// 更新
	/// </summary>
	void Collider::Update()
	{
		// 持ち主であるゲームオブジェクトが
		// 物理挙動用コンポーネントを持っていたら衝突時に動くようにする
		isStatic = !GetOwnerObject().hasRigidbody;
	}

	/// <summary>
	/// 描画
	/// </summary>
	void Collider::Draw()
	{
		// ライティング無しシェーダ
		Shader::ProgType progUnlit = Shader::ProgType::Unlit;

		// 描画に使うシェーダを指定
		Shader::UseProgram(progUnlit);

		// 色をシェーダに設定する
		// 緑色に設定
		Shader::SetVector4(progUnlit, UniformVector4::color, Color::green);

		// 座標変換行列をシェーダに設定する
		Shader::SetMatrix4x4(progUnlit, UniformMatrix4x4::transformMatrix, GetTransformMatrix());

		// 共有マテリアルを使って
		// スタティックメッシュを描画
		Mesh::Draw(staticMesh, progUnlit, staticMesh->GetMaterialList());
	}

	/// <summary>
	/// 情報を編集できるように表示する
	/// </summary>
	void Collider::InfoEditor()
	{
		// 重複の可否を表示する
		ImGuiHelper::CheckBox("IsTrigger", GetID_String(), isTrigger);

		// コライダー別の情報を表示する
		ColliderInfoEditor();
	}

	/// <summary>
	/// コンポーネントの情報をJson型に格納する
	/// </summary>
	/// <param name="[out] json"> 情報を格納するJson型 </param>
	void Collider::ToJson(Json& json) const
	{
		// 重複の可否を格納
		json["IsTrigger"] = isTrigger;

		// コライダー別の情報を格納
		ColliderToJson(json);
	}

	/// <summary>
	/// コンポーネントの情報をJson型から取得する
	/// </summary>
	/// <param name="[in] json"> 情報を格納しているJson型 </param>
	void Collider::FromJson(const Json& json)
	{
		// 重複の可否を取得
		json["IsTrigger"].get_to(isTrigger);

		// コライダー別の情報を取得
		ColliderFromJson(json);
	}
}