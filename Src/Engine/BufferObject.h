/**
* @file BufferObject.h
*/
#ifndef POKARINENGINE_BUFFEROBJECT_H_INCLUDED
#define POKARINENGINE_BUFFEROBJECT_H_INCLUDED

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
		/// <param name="[in] size"> �o�b�t�@�I�u�W�F�N�g�̃o�C�g��  </param>
		/// <param name="[in] data"> 
		/// <para> �o�b�t�@�ɃR�s�[����f�[�^�̃A�h���X�A </para>
		/// <para> nullptr���w�肷��Ƌ�̃o�b�t�@���쐬����� </para>
		/// </param>
		/// <param name="[in] flags"> �������̎g�p���@�������t���O </param>
		/// <returns> �쐬�����o�b�t�@�I�u�W�F�N�g�ւ̃|�C���^ </returns>
		static BufferObjectPtr Create(GLsizeiptr size,
			const void* data = nullptr, GLbitfield flags = 0)
		{
			return std::make_shared<BufferObject>(size, data, flags);
		}

	public: // --------- �R���X�g���N�^�E�f�X�g���N�^ ----------

		/// <summary>
		/// �o�b�t�@�I�u�W�F�N�g���쐬����R���X�g���N�^
		/// </summary>
		/// <param name="[in] size"> �o�b�t�@�I�u�W�F�N�g�̃o�C�g��  </param>
		/// <param name="[in] data"> 
		/// <para> �o�b�t�@�ɃR�s�[����f�[�^�̃A�h���X�A </para>
		/// <para> nullptr���w�肷��Ƌ�̃o�b�t�@���쐬����� </para>
		/// </param>
		/// <param name="[in] flags"> �������̎g�p���@�������t���O </param>
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

		/* �폜�p�f�X�g���N�^�𕡐���Ă΂�Ȃ��悤�ɋ֎~���� */

		// �R�s�[�R���X�g���N�̋֎~
		BufferObject(const BufferObject&) = delete;

		// ����̋֎~
		BufferObject& operator=(const BufferObject&) = delete;

	public: // ---------------- ���̎擾 ------------------

		// ���ʔԍ����擾(GLuint�^�ɃL���X�g)
		operator GLuint() const { return id; }

		/// <summary>
		/// �o�b�t�@�T�C�Y���擾
		/// </summary>
		/// <returns> �o�b�t�@�T�C�Y </returns>
		GLsizeiptr GetSize() const { return bufferSize; }

	private: // -------- �o�b�t�@�I�u�W�F�N�g�̏�� ---------

		// �I�u�W�F�N�g���ʔԍ�
		GLuint id = 0; 
		
		// �o�b�t�@�T�C�Y(�o�C�g��)
		GLsizeiptr bufferSize = 0;
	};

} // namespace PokarinEngine

#endif // !POKARINENGINE_BUFFEROBJECT_H_INCLUDED
