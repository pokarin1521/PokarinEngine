/**
* @file RenderbufferObject.h
*/
#ifndef POKARINENGINE_RENDERBUFFEROBJECT_H_INCLUDED
#define POKARINENGINE_RENDERBUFFEROBJECT_H_INCLUDED

#include "glad/glad.h"

#include <memory>

namespace PokarinEngine
{
	/// <summary>
	/// RBO(�����_�����O���ꂽ���ʂ�ێ�����o�b�t�@�I�u�W�F�N�g)
	/// </summary>
	class RenderbufferObject
	{
	public: // ------------------- RBO���쐬 ---------------------

		/// <summary>
		/// RBO���쐬����
		/// </summary>
		/// <param name="[in] width"> �� </param>
		/// <param name="[in] height"> ���� </param>
		/// <param name="[in] internalformat"> �ێ�������̃o�b�t�@ </param>
		/// <returns> �쐬����RBO </returns>
		static std::shared_ptr<RenderbufferObject> Create(
			GLsizei width, GLsizei height, GLenum internalformat)
		{
			return std::make_shared<RenderbufferObject>(
				width, height, internalformat);
		}

	public: // ---------- �R���X�g���N�^�E�f�X�g���N�^ -----------

		/// <summary>
		/// RBO���쐬����
		/// </summary>
		/// <param name="[in] width"> �� </param>
		/// <param name="[in] height"> ���� </param>
		/// <param name="[in] internalformat"> �ێ�������̃o�b�t�@ </param>
		RenderbufferObject(
			GLsizei width, GLsizei height, GLenum internalformat);

		/// <summary>
		/// RBO���폜����f�X�g���N�^
		/// </summary>
		~RenderbufferObject();

	public: // -------------------- �֎~���� ---------------------

		/* �폜�p�f�X�g���N�^�𕡐���Ă΂�Ȃ��悤�ɋ֎~���� */

		// �R�s�[�R���X�g���N�^�̋֎~
		RenderbufferObject(const RenderbufferObject&) = delete;

		// ����̋֎~
		RenderbufferObject operator=(const RenderbufferObject&) = delete;

	public: // ---------------------- �ϊ� -----------------------

		// GLuint�^�ɕϊ�(���ʔԍ���Ԃ�)
		operator GLuint() const { return id; }

	private: // --------------------- ��� -----------------------

		// RBO�̎��ʔԍ�
		GLuint id = 0;
	};

} // namespace PokarinEngine

#endif // !POKARINENGINE_RENDERBUFFEROBJECT_H_INCLUDED
