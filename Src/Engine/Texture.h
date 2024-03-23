/**
* @file Texture.h
*/
#ifndef TEXTURE_H_INCLUDED
#define TEXTURE_H_INCLUDED

#include "glad/glad.h"

#include <string>
#include <memory>

namespace PokarinEngine
{
	// --------------------- 
	// �O���錾
	// --------------------- 

	class Texture;

	// ----------------------
	// �^�̕ʖ����`
	// ----------------------

	// �e�N�X�`���|�C���^�^
	using TexturePtr = std::shared_ptr<Texture>;

	/**
	* �e�N�X�`���Ǘ��N���X
	*/
	class Texture
	{
		// Engine�N���X�ɏ������J
		friend class Engine;

	public: // ----------------- �֎~���� -------------------

		// �R�s�[�R���X�g���N�^�̋֎~
		Texture(const Texture&) = delete;

		// �R�s�[����̋֎~
		Texture& operator=(const Texture&) = delete;

	public: // ----------------- �L���X�g -------------------

		// GLuint�^�ɕϊ�(�Ǘ��ԍ���Ԃ�)
		operator GLuint() const { return id; }

	public: // ---------- �e�N�X�`���̏����擾 ------------

		/// <summary>
		/// �e�N�X�`���̕����擾
		/// </summary>
		int GetWidth() const { return width; }

		/// <summary>
		/// �e�N�X�`���̍������擾
		/// </summary>
		int GetHeight() const { return fboHeight; }

		/// <summary>
		/// �摜�̃A�X�y�N�g����擾
		/// </summary>
		float GetAspectRatio() const
		{
			return static_cast<float>(width) / static_cast<float>(fboHeight);
		}

		/// <summary>
		/// �e�N�X�`���̖��O���擾
		/// </summary>
		const std::string& GetName() const { return name; }

	private: // ---------- �R���X�g���N�^�E�f�X�g���N�^ -----------

		/* �L���b�V����L���ɂ��邽�߂�
		�R���X�g���N�^�ƃf�X�g���N�^���v���C�x�[�g�����o�ɂ���
		����ɐ�����폜���ł��Ȃ��悤�ɂ��� */

		/// <summary>
		/// �e�N�X�`����ǂݍ��ރR���X�g���N�^
		/// (�ÖٓI�L���X�g���֎~)
		/// </summary>
		/// <param name="filename"> �ǂݍ��ރe�N�X�`���t�@�C���̖��O </param>
		explicit Texture(const char* filename);

		/// <summary>
		/// �e�N�X�`�����쐬����R���X�g���N�^
		/// (�ÖٓI�L���X�g���֎~)
		/// </summary>
		/// <param name="width"> �� </param>
		/// <param name="height"> ���� </param>
		explicit Texture(GLsizei w, GLsizei h);

		/// <summary>
		/// �e�N�X�`�����폜����f�X�g���N�^
		/// </summary>
		~Texture();

	private: // -------------- �e�N�X�`���̏�� ---------------

		std::string name;	// �e�N�X�`����(��Ƀf�o�b�O�p)
		GLuint id = 0;		// �e�N�X�`���I�u�W�F�N�g�Ǘ��ԍ�
		int width = 0;		// �e�N�X�`���̕�
		int fboHeight = 0;		// �e�N�X�`���̍���
	};

} // namespace PokarinEngine

#endif // !TEXTURE_H_INCLUDED
