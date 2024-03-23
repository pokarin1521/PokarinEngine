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

		/// <summary>
		/// �V�F�[�_�v���O�����̎��
		/// </summary>
		enum class ProgType
		{
			Standard, // �W��
			Unlit,    // ���C�e�B���O���� 
		};

		/// <summary>
		/// �V�F�[�_��������
		/// </summary>
		/// <param name="[out] progList"> �V�F�[�_�v���O�����̊Ǘ��ԍ��z�� </param>
		void Initialize(ProgList& progList);

	} // namespace Shader

} // namespace PokarinEngine

#endif // !SHADER_H_INCLUDED
