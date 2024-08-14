/**
* @file Collider.cpp
*/
#include "Collider.h"

#include "../../GameObject.h"
#include "../../Color.h"
#include "../../Shader/Shader.h"
#include "../../Mesh/Mesh.h"

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
		isStatic = !GetOwnerObject().HasRigidbody();
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
}