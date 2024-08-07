/**
* @file SkySphere.cpp
*/
#include "SkySphere.h"

#include "Mesh/Mesh.h"

#include "Configs/MeshConfig.h"

namespace PokarinEngine
{
	/// <summary>
	/// スカイスフィア管理用
	/// </summary>
	namespace SkySphere
	{
		// -------------------------
		// 変数
		// -------------------------

		// スカイスフィア用メッシュ
		StaticMeshPtr skySphere;

		// -------------------------
		// 関数
		// -------------------------

		/// <summary>
		/// スカイスフィア用メッシュを読み込む
		/// </summary>
		/// <returns> スカイスフィア用メッシュ </returns>
		StaticMeshPtr Load()
		{
			Mesh::GetStaticMesh(StaticMeshFile::skySphere);
		}

		/// <summary>
		/// スカイスフィアを描画する
		/// </summary>
		void Draw(const Camera& camera)
		{
			
		}
	}
}