/**
* @file FramebufferObject.h
*/
#ifndef FRAMEBUFFEROBJECT_H_INCLUDED
#define FRAMEBUFFEROBJECT_H_INCLUDED

#include "glad/glad.h"
#include "Window.h"
#include "Debug.h"
#include "Texture.h"

#include "Color.h"

#include <memory>

namespace PokarinEngine
{
	// -------------------
	// �O���錾
	// -------------------

	class FramebufferObject;
	class RenderbufferObject;
	class Engine;

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
		/// <param name="engine"> �G���W���N���X�̎Q�� </param>
		/// <param name="_windowID"> �E�B���h�E�ԍ� </param>
		/// <param name="width"> FBO�̕� </param>
		/// <param name="height"> FBO�̍��� </param>
		static FramebufferObjectPtr Create(
			Engine& engine, WindowID _windowID, GLsizei width, GLsizei height)
		{
			return std::make_shared<FramebufferObject>(
				engine, _windowID, width, height);
		}

	public: // -------- �R���X�g���N�^�E�f�X�g���N�^ --------

		/// <summary>
		/// FBO���쐬����R���X�g���N�^
		/// </summary>
		/// <param name="engine"> �G���W���N���X�̎Q�� </param>
		/// <param name="_windowID"> �E�B���h�E�ԍ� </param>
		/// <param name="width"> FBO�̕� </param>
		/// <param name="height"> FBO�̍��� </param>
		FramebufferObject(
			Engine& engine, WindowID _windowID, GLsizei width, GLsizei height);

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
		/// <param name="red"> �� </param>
		/// <param name="green"> �� </param>
		/// <param name="blue"> �� </param>
		/// <param name="alpha"> �����x </param>
		void ClearColor(const Color& color);

	public: // ----------------- �T�C�Y --------------------

		/// <summary>
		/// FBO�̑傫����ݒ肷��
		/// </summary>
		/// <param name="width"> �� </param>
		/// <param name="height"> ���� </param>
		void SetSize(GLsizei width, GLsizei height)
		{
			fboWidth = width;
			fboHeight = height;
		}

	public: // -------------- �Ǘ��ԍ��̎擾 ---------------

		// �Ǘ��ԍ��̎擾(GLuint�^�ɕϊ�)
		operator GLuint() const { return id; }

	public: // ------------- �e�N�X�`���̎擾 --------------

		/// <summary>
		/// �`���̃e�N�X�`�����擾����
		/// </summary>
		/// <returns> �J���[�o�b�t�@�p�e�N�X�`���̊Ǘ��ԍ� </returns>
		GLuint GetTexture() { return *texture; }

	private: // ------------------- ��� --------------------

		// FBO�̊Ǘ��ԍ�
		GLuint id = 0;

		// �J���[�o�b�t�@�p�e�N�X�`��
		TexturePtr texture;

		// �[�x�o�b�t�@�pRBO
		RenderbufferObjectPtr rbo;

		// FBO�̑傫��
		GLsizei fboWidth = 0, fboHeight = 0;

		// �E�B���h�E���ʔԍ�
		WindowID windowID;
	};

} // namespace PokarinEngine

#endif // !FRAMEBUFFEROBJECT_H_INCLUDED
