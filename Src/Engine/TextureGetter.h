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

		/* static�֐��̂ݎg�p�ł���悤�ɁA�C���X�^���X���ł��Ȃ��悤�ɂ��� */

		TextureGetter() = default;
		~TextureGetter() = default;

	private: // ------------------ �e�N�X�`���쐬�p -------------------

		// �R���X�g���N�^�A�f�X�g���N�^��
		// �Ăׂ�悤�ɂ��邽�߂̕⏕�N���X
		struct TexHelper : public Texture
		{
			TexHelper(const char* fileName) : Texture(fileName) {}
			TexHelper(GLsizei w, GLsizei h) : Texture(w, h) {}
		};

	private: // --------------------- �L���b�V�� ----------------------

		// �e�N�X�`���p�L���b�V��
		// <�e�N�X�`���̃t�@�C����, �e�N�X�`��>
		inline static std::unordered_map<std::string, TexturePtr> textureCache;
	};
}

#endif // !POKARINENGINE_TEXTUREGETTER_H_INCLUDED
