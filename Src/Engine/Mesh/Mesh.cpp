/**
* @file Mesh.cpp
*/
#include "Mesh.h"

#include "MeshBuffer.h"
#include "Vertex.h"

#include "../Debug.h" 
#include "../TextureGetter.h"

namespace PokarinEngine
{
	/// <summary>
	/// 初期化
	/// </summary>
	void Mesh::Initialize()
	{
		// -----------------------------------
		// メッシュバッファを作成する
		// -----------------------------------

		// メッシュバッファのサイズ
		// 32メガバイトとし、足りない場合は大きくするかメッシュバッファを増やす
		static const size_t meshBufferSize = 32'000'000;

		// 補助クラスを用いて
		// メッシュバッファを作成する
		meshBuffer = std::make_shared<MeshBuffer>(meshBufferSize);
	}

	/// <summary>
	/// スタティックメッシュを取得する
	/// </summary>
	/// <param name="fileName"> スタティックメッシュのファイル名 </param>
	/// <returns> スタティックメッシュ </returns>
	StaticMeshPtr Mesh::GetStaticMesh(const std::string& fileName)
	{
		return meshBuffer->GetStaticMesh(fileName);
	}

	/// <summary>
	/// メッシュを描画する
	/// </summary>
	/// <param name="[in] mesh"> 描画するスタティックメッシュ </param>
	/// <param name="[in] progType"> 使用するシェーダプログラムの種類 </param>
	/// <param name="[in] materialList"> 使用するマテリアル配列 </param>
	void Mesh::Draw(const StaticMeshPtr& mesh, Shader::ProgType progType, const MaterialList& materialList)
	{
		// メッシュがなければ何もしない
		if (!mesh)
		{
			return;
		}

		// カラーパラメータを取得
		Color objectColor = Color::white;

		// シェーダからオブジェクトの色を取得
		objectColor = Shader::GetVector4(progType, UniformVector4::color);

		/* 1つのOBJファイルには、
		異なるマテリアルを使う複数の図形を定義することができる
		しかし、glDrawElementsBaseVertex関数で使えるマテリアルは1種類だけ

		なので、マテリアルごとに描画データを定義し、
		描画データ単位で描画する必要がある

		これに対応するため、StaticMesh構造体は
		DrawParamsを配列で管理するようにしている */

		// メッシュの描画パラメータ配列
		const DrawParameterList drawParameterList = mesh->GetDrawParameterList();

		// VAOをバインド
		glBindVertexArray(*meshBuffer->GetVAO());

		// パラメータごとに描画
		for (const auto& drawParameter : drawParameterList)
		{
			// マテリアルを設定
			// マテリアルがあるか確認
			if (drawParameter.materialNo >= 0 && drawParameter.materialNo < materialList.size())
			{
				// マテリアル取得
				const Material& material = *materialList[drawParameter.materialNo];

				// マテリアルを反映したオブジェクトの色
				const Color color = objectColor * material.baseColor;

				// マテリアルを反映したオブジェクトの色を
				// シェーダに設定する
				Shader::SetVector4(progType, UniformVector4::color, color);

				// 発光色テクスチャの識別番号
				float emissionTextureID = 0;

				// 発光色テクスチャがあるなら識別番号を設定する
				if (material.emissionTexture)
				{
					emissionTextureID = static_cast<float>(*material.emissionTexture);
				}

				// 発光色と発光色テクスチャの識別番号
				Vector4 emission = {
					material.emission.r, material.emission.g, material.emission.b,
					emissionTextureID };

				// 発光色と
				// 発光色テクスチャの識別番号をシェーダに設定する
				Shader::SetVector4(progType, UniformVector4::emission, emission);

				// マテリアルにテクスチャが設定されているなら
				if (material.baseTexture)
				{
					// 通常の色用テクスチャをバインド
					Shader::BindTexture(TextureUnit::color, material.baseTexture);
				}
				else
				{
					// テクスチャがないのでバインド解除
					Shader::UnBindTexture(TextureUnit::color);
				}

				// エミッションテクスチャがある
				if (material.emissionTexture)
				{
					// エミッションテクスチャをバインド
					Shader::BindTexture(TextureUnit::emission, material.emissionTexture);
				}
				else
				{
					// エミッションテクスチャがないのでバインド解除
					Shader::UnBindTexture(TextureUnit::emission);
				}
			}

			// 描画
			glDrawElementsBaseVertex(
				drawParameter.mode, drawParameter.count, GL_UNSIGNED_SHORT,
				drawParameter.indices, drawParameter.baseVertex);
		}

		// VAOのバインド解除
		glBindVertexArray(0);
	}

} // namespace PokarinEngine
