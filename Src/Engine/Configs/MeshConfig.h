/**
* @file MeshConfig.h
*/
#ifndef POKARINENGINE_MESHCONFIG_H_INCLUDED
#define POKARINENGINE_MESHCONFIG_H_INCLUDED

#include <vector>

namespace PokarinEngine
{
	/// <summary>
	/// �X�^�e�B�b�N���b�V���̃t�@�C����
	/// </summary>
	namespace StaticMeshFile
	{
		// ------------------------
		// ��{�`
		// ------------------------

		// ������
		inline constexpr const char* cube = "Res/MeshData/Basic/Cube/Cube.obj";

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
		// �R���C�_�[
		// ------------------------

		// Box�R���C�_�[
		inline constexpr const char* boxCollider = "Res/MeshData/Collider/Box/Box.obj";		
		
		// ���̃R���C�_�[
		inline constexpr const char* sphereCollider = "Res/MeshData/Collider/Sphere/Sphere.obj";

		// ------------------------ 
		// ���������{�b�g 
		// ------------------------

		// ���������{�b�g
		inline constexpr const char* robot = "Res/MeshData/Robot/Robot.obj";
	}

} // namespace PokarinEngine

#endif // !POKARINENGINE_MESHCONFIG_H_INCLUDED
