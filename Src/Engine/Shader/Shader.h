/**
* @file Shader.h
*/
#ifndef SHADER_H_INCLUDED
#define SHADER_H_INCLUDED

#include "glad/glad.h"

#include <GLFW/glfw3.h>
#include <unordered_map>

namespace PokarinEngine
{
	/// <summary>
	/// �V�F�[�_�Ǘ��p
	/// </summary>
	namespace Shader
	{
		// --------------------------
		// �O���錾
		// --------------------------

		enum class ProgType;

		// --------------------------
		// �^�̕ʖ����`
		// --------------------------
		
		// �V�F�[�_�v���O�����̊Ǘ��ԍ��z��
		// <�V�F�[�_�v���O�����̎��, �V�F�[�_�v���O�����̊Ǘ��ԍ�>
		using ProgList = std::unordered_map<ProgType, GLuint>;

		// -------------------------------
		// ���
		// -------------------------------

		/// <summary>
		/// �V�F�[�_�v���O�����̎��
		/// </summary>
		enum class ProgType
		{
			Standard, // �W��
			Unlit,    // ���C�e�B���O���� 
		};

		// --------------------------------
		// �֐�
		// --------------------------------

		/// <summary>
		/// �V�F�[�_��������
		/// </summary>
		void Initialize();

		/// <summary>
		/// �V�F�[�_�v���O�����̊Ǘ��ԍ����擾����
		/// </summary>
		/// <param name="progType"> �V�F�[�_�v���O�����̎�� </param>
		/// <returns> �w�肵����ނ̃V�F�[�_�v���O�����̊Ǘ��ԍ� </returns>
		GLuint GetProgram(const ProgType& progType);

		/// <summary>
		/// �S�ẴV�F�[�_�v���O�����̊Ǘ��ԍ����擾����
		/// </summary>
		/// <returns> 
		/// <para> �S�ẴV�F�[�_�v���O�����̊Ǘ��ԍ� </para>
		/// <para> [�V�F�[�_�v���O�����̎��, �V�F�[�_�v���O�����̊Ǘ��ԍ�] </para>
		/// </returns>
		const ProgList& GetAllProgram();

	} // namespace Shader

} // namespace PokarinEngine

#endif // !SHADER_H_INCLUDED
