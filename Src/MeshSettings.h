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
		inline constexpr char sphere[] = "Res/MeshData/Basic/Sphere/Sphere.obj";

		// ��
		inline constexpr char plane[] = "Res/MeshData/Basic/Plane/Plane.obj";

		// ------------------------
		// UI�p
		// ------------------------

		// UI�p�̔�
		inline constexpr char ui[] = "Res/MeshData/UI/Plane.obj";

		// ------------------------ 
		// ���������{�b�g 
		// ------------------------

		// ���������{�b�g
		inline constexpr char robot[] = "Res/MeshData/Robot/Robot.obj";

		// ------------------------
		// �S�Ă�OBJ�t�@�C��
		// ------------------------

		// �S�Ă�OBJ�t�@�C��
		inline constexpr const char* allObj[] = { sphere, plane, ui, robot };
	}

} // namespace PokarinEngine

#endif // !MESHSETTINGS_H_INCLUDED