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
	/// ���L�}�e���A���z����R�s�[
	/// </summary>
	/// <param name="[in] original"> �R�s�[���}�e���A���z��̎����� </param>
	/// <returns> ���L�}�e���A���z��̃R�s�[ </returns>
	MaterialList CloneMaterialList(const StaticMeshPtr& original);

	/// <summary>
	/// ���b�V���Ǘ��p
	/// </summary>
	namespace Mesh
	{	
		/// <summary>
		/// �X�^�e�B�b�N���b�V�����擾����
		/// </summary>
		/// <param name="fileName"> �X�^�e�B�b�N���b�V���̃t�@�C���� </param>
		/// <returns> �X�^�e�B�b�N���b�V�� </returns>
		StaticMeshPtr GetStaticMesh(const std::string& fileName);

		/// <summary>
		/// ���b�V����`�悷��
		/// </summary>
		/// <param name="[in] mesh"> �`�悷��X�^�e�B�b�N���b�V�� </param>
		/// <param name="[in] program"> �g�p����V�F�[�_�v���O�����̊Ǘ��ԍ� </param>
		/// <param name="[in] materialss"> �g�p����}�e���A���z�� </param>
		void Draw(const StaticMeshPtr& mesh, GLuint program, const MaterialList& materials);
	};

} // namespace PokarinEngine

#endif // !POKARINENGINE_MESH_H_INCLUDED