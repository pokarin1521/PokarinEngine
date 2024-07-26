/**
* @file MeshConfig.h
*/
#ifndef MESHCONFIG_H_INCLUDED
#define MESHCONFIG_H_INCLUDED

#include <vector>

namespace PokarinEngine
{
	/// <summary>
	/// スタティックメッシュのファイル名(OBJ)
	/// </summary>
	namespace StaticMeshFile_OBJ
	{
		// ------------------------
		// 基本形
		// ------------------------

		// 直方体
		inline constexpr const char* cube = "Res/MeshData/Basic/Cube/Cube.obj";

		// 球体
		inline constexpr const char* sphere = "Res/MeshData/Basic/Sphere/Sphere.obj";

		// 板
		inline constexpr const char* plane = "Res/MeshData/Basic/Plane/Plane.obj";

		// ------------------------
		// UI用
		// ------------------------

		// UI用の板
		inline constexpr const char* ui = "Res/MeshData/UI/Plane.obj";

		// ------------------------
		// スカイスフィア
		// ------------------------

		inline constexpr const char* skySphere = "Res/MeshData/SkySphere/SkySphere.obj";

		// ------------------------
		// コライダー
		// ------------------------

		// Boxコライダー
		inline constexpr const char* boxCollider = "Res/MeshData/Collider/Box/Box.obj";		
		
		// 球体コライダー
		inline constexpr const char* sphereCollider = "Res/MeshData/Collider/Sphere/Sphere.obj";

		// ------------------------ 
		// お試しロボット 
		// ------------------------

		// お試しロボット
		inline constexpr const char* robot = "Res/MeshData/Robot/Robot.obj";

		// ------------------------
		// 全てのOBJファイル
		// ------------------------

		// 全てのOBJファイル
		inline constexpr const char* allObj[] = {
			cube, sphere, plane, ui, robot, skySphere };
	}

} // namespace PokarinEngine

#endif // !MESHCONFIG_H_INCLUDED
