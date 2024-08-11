/**
* @file Mesh.cpp
*/
#include "Mesh.h"

#include "MeshBuffer.h"
#include "Vertex.h"

#include "../Debug.h" 
#include "../TextureGetter.h"
#include "../ShaderConfig.h"

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
	/// <param name="[in] program"> 使用するシェーダプログラムの識別番号 </param>
	/// <param name="[in] materialList"> 使用するマテリアル配列 </param>
	void Mesh::Draw(const StaticMeshPtr& mesh, GLuint program, const MaterialList& materialList)
	{
		// メッシュがなければ何もしない
		if (!mesh)
		{
			return;
		}

		// カラーパラメータを取得
		Color objectColor = Color::white;

		// シェーダからオブジェクトの色を取得
		if (program)
		{
			glGetUniformfv(program, 100, &objectColor.r);
		}

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

				if (program)
				{
					// マテリアルを反映したオブジェクトの色
					const Color color = objectColor * material.baseColor;

					// マテリアルを反映したオブジェクトの色を
					// GPUにコピー
					glProgramUniform4fv(program, ShaderConfig::Uniform::color, 1, &color.r);

					// 発光色と
					// エミッションテクスチャの識別番号をGPUにコピー
					glProgramUniform4f(program, ShaderConfig::Uniform::emissionColor,
						material.emission.r,
						material.emission.g,
						material.emission.b,
						static_cast<bool>(material.texEmission));
				}

				// マテリアルにテクスチャが設定されているなら
				if (material.texBaseColor)
				{
					// テクスチャの識別番号
					const GLuint tex = *material.texBaseColor;

					// 通常の色用テクスチャをバインド
					glBindTextures(ShaderConfig::Texture::color, 1, &tex);

				}
				else
				{
					// テクスチャがないのでバインド解除
					glBindTextures(ShaderConfig::Texture::color, 1, 0);
				}

				// エミッションテクスチャがある
				if (material.texEmission)
				{
					// エミッションテクスチャをバインド
					const GLuint tex = *material.texEmission;
					glBindTextures(ShaderConfig::Texture::emission, 1, &tex);
				}
				else
				{
					// エミッションテクスチャがないので
					// バインド解除
					glBindTextures(ShaderConfig::Texture::emission, 1, 0);
				}
			}

			// 描画
			glDrawElementsBaseVertex(
				drawParameter.mode, drawParameter.count, GL_UNSIGNED_SHORT, drawParameter.indices, drawParameter.baseVertex);
		}

		// VAOのバインド解除
		glBindVertexArray(0);
	}

} // namespace PokarinEngine
