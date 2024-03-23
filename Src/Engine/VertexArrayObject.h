/**
* @file VertexArrayObject.h
*/
#ifndef VERTEXARRAYOBJECT_H_INCLUDED
#define VERTEXARRAYOBJECT_H_INCLUDED

#include "glad/glad.h"

#include <memory>

namespace PokarinEngine
{
	// -----------------
	// �O���錾
	// -----------------

	class VertexArrayObject;

	// -----------------------
	// �^�̕ʖ����`
	// -----------------------

	using VertexArrayObjectPtr = std::shared_ptr<VertexArrayObject>;

	/// <summary>
	/// <para> ���_�z��I�u�W�F�N�g(VAO) </para>
	/// <para> ���_�o�b�t�@�Ɋi�[����Ă��钸�_�f�[�^�ɂ��āA </para>
	/// <para> �e�v�f���ǂ̂悤�ɔz�u����Ă��邩���L�q����I�u�W�F�N�g </para>
	/// </summary>
	class VertexArrayObject
	{
	public: // ---------------- static�֐� ------------------

		/// <summary>
		/// VAO���쐬����
		/// </summary>
		/// <returns> �쐬����VAO�ւ̃|�C���^ </returns>
		static VertexArrayObjectPtr Create()
		{
			return std::make_shared<VertexArrayObject>();
		}

	public: // -------- �R���X�g���N�^�E�f�X�g���N�^ --------

		/// <summary>
		/// VAO���쐬����R���X�g���N�^
		/// </summary>
		VertexArrayObject() { glCreateVertexArrays(1, &id); }

		/// <summary>
		/// VAO���폜����f�X�g���N�^
		/// </summary>
		~VertexArrayObject() { glDeleteVertexArrays(1, &id); }

	public: // ----------------- �֎~���� -------------------

		// �R�s�[�R���X�g���N�^�̋֎~
		VertexArrayObject(const VertexArrayObject&) = delete;

		// ����̋֎~
		VertexArrayObject& operator=(const VertexArrayObject&) = delete;

	public: // ---------------- ���̎擾 ------------------

		// �Ǘ��ԍ����擾(GLuint�^�ɃL���X�g)
		operator GLuint() const { return id; }

		/// <summary>
		/// ���_������ݒ�
		/// </summary>
		/// <param name="index"> �ݒ肷�钸�_�����̔ԍ�(0�`15) </param>
		/// <param name="size"> �v�f�̃f�[�^��(float = 1, vec2 = 2, vec3 = 3) </param>
		/// <param name="stride"> ���̒��_�f�[�^�ɂ��铯��v�f�܂ł̊Ԋu(�o�C�g��) </param>
		/// <param name="offset"> ���_�f�[�^���ɂ�����v�f�̈ʒu(�o�C�g��) </param>
		void SetAttribute(GLuint index, GLuint size, size_t stride, size_t offset)
		{
			/* void glVertexAttribPointer(���_�����z��̃C���f�b�N�X, �f�[�^�̗v�f��,
			�f�[�^�̌^, ���K���̗L��, ���̃f�[�^�܂ł̃o�C�g��, �ŏ��̃f�[�^�̈ʒu)

			�o�C���h�����o�b�t�@�ɓ����Ă�f�[�^�̏���GPU�ɋ�����
			GPU�ɋ������f�[�^�́AVertexPuller�����o��VertexShader�ɓn��
			(VertexShader�̃C���ϐ�)

			GPU��16�v�f�̒��_�����z����������Ƃ��ł��A
			glEnableVertexAttribArray�ŕK�v�ȗv�f��L�������Ďg�� */

			// ���_�����̗L����
			glEnableVertexAttribArray(index);

			// ���_������ݒ�
			glVertexAttribPointer(index, size, GL_FLOAT, GL_FALSE,
				static_cast<GLsizei>(stride), reinterpret_cast<void*>(offset));
		}

	private: // ---------------- VAO�̏�� ------------------

		GLuint id = 0; // �I�u�W�F�N�g�Ǘ��ԍ�
	};

} // namespace PokarinEngine

#endif // !VERTEXARRAYOBJECT_H_INCLUDED
