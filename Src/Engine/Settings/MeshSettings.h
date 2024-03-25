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
		inline constexpr char sphere[] = "Res/MeshData/Basic/Sphere/Sphere.obj";

		// 板
		inline constexpr char plane[] = "Res/MeshData/Basic/Plane/Plane.obj";

		// ------------------------
		// UI用
		// ------------------------

		// UI用の板
		inline constexpr char ui[] = "Res/MeshData/UI/Plane.obj";

		// ------------------------ 
		// お試しロボット 
		// ------------------------

		// お試しロボット
		inline constexpr char robot[] = "Res/MeshData/Robot/Robot.obj";

		// ------------------------
		// 全てのOBJファイル
		// ------------------------

		// 全てのOBJファイル
		inline constexpr const char* allObj[] = { sphere, plane, ui, robot };
	}

} // namespace PokarinEngine

#endif // !MESHSETTINGS_H_INCLUDED
