/**
* @file MeshSettings.h
*/
#ifndef MESHSETTINGS_H_INCLUDED
#define MESHSETTINGS_H_INCLUDED

#include <vector>

namespace PokarinEngine
{
	/// <summary>
	/// �X�^�e�B�b�N���b�V���̃t�@�C����(OBJ)
	/// </summary>
	namespace StaticMeshFile_OBJ
	{
		// ------------------------
		// ��{�`
		// ------------------------

		// ����
		inline constexpr const char* sphere = "Res/MeshData/Basic/Sphere/Sphere.obj";

		// ��
		inline constexpr const char* plane = "Res/MeshData/Basic/Plane/Plane.obj";

		// ------------------------
		// UI�p
		// ------------------------

		// UI�p�̔�
		inline constexpr const char* ui = "Res/MeshData/UI/Plane.obj";

		// ------------------------
		// �X�J�C�X�t�B�A
		// ------------------------

		inline constexpr const char* skySphere = "Res/MeshData/SkySphere/SkySphere.obj";

		// ------------------------ 
		// ���������{�b�g 
		// ------------------------

		// ���������{�b�g
		inline constexpr const char* robot = "Res/MeshData/Robot/Robot.obj";

		// ------------------------
		// �S�Ă�OBJ�t�@�C��
		// ------------------------

		// �S�Ă�OBJ�t�@�C��
		inline constexpr const char* allObj[] = {
			sphere, plane, ui, robot, skySphere };
	}

} // namespace PokarinEngine

#endif // !MESHSETTINGS_H_INCLUDED
