/**
* @file Shader.h
*/
#ifndef POKARINENGINE_SHADER_H_INCLUDED
#define POKARINENGINE_SHADER_H_INCLUDED

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
		
		// �V�F�[�_�v���O�����̎��ʔԍ��z��
		// <�V�F�[�_�v���O�����̎��, �V�F�[�_�v���O�����̎��ʔԍ�>
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
		/// �V�F�[�_�v���O�����̎��ʔԍ����擾����
		/// </summary>
		/// <param name="[in] progType"> �V�F�[�_�v���O�����̎�� </param>
		/// <returns> �w�肵����ނ̃V�F�[�_�v���O�����̎��ʔԍ� </returns>
		GLuint GetProgram(ProgType progType);

		/// <summary>
		/// �S�ẴV�F�[�_�v���O�����̎��ʔԍ����擾����
		/// </summary>
		/// <returns> 
		/// <para> �S�ẴV�F�[�_�v���O�����̎��ʔԍ� </para>
		/// <para> [�V�F�[�_�v���O�����̎��, �V�F�[�_�v���O�����̎��ʔԍ�] </para>
		/// </returns>
		const ProgList& GetAllProgram();

	} // namespace Shader

} // namespace PokarinEngine

#endif // !POKARINENGINE_SHADER_H_INCLUDED
