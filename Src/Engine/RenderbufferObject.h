/**
* @file RenderbufferObject.h
*/
#ifndef RENDERBUFFEROBJECT_H_INCLUDED
#define RENDERBUFFEROBJECT_H_INCLUDED

#include "glad/glad.h"

#include <memory>

namespace PokarinEngine
{
	// ----------------------
	// �O���錾
	// ----------------------

	class RenderbufferObject;

	// ------------------------------
	// �^�̕ʖ����`
	// ------------------------------

	using RenderbufferObjectPtr = std::shared_ptr<RenderbufferObject>;

	/// <summary>
	/// RBO(�����_�����O���ꂽ���ʂ�ێ�����o�b�t�@�I�u�W�F�N�g)
	/// </summary>
	class RenderbufferObject
	{
	public: // ------------------- RBO���쐬 ---------------------

		/// <summary>
		/// RBO���쐬����
		/// </summary>
		/// <param name="width"> �� </param>
		/// <param name="height"> ���� </param>
		/// <param name="internalformat"> �ێ�������̃o�b�t�@ </param>
		/// <returns> �쐬����RBO </returns>
		static RenderbufferObjectPtr Create(
			GLsizei width, GLsizei height, GLenum internalformat)
		{
			return std::make_shared<RenderbufferObject>(
				width, height, internalformat);
		}

	public: // ---------- �R���X�g���N�^�E�f�X�g���N�^ -----------

		/// <summary>
		/// RBO���쐬����
		/// </summary>
		/// <param name="width"> �� </param>
		/// <param name="height"> ���� </param>
		/// <param name="internalformat"> �ێ�������̃o�b�t�@ </param>
		RenderbufferObject(
			GLsizei width, GLsizei height, GLenum internalformat);

		/// <summary>
		/// RBO���폜����f�X�g���N�^
		/// </summary>
		~RenderbufferObject();

	public: // ----------------- �֎~���� -------------------

		// �R�s�[�R���X�g���N�^�̋֎~
		RenderbufferObject(const RenderbufferObject&) = delete;

		// ����̋֎~
		RenderbufferObject operator=(const RenderbufferObject&) = delete;

	public: // ------------------- �L���X�g ----------------------

		// GLuint�^�ɕϊ�(�Ǘ��ԍ���Ԃ�)
		operator GLuint() const { return id; }

	private: // --------------------- ��� -----------------------

		// RBO�̊Ǘ��ԍ�
		GLuint id = 0;
	};

} // namespace PokarinEngine

#endif // !RENDERBUFFEROBJECT_H_INCLUDED
