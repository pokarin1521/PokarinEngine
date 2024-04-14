/**
* @file BufferObject.h
*/
#ifndef BUFFEROBJECT_H_INCLUDED
#define BUFFEROBJECT_H_INCLUDED

#include "glad/glad.h"

#include <memory>

namespace PokarinEngine
{
	// -----------------
	// �O���錾
	// -----------------

	class BufferObject;

	// -----------------------
	// �^�̕ʖ����`
	// -----------------------

	using BufferObjectPtr = std::shared_ptr <BufferObject>;

	/// <summary>
	/// �o�b�t�@�I�u�W�F�N�g(BO)
	/// </summary>
	class BufferObject
	{
	public: // ----------- �o�b�t�@�I�u�W�F�N�g�쐬 ------------

		/// <summary>
		/// �o�b�t�@�I�u�W�F�N�g���쐬����
		/// </summary>
		/// <param name="size"> 
		/// �o�b�t�@�I�u�W�F�N�g�̃o�C�g�� 
		/// </param>
		/// <param name="data"> 
		/// �o�b�t�@�ɃR�s�[����f�[�^�̃A�h���X�A
		/// nullptr���w�肷��Ƌ�̃o�b�t�@���쐬����� 
		/// </param>
		/// <param name="flags"> 
		/// �������̎g�p���@�������t���O
		/// </param>
		/// <returns> �쐬�����o�b�t�@�I�u�W�F�N�g�ւ̃|�C���^ </returns>
		static BufferObjectPtr Create(GLsizeiptr size,
			const void* data = nullptr, GLbitfield flags = 0)
		{
			return std::make_shared<BufferObject>(size, data, flags);
		}

	public: // --------- �R���X�g���N�^�E�f�X�g���N�^ ----------

		/// <summary>
		/// �o�b�t�@�I�u�W�F�N�g�쐬�p�R���X�g���N�^
		/// </summary>
		/// <param name="size"> �o�b�t�@�T�C�Y(�o�C�g��) </param>
		/// <param name="data"> �R�s�[����f�[�^�̃A�h���X </param>
		/// <param name="flags"> �������̎g�p���@�������t���O </param>
		BufferObject(GLsizeiptr size, const void* data, GLbitfield flags)
		{
			// �o�b�t�@�T�C�Y��ݒ�
			bufferSize = size;

			// �o�b�t�@�I�u�W�F�N�g�̍쐬
			glCreateBuffers(1, &id);

			// GPU�������̊m��
			glNamedBufferStorage(id, bufferSize, data, flags);
		}

		/// <summary> 
		/// �o�b�t�@�I�u�W�F�N�g���폜����f�X�g���N�^
		/// </summary>
		~BufferObject() { glDeleteBuffers(1, &id); }

	public: // ----------------- �֎~���� -------------------

		// �R�s�[�R���X�g���N�̋֎~
		BufferObject(const BufferObject&) = delete;

		// ����̋֎~
		BufferObject& operator=(const BufferObject&) = delete;

	public: // ---------------- ���̎擾 ------------------

		// �Ǘ��ԍ����擾(GLuint�^�ɃL���X�g)
		operator GLuint() const { return id; }

		/// <summary>
		/// �o�b�t�@�T�C�Y���擾
		/// </summary>
		/// <returns> �o�b�t�@�T�C�Y </returns>
		GLsizeiptr GetSize() const { return bufferSize; }

	private: // -------- �o�b�t�@�I�u�W�F�N�g�̏�� ---------

		GLuint id = 0; // �I�u�W�F�N�g�Ǘ��ԍ�
		GLsizeiptr bufferSize = 0; // �o�b�t�@�T�C�Y(�o�C�g��)
	};

} // namespace PokarinEngine

#endif // !BUFFEROBJECT_H_INCLUDED
