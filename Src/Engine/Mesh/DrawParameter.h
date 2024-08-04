/**
* @file DrawParameter.h
*/
#ifndef POKARINENGINE_DRAWPARAMETER_H_INCLUDED
#define POKARINENGINE_DRAWPARAMETER_H_INCLUDED

#include "glad/glad.h"

#include <vector>

namespace PokarinEngine
{
	// -----------------------
	// �O���錾
	// -----------------------

	struct DrawParameter;

	// --------------------------------
	// �^�̕ʖ����`
	// --------------------------------

	using DrawParameterList = std::vector<DrawParameter>;

	/// <summary>
	/// �`��p�����[�^
	/// </summary>
	struct DrawParameter
	{
		// �v���~�e�B�u�̎��
		GLenum mode = GL_TRIANGLES;

		// �`�悷��C���f�b�N�X��
		GLsizei count = 0;

		// �`��J�n�C���f�b�N�X�̈ʒu(�o�C�g���Ŏw��)
		const void* indices = 0;

		// �C���f�b�N�X0�ƂȂ钸�_�z��̗v�f�ԍ�
		GLint baseVertex = 0;

		// �}�e���A���C���f�b�N�X
		int materialNo = -1;
	};
}

#endif // !POKARINENGINE_DRAWPARAMETER_H_INCLUDED
