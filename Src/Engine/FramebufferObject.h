/**
* @file FramebufferObject.h
*/
#ifndef POKARINENGINE_FRAMEBUFFEROBJECT_H_INCLUDED
#define POKARINENGINE_FRAMEBUFFEROBJECT_H_INCLUDED

#include "glad/glad.h"

#include "Window.h"
#include "Debug.h"
#include "TextureGetter.h"

#include "Color.h"

#include <memory>

namespace PokarinEngine
{
	// -------------------
	// �O���錾
	// -------------------

	class FramebufferObject;
	class RenderbufferObject;

	// --------------------------
	// �^�̕ʖ����`
	// --------------------------

	using FramebufferObjectPtr = std::shared_ptr<FramebufferObject>;
	using RenderbufferObjectPtr = std::shared_ptr<RenderbufferObject>;

	/// <summary>
	/// FBO(�J���[�o�b�t�@�E�[�x�o�b�t�@�Ȃǂ𓝊�����o�b�t�@�I�u�W�F�N�g)
	/// </summary>
	class FramebufferObject
	{
	public: // ------------------ FBO�쐬 -------------------

		/// <summary>
		/// FBO���쐬����
		/// </summary>
		static FramebufferObjectPtr Create()
		{
			return std::make_shared<FramebufferObject>();
		}

	public: // -------- �R���X�g���N�^�E�f�X�g���N�^ --------

		/// <summary>
		/// FBO���쐬����R���X�g���N�^
		/// </summary>
		FramebufferObject();

		/// <summary>
		/// FBO���폜����f�X�g���N�^
		/// </summary>
		~FramebufferObject();

	public: // ----------------- �֎~���� -------------------

		// �R�s�[�R���X�g���N�^�̋֎~
		FramebufferObject(const FramebufferObject&) = delete;

		// ����̋֎~
		FramebufferObject operator=(const FramebufferObject&) = delete;

	public: // --------------- FBO�̃o�C���h ----------------

		/// <summary>
		/// FBO���o�C���h����
		/// </summary>
		void Bind() const;

		/// <summary>
		/// FBO�̃o�C���h����������
		/// </summary>
		void UnBind() const;

	public: // -------------- �J���[�o�b�t�@ ---------------

		/// <summary>
		/// �J���[�o�b�t�@�p�e�N�X�`�����N���A
		/// </summary>
		/// <param name="[in] Color"> �N���A�F </param>
		void ClearColor(const Color& color);

	public: // ----------------- �T�C�Y --------------------

		/// <summary>
		/// FBO�̑傫����ݒ肷��
		/// </summary>
		/// <param name="[in] fboWidth"> �� </param>
		/// <param name="[in] fboHeight"> ���� </param>
		void SetSize(GLsizei fboWidth, GLsizei fboHeight)
		{
			width = fboWidth;
			height = fboHeight;
		}

	public: // -------------- ���ʔԍ��̎擾 ---------------

		// ���ʔԍ��̎擾(GLuint�^�ɕϊ�)
		operator GLuint() const { return id; }

	public: // ------------- �e�N�X�`���̎擾 --------------

		/// <summary>
		/// �`���̃e�N�X�`���̎��ʔԍ����擾����
		/// </summary>
		/// <returns> �J���[�o�b�t�@�p�e�N�X�`���̎��ʔԍ� </returns>
		GLuint GetTextureID() const { return *texture; }

	private: // ------------------- ��� --------------------

		// FBO�̎��ʔԍ�
		GLuint id = 0;

		// �J���[�o�b�t�@�p�e�N�X�`��
		TexturePtr texture;

		// �[�x�o�b�t�@�pRBO
		RenderbufferObjectPtr rbo;

		// FBO�̑傫��
		// 16 : 9 �Őݒ�
		GLsizei width = 1280, height = 720;
	};

} // namespace PokarinEngine

#endif // !POKARINENGINE_FRAMEBUFFEROBJECT_H_INCLUDED
