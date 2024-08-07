/**
* @file TextureGetter.h
*/
#ifndef POKARINENGINE_TEXTUREGETTER_H_INCLUDED
#define POKARINENGINE_TEXTUREGETTER_H_INCLUDED

#include "Texture.h"

#include <unordered_map>

namespace PokarinEngine
{
	/// <summary>
	/// �e�N�X�`���擾�p�N���X
	/// </summary>
	class TextureGetter
	{
	public: // ----------------------- �֎~���� -----------------------

		/* static�֐��ŃA�N�Z�X���Ăق����̂ŁA�֎~���� */

		// �R�s�[�R���X�g���N�^�̋֎~
		TextureGetter(const TextureGetter&) = delete;

		// ����̋֎~
		TextureGetter& operator=(const TextureGetter&) = delete;

	public: // ------------------- �e�N�X�`���̎擾 -------------------

		/// <summary>
		/// �e�N�X�`�����擾����
		/// </summary>
		/// <param name="[in] fileName"> �t�@�C���� </param>
		/// <returns> �t�@�C��������v����e�N�X�`�� </returns>
		static TexturePtr Get(const std::string& fileName);

		/// <summary>
		/// �e�N�X�`�����擾����
		/// </summary>
		/// <param name="[in] width"> �� </param>
		/// <param name="[in] height"> ���� </param>
		/// <returns> �w�肵���傫���̃e�N�X�`�� </returns>
		static TexturePtr Get(GLsizei width, GLsizei height);

	private: // ------------ �R���X�g���N�^�E�f�X�g���N�^ -------------

		/* static�֐��ŃA�N�Z�X���Ăق����̂ŁA�C���X�^���X���ł��Ȃ��悤�ɂ��� */

		TextureGetter() = default;
		~TextureGetter() = default;

	private: // ----------------- �e�N�X�`���ǂݍ��� ------------------

		/// <summary>
		/// �e�N�X�`����ǂݍ���
		/// </summary>
		/// <param name="[in] fileName"> �e�N�X�`���̃t�@�C���� </param>
		/// <returns> �ǂݍ��񂾃e�N�X�`�� </returns>
		static TexturePtr LoadTexture(const std::string& fileName);

	private: // --------------------- �L���b�V�� ----------------------

		// �e�N�X�`���p�L���b�V��
		// <�e�N�X�`���̃t�@�C����, �e�N�X�`��>
		inline static std::unordered_map<std::string, TexturePtr> textureCache;
	};
}

#endif // !POKARINENGINE_TEXTUREGETTER_H_INCLUDED
