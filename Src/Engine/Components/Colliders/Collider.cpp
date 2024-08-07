/**
* @file Collider.cpp
*/
#include "Collider.h"

#include "../../GameObject.h"
#include "../../Color.h"
#include "../../Shader/Shader.h"

#include "../../Mesh/Mesh.h"

#include "../../Configs/MeshConfig.h"
#include "../../Configs/ShaderConfig.h"

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

			staticMesh = Mesh::GetStaticMesh(StaticMeshFile::boxCollider);

			break;

		case Type::Sphere:

			staticMesh = Mesh::GetStaticMesh(StaticMeshFile::sphereCollider);

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
		// ライティング無しシェーダの識別番号
		GLuint progUnlit = Shader::GetProgram(Shader::ProgType::Unlit);

		// 描画に使うシェーダを指定
		glUseProgram(progUnlit);

		// 色をGPUにコピー
		// 緑色で描画する
		glProgramUniform4fv(progUnlit,
			UniformLocation::color, 1, &Color::green.r);

		// 座標変換行列
		const Matrix4x4 transformMatrix = GetTransformMatrix();

		// 座標変換行列をGPUにコピー
		glProgramUniformMatrix4fv(
			progUnlit, UniformLocation::transformMatrix,
			1, GL_FALSE, &transformMatrix[0].x);

		// 共有マテリアルを使って
		// スタティックメッシュを描画
		Mesh::Draw(staticMesh, progUnlit, staticMesh->GetMaterialList());
	}
}