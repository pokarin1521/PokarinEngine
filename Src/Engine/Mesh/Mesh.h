/**
* @file Mesh.h
*/
#ifndef POKARINENGINE_MESH_H_INCLUDED
#define POKARINENGINE_MESH_H_INCLUDED

#include "../Texture.h"

#include "../Math/Vector.h"
#include "../Color.h"

#include <string>
#include <vector>
#include <memory>
#include <unordered_map>
#include <functional>

namespace PokarinEngine
{
	/// <summary>
	/// 共有マテリアル配列をコピー
	/// </summary>
	/// <param name="[in] original"> コピー元マテリアル配列の持ち主 </param>
	/// <returns> 共有マテリアル配列のコピー </returns>
	MaterialList CloneMaterialList(const StaticMeshPtr& original);

	/// <summary>
	/// メッシュ管理用
	/// </summary>
	namespace Mesh
	{	
		/// <summary>
		/// スタティックメッシュを取得する
		/// </summary>
		/// <param name="fileName"> スタティックメッシュのファイル名 </param>
		/// <returns> スタティックメッシュ </returns>
		StaticMeshPtr GetStaticMesh(const std::string& fileName);

		/// <summary>
		/// メッシュを描画する
		/// </summary>
		/// <param name="[in] mesh"> 描画するスタティックメッシュ </param>
		/// <param name="[in] program"> 使用するシェーダプログラムの管理番号 </param>
		/// <param name="[in] materialss"> 使用するマテリアル配列 </param>
		void Draw(const StaticMeshPtr& mesh, GLuint program, const MaterialList& materials);
	};

} // namespace PokarinEngine

#endif // !POKARINENGINE_MESH_H_INCLUDED