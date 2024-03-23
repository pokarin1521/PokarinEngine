/**
* FramebufferObject.cpp
*/
#include "FramebufferObject.h"

#include "Engine.h"
#include "Window.h"
#include "Texture.h"
#include "RenderbufferObject.h"

namespace PokarinEngine
{
	/// <summary>
	/// FBO���쐬����R���X�g���N�^
	/// </summary>
	/// <param name="engine"> �G���W���N���X�̎Q�� </param>
	/// <param name="_windowID"> �E�B���h�E�ԍ� </param>
	/// <param name="width"> FBO�̕� </param>
	/// <param name="height"> FBO�̍��� </param>
	FramebufferObject::FramebufferObject(
		Engine& engine, WindowID _windowID, GLsizei width, GLsizei height)
	{
		// -------------------------------
		// �E�B���h�E�ԍ���ݒ�
		// -------------------------------

		windowID = _windowID;

		// -------------------------------
		// FBO�̑傫����ݒ�
		// -------------------------------

		fboWidth = width;
		fboHeight = height;

		// -------------------------------------------
		// �J���[�o�b�t�@�p�e�N�X�`�����쐬
		// -------------------------------------------

		texture = engine.GetTexture(fboWidth, fboHeight);

		// -------------------------------
		// �[�x�o�b�t�@�pRBO���쐬
		// -------------------------------

		rbo = RenderbufferObject::Create(
			fboWidth, fboHeight, GL_DEPTH_COMPONENT32F);

		// -------------------------
		// FBO���쐬
		// -------------------------

		// FBO���쐬
		glCreateFramebuffers(1, &id);

		// �J���[�o�b�t�@�p�e�N�X�`�������蓖�Ă�
		glNamedFramebufferTexture(id, GL_COLOR_ATTACHMENT0, *texture, 0);

		// �[�x�o�b�t�@�p�e�N�X�`�������蓖�Ă�
		glNamedFramebufferRenderbuffer(
			id, GL_DEPTH_ATTACHMENT, GL_RENDERBUFFER, *rbo);

		// FBO������ɍ쐬�ł������m�F����
		if (glCheckNamedFramebufferStatus(
			id, GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
		{
			LOG_WARNING("FBO�̍쐬�Ɏ��s���܂���");

			// FBO�폜
			glDeleteFramebuffers(1, &id);

			// �Ǘ��ԍ���������
			id = 0;
		}
	}

	/// <summary>
	/// FBO���폜����f�X�g���N�^
	/// </summary>
	FramebufferObject::~FramebufferObject()
	{
		glDeleteFramebuffers(1, &id);
	}

	/// <summary>
	/// FBO���o�C���h����
	/// </summary>
	void FramebufferObject::Bind() const
	{
		// FBO���o�C���h
		glBindFramebuffer(GL_FRAMEBUFFER, id);

		// �r���[�|�[�g��FBO�̑傫���Őݒ�
		glViewport(0, 0, fboWidth, fboHeight);

		// �[�x�o�b�t�@���N���A
		glClear(GL_DEPTH_BUFFER_BIT);
	}

	/// <summary>
	/// FBO�̃o�C���h����������
	/// </summary>
	void FramebufferObject::UnBind() const
	{
		// FBO�̃o�C���h����
		glBindFramebuffer(GL_FRAMEBUFFER, 0);

		// OpenGL�E�B���h�E�̑傫�����擾
		GLsizei windowWidth = 0, windowHeight = 0;
		Window::GetWindowSize(windowID, windowWidth, windowHeight);

		// �r���[�|�[�g��OpenGL�E�B���h�E�̑傫���ɖ߂�
		glViewport(0, 0, windowWidth, windowHeight);
	}

	/// <summary>
	/// �J���[�o�b�t�@�p�e�N�X�`�����N���A
	/// </summary>
	void FramebufferObject::ClearColor(const Color& color)
	{
		glBindFramebuffer(GL_FRAMEBUFFER, id);

		// �o�b�N�o�b�t�@���N���A����Ƃ��̐F���w��
		glClearColor(color.r, color.g, color.b, color.a);

		// �o�b�N�o�b�t�@���N���A
		// ����̓J���[�o�b�t�@�Ɛ[�x�o�b�t�@���w��
		glClear(GL_COLOR_BUFFER_BIT);

		glBindFramebuffer(GL_FRAMEBUFFER, 0);
	}

} // namespace PokarinEngine