/**
* @file Texture.h
*/
#ifndef POKARINENGINE_TEXTURE_H_INCLUDED
#define POKARINENGINE_TEXTURE_H_INCLUDED

#include "glad/glad.h"
#include "ImGui/imgui.h"

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

	/// <summary>
	/// �e�N�X�`���Ǘ��N���X
	/// </summary>
	class Texture
	{
		// �e�N�X�`���擾�p�N���X�ɏ������J
		friend class TextureGetter;

	public: // ----------------- �֎~���� -------------------

		// �R�s�[�R���X�g���N�^�̋֎~
		Texture(const Texture&) = delete;

		// �R�s�[����̋֎~
		Texture& operator=(const Texture&) = delete;

	public: // ------------------- �ϊ� ---------------------

		// GLuint�^�ɕϊ�(���ʔԍ���Ԃ�)
		operator GLuint() const { return id; }		
		
		// ImTextureID�^�ɕϊ�(���ʔԍ���Ԃ�)
		operator ImTextureID() const { return (void*)(std::intptr_t)id; }

	public: // ---------- �e�N�X�`���̏����擾 ------------

		/// <summary>
		/// �e�N�X�`���̕����擾
		/// </summary>
		int GetWidth() const
		{
			return width;
		}

		/// <summary>
		/// �e�N�X�`���̍������擾
		/// </summary>
		int GetHeight() const
		{
			return height;
		}
		
		/// <summary>
		/// �摜�̃A�X�y�N�g����擾
		/// </summary>
		float GetAspectRatio() const
		{
			return static_cast<float>(width) / static_cast<float>(height);
		}

		/// <summary>
		/// �e�N�X�`���̖��O���擾
		/// </summary>
		const std::string& GetName() const
		{
			return name;
		}

	private: // ---------- �R���X�g���N�^�E�f�X�g���N�^ -----------

		/* �L���b�V����L���ɂ��邽�߂�
		�R���X�g���N�^�ƃf�X�g���N�^���v���C�x�[�g�����o�ɂ���
		����ɐ�����폜���ł��Ȃ��悤�ɂ��� */

		/// <summary>
		/// �e�N�X�`����ǂݍ��ރR���X�g���N�^
		/// (�ÖٓI�L���X�g���֎~)
		/// </summary>
		/// <param name="[in] fileName"> �ǂݍ��ރe�N�X�`���t�@�C���̖��O </param>
		explicit Texture(const char* fileName);

		/// <summary>
		/// �e�N�X�`�����쐬����R���X�g���N�^
		/// (�ÖٓI�L���X�g���֎~)
		/// </summary>
		/// <param name="[in] width"> �� </param>
		/// <param name="[in] height"> ���� </param>
		explicit Texture(GLsizei w, GLsizei h);

		/// <summary>
		/// �e�N�X�`�����폜����f�X�g���N�^
		/// </summary>
		~Texture();

	private: // -------------------- ��� ---------------------

		// �e�N�X�`����
		std::string name;

		// �e�N�X�`���I�u�W�F�N�g�̎��ʔԍ�
		GLuint id = 0;		
		
		// �e�N�X�`���̕�
		int width = 0;	
		
		// �e�N�X�`���̍���
		int height = 0;	
	};

} // namespace PokarinEngine

#endif // !POKARINENGINE_TEXTURE_H_INCLUDED
