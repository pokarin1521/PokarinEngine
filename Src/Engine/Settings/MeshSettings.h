/**
* @file MeshSettings.h
*/
#ifndef MESHSETTINGS_H_INCLUDED
#define MESHSETTINGS_H_INCLUDED

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
		// お試しロボット 
		// ------------------------

		// お試しロボット
		inline constexpr const char* robot = "Res/MeshData/Robot/Robot.obj";

		// ------------------------
		// 全てのOBJファイル
		// ------------------------

		// 全てのOBJファイル
		inline constexpr const char* allObj[] = {
			sphere, plane, ui, robot, skySphere };
	}

} // namespace PokarinEngine

#endif // !MESHSETTINGS_H_INCLUDED
