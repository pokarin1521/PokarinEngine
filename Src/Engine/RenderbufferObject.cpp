/**
* @file RenderbufferObject.cpp
*/
#include "RenderbufferObject.h"

namespace PokarinEngine
{
	/// <summary>
	/// RBO���쐬����
	/// </summary>
	/// <param name="width"> �� </param>
	/// <param name="height"> ���� </param>
	/// <param name="internalformat"> �ێ�������̃o�b�t�@ </param>
	RenderbufferObject::RenderbufferObject(
		GLsizei width, GLsizei height, GLenum internalformat)
	{
		// --------------------------------------------
		// �[�x�o�b�t�@�p�����_�[�o�b�t�@���쐬
		// --------------------------------------------

		glCreateRenderbuffers(1, &id);

		// ---------------------------------------
		// ����ێ��ł���悤�ɕR�Â���
		// ---------------------------------------

		// �ݒ�ł���悤�Ƀo�C���h
		glBindRenderbuffer(GL_RENDERBUFFER, id);

		// �w�肳�ꂽ����ێ��ł���悤�ɕR�Â�
		glRenderbufferStorage(
			GL_RENDERBUFFER, internalformat, width, height);

		// �둀��̂Ȃ��悤�Ƀo�C���h����
		glBindRenderbuffer(GL_RENDERBUFFER, 0);
	}

	/// <summary>
	/// RBO���폜����f�X�g���N�^
	/// </summary>
	RenderbufferObject::~RenderbufferObject()
	{
		glDeleteRenderbuffers(1, &id);
	}

} // namespace PokarinEngine