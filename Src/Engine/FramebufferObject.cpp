/**
* FramebufferObject.cpp
*/
#include "FramebufferObject.h"

#include "Engine.h"
#include "RenderbufferObject.h"

namespace PokarinEngine
{
	/// <summary>
	/// FBO���쐬����R���X�g���N�^
	/// </summary>
	FramebufferObject::FramebufferObject()
	{
		// -------------------------------------------
		// �J���[�o�b�t�@�p�e�N�X�`�����쐬
		// -------------------------------------------

		texture = TextureGetter::Get(width, height);

		// -------------------------------
		// �[�x�o�b�t�@�pRBO���쐬
		// -------------------------------

		// �ǂ�GPU�ł�����Ȃ�̐��\�ɂȂ邱�Ƃ����҂���
		// �[�x�o�b�t�@��GL_DEPTH_COMPONENT32�ɂ���
		rbo = RenderbufferObject::Create(
			width, height, GL_DEPTH_COMPONENT32);

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

			// ���ʔԍ���������
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
		glViewport(0, 0, width, height);

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

		// OpenGL���C���E�B���h�E�̑傫�����擾
		GLsizei windowWidth = 0, windowHeight = 0;
		Window::GetWindowSize(WindowID::Main, windowWidth, windowHeight);

		// �r���[�|�[�g��OpenGL���C���E�B���h�E�̑傫���ɖ߂�
		glViewport(0, 0, windowWidth, windowHeight);
	}

	/// <summary>
	/// �o�b�N�o�b�t�@���N���A����
	/// </summary>
	/// <param name="[in] color"> �N���A�F </param>
	void FramebufferObject::ClearColor(const Color& color)
	{
		// �o�C���h
		glBindFramebuffer(GL_FRAMEBUFFER, id);

		// �N���A�F��ݒ肷��
		glClearColor(color.r, color.g, color.b, color.a);

		// �o�b�N�o�b�t�@���N���A
		// ����̓J���[�o�b�t�@�Ɛ[�x�o�b�t�@���w��
		glClear(GL_COLOR_BUFFER_BIT);

		// �o�C���h����
		glBindFramebuffer(GL_FRAMEBUFFER, 0);
	}

} // namespace PokarinEngine