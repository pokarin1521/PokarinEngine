/**
* @file Texture.cpp
*/
#include "Texture.h"
#include "Debug.h"

#include <vector>
#include <fstream>
#include <filesystem>

namespace PokarinEngine
{
	/// <summary>
	/// �e�N�X�`����ǂݍ��ރR���X�g���N�^
	/// (�ÖٓI�L���X�g���֎~)
	/// </summary>
	/// <param name="filename"> �ǂݍ��ރe�N�X�`���t�@�C���̖��O </param>
	Texture::Texture(const char* filename)
	{
		// ---------------------
		// �t�@�C�����J��
		// ---------------------

		// �o�C�i�����[�h���w��
		// ���s�����̕ϊ������s��Ȃ��̂ŁA�ǂݍ��݂�����
		std::ifstream file(filename, std::ios::binary);

		// �t�@�C�����J���ĂȂ���ΏI��
		if (!file)
		{
			LOG_ERROR("%s���J���܂���", filename);
			return;
		}

		// --------------------------
		// �t�@�C����ǂݍ���
		// --------------------------

		// �t�@�C���S�̂���C�ɓǂݍ��ނ��߂ɁA�t�@�C���T�C�Y�擾
		const size_t filesize = std::filesystem::file_size(filename);
		std::vector<uint8_t> buffer(filesize);

		// �t�@�C����ǂݍ��݁Abuffer�Ɋi�[
		file.read(reinterpret_cast<char*>(buffer.data()), buffer.size());

		// �����t�@�C����������Ȃ��̂ŕ���
		file.close();

		// ------------------------------
		//	�w�b�_��������擾
		// ------------------------------

		/* TGA�t�@�C���̐擪18�o�C�g��TGA�w�b�_�Ƃ����f�[�^�̈�
		�w�b�_�ɂ͉摜�T�C�Y�Ȃǂ̏�񂪂���A�摜�f�[�^�͂��̌��ɂ��� */

		// TGA�w�b�_�̃o�C�g��
		const size_t tgaHeaderSize = 18;

		// �摜�f�[�^�̈ʒu
		// (�w�b�_�T�C�Y + �w�b�_�̌�ɑ���ID�̒���)
		const int imageOffset = tgaHeaderSize + buffer[0];

		// �摜�`��
		const uint8_t imageType = buffer[2];

		// 1�s�N�Z���̃o�C�g��
		// 8�Ŋ���؂�Ȃ��ꍇ���l����
		// 1���Ȃ������𑫂����ƂŁA������؂�グ��
		const int pixelBytes = (buffer[16] + 7) / 8;

		// ------------------------------
		// �摜�̕��A���������߂�
		// ------------------------------

		// �摜�̕��ƍ����͂��ꂼ��16�r�b�g�̃f�[�^
		// �l�𕜌����邽�ߏ��8�r�b�g��256�{(����8�V�t�g)���č���
		width = buffer[12] + buffer[13] * 256;
		fboHeight = buffer[14] + buffer[15] * 256;

		// ----------------------------------
		// �摜�����k�`���Ȃ�W�J����
		// ----------------------------------

		/* ----- �摜�`�� -----
		 0(0b0000) : �摜�Ȃ�

		 1(0b0001) : �C���f�b�N�X(�����k)
		 2(0b0010) : �g�D���[�J���[(�����k)
		 3(0b0011) : ����(�����k)

		 9(0b1001) : �C���f�b�N�X(RLE)
		10(0b1010) : �g�D���[�J���[(RLE)
		11(0b1011) : ����(RLE) */

		// ��3�r�b�g��1�̏ꍇ�ARLE(���k�`��)
		if (imageType & 0b1000)
		{
			// ------------ �W�J�O�̏��� --------------

			// �W�J��̃o�C�g��
			const int imageBytes = width * fboHeight * pixelBytes;

			// �W�J�p�̃o�b�t�@
			// �摜�f�[�^���O�̕���(�w�b�_ + ID)���R�s�[����̂�
			// ���̕��T�C�Y���v���X���Ă���
			std::vector<uint8_t> tmp(imageOffset + imageBytes);

			// �W�J���tmp��buffer�����ւ���̂ŁA
			// �摜�f�[�^���O�̕���(�w�b�_ + ID)���R�s�[
			std::copy_n(buffer.begin(), imageOffset, tmp.begin());

			// ----------- �摜�f�[�^��W�J ------------

			// ���k�f�[�^�̃A�h���X
			const uint8_t* src = buffer.data() + imageOffset;

			// �f�[�^�W�J��A�h���X
			uint8_t* dest = tmp.data() + imageOffset;

			// �W�J�I���A�h���X
			const uint8_t* const destEnd = dest + imageBytes;

			while (dest != destEnd)
			{
				// �p�P�b�gID (�p�P�b�g�w�b�_�̍ŏ�ʂ�1�r�b�g)
				// 0 : �����k�p�P�b�g
				// 1 : ���k�p�P�b�g
				const int isRLE = *src & 0x80;

				// �f�[�^�� (�p�P�b�g�w�b�_��0�`6�r�b�g)
				//�u�f�[�^�� - 1�v���L�^����Ă���̂�+1���Ă���
				const int count = (*src & 0x7f) + 1;

				// �A�h���X���p�P�b�g�f�[�^�̈ʒu�ɐi�߂�
				++src;

				// ���k�p�P�b�g
				if (isRLE)
				{
					// �p�P�b�g�f�[�^���w��񐔃R�s�[
					for (int a = 0; a < count; ++a)
					{
						memcpy(dest, src, pixelBytes);
						dest += pixelBytes;
					}

					src += pixelBytes;
				}
				// �����k�p�P�b�g
				else
				{
					// �p�P�b�g�f�[�^�̃o�C�g��
					const int dataBytes = pixelBytes * count;

					// �p�P�b�g�f�[�^�S�̂��R�s�[
					memcpy(dest, src, dataBytes);

					dest += dataBytes;
					src += dataBytes;
				}

			} // while dest

			// buffer��tmp�̓��e������
			buffer.swap(tmp);
		}

		// --------------------------------------
		// �f�[�^�̊i�[�����𒲂ׁA�Ή�����
		// --------------------------------------

		/* TGA�t�@�C���͉摜�f�[�^�̊i�[������I�Ԃ��Ƃ��ł���
		TGA�̃f�t�H���g�ł͉摜�́u�������v�Ɋi�[�����
		�������A�ϊ��Ɏg�����c�[���ɂ���āu�ォ�牺�v�Ɋi�[�����

		OpenGL�̉摜�f�[�^�́u�������v�Ɋi�[�����̂ŁA
		�i�[�����ɂ���ĉ摜�����]���Ă��܂�

		�����h�����߁ATGA�t�@�C����17�o�C�g�ڂɂ���i�[������ǂݎ��

		17�o�C�g�ڂ̓���́A
		bit:0-3 �A���t�@�v�f�̃r�b�g��,
		bit:4	���E�̊i�[����(0 = ������E, 1 = �E���獶),
		bit:5	�㉺�̊i�[����(0 = �������, 1 = �ォ�牺),
		bit:6-7 (���g�p)

		�㉺�̊i�[�������m�肽���̂ŁA5�r�b�g�ڂ𒲂ׂ� */

		// 5�r�b�g�ڂ𒲂ׂ�
		const bool topToBottom = buffer[17] & 0b0010'0000;

		// �i�[�������u�ォ�牺�v�̏ꍇ�A
		// �f�[�^���㉺���]
		if (topToBottom)
		{
			// �摜��1�s�N�Z���̃r�b�g��
			const int pixelDepth = buffer[16];

			// ��s�̃o�C�g��
			const int lineByteSize = width * pixelDepth / 8;

			// �擪18�o�C�g����΂��āA�摜�̈�ԏ�̍s�����߂�
			uint8_t* top = buffer.data() + tgaHeaderSize; // ��̍s�̈ʒu		

			// �Ō�̍s�ȊO�̉摜�ʐ� + ��ԏ�̍s�̈ʒu�ŁA
			// ��ԉ��̍s�̈ʒu�����߂�
			uint8_t* bottom = top + lineByteSize * (fboHeight - 1); // ���̍s�̈ʒu

			// ��s���ۊǂł���悤�ɃT�C�Y�m��
			std::vector<uint8_t> tmp(lineByteSize); // �㉺����ւ��p�o�b�t�@

			// �s�P�ʂŏ㉺���]
			// �㉺�̍s�̈ʒu���t�]����܂ŌJ��Ԃ�
			while (top < bottom)
			{
				// �u��̍s�̃R�s�[�v�����
				std::copy_n(top, lineByteSize, tmp.data());

				// ���̍s���A��̍s�ɏ㏑��
				std::copy_n(bottom, lineByteSize, top);

				// �u��̍s�̃R�s�[�v�����̍s�ɏ㏑��
				std::copy_n(tmp.data(), lineByteSize, bottom);

				// ��̍s�̈ʒu��1�s���Ɉړ�
				top += lineByteSize;

				// ���̍s�̈ʒu��1�s��Ɉړ�
				bottom -= lineByteSize;
			}

		} // if topToBottom

		// ----------------------------------------------------
		// 1�s�N�Z���̃r�b�g���ɑΉ�����`���̃Z�b�g������
		// ----------------------------------------------------

		// �`��
		struct Format
		{
			int pixelDepth;		// TGA��1�s�N�Z���̃r�b�g��

			GLenum imageFormat;	// TGA�̃f�[�^�`��

			GLenum imageType;	// TGA�̃f�[�^�^

			GLenum gpuFormat;	// GPU���̃f�[�^�`��
		};

		// �`�����X�g
		constexpr Format formatList[] = {
			{ 32, GL_BGRA, GL_UNSIGNED_BYTE,			  GL_RGBA8 },
			{ 24, GL_BGR,  GL_UNSIGNED_BYTE,			  GL_RGB8 },
			{ 16, GL_BGRA, GL_UNSIGNED_SHORT_1_5_5_5_REV, GL_RGB5_A1 },
			{ 15, GL_BGR,  GL_UNSIGNED_SHORT_1_5_5_5_REV, GL_RGB5 },
			{  8, GL_RED,  GL_UNSIGNED_BYTE,			  GL_R8 },
		};

		// ���Ή��̃r�b�g����TGA�t�@�C����ǂݍ��񂾂Ƃ���
		// �z��̍Ō�̗v�f������悤�ɁAend()-1����

		// �s�N�Z���̃r�b�g���ɉ������`��
		const Format* format = std::find_if(
			formatList, std::end(formatList) - 1,
			[pixelDepth = buffer[16]](const Format& e) {
				return e.pixelDepth == pixelDepth; });

		// ------------------------------------------------
		// �摜�ɍ��킹�āA�A���C�������g��ύX����
		// ------------------------------------------------

		/* OpenGL���摜�f�[�^��ǂݎ��Ƃ��́A
		�u�f�[�^��4�o�C�g�P�ʂœǂݎ��v�Ƃ����d�g�݂ɂȂ��Ă���
		���̓ǂݎ��P�ʂ̂��Ƃ��u�A���C�������g(Alignment, ����)�v�Ƃ���

		TGA�t�@�C���̏ꍇ�A
		�u�����̃s�N�Z�����L�^������A���̗�̃s�N�Z�����L�^����v�ƂȂ��Ă���

		32�r�b�g�`���̉摜�f�[�^�Ȃ�A
		�����̃f�[�^���͏��OpenGL�̃A���C�������g�Ŋ���؂��
		�������A8�r�b�g�`����24�r�b�g�`���̏ꍇ�A
		�����ɂ���Ă̓A���C�������g�Ŋ���؂ꂸ�G���[����������

		�Ȃ̂ŁA�摜�ɍ��킹�ăA���C�������g��ύX����
		(�C���^�[�l�b�g����摜���E�����ƂɂȂ�̂ŁA�I�ʂ͂���) */

		// �A���C�������g�L�^�p
		// (���ɖ߂��̂Ɏg��)
		GLint alignment;

		// ���݂̃A���C�������g���L�^
		glGetIntegerv(GL_UNPACK_ALIGNMENT, &alignment);

		// �A���C�������g�z��
		// 1�s�̃o�C�g����4�Ŋ���؂�� : 4
		// 2�Ŋ���؂��(4�Ŋ�����2�]��) : 2
		// ����ȊO	: 1
		constexpr int alignmentList[] = { 4, 1, 2, 1 };

		// 1�s�̃o�C�g��
		const int lineByteSize = width * pixelBytes;

		// �摜�ɍ��킹���A���C�������g
		const int imageAlignment = alignmentList[lineByteSize % 4];

		// �L�^����OpenGL�̃A���C�������g��
		// �摜�ɍ��킹���A���C�������g���Ⴄ�ꍇ
		if (alignment != imageAlignment)
		{
			// OpenGL�̃A���C�������g��ύX
			glPixelStorei(GL_UNPACK_ALIGNMENT, imageAlignment);
		}

		// -----------------------
		// �e�N�X�`�����쐬
		// -----------------------

		//	�e�N�X�`���̊Ǘ��ԍ�
		GLuint object = 0;

		// �摜�f�[�^���Ǘ�����e�N�X�`���I�u�W�F�N�g���쐬
		glCreateTextures(GL_TEXTURE_2D, 1, &object);

		/* �摜�f�[�^�̃R�s�[��ł���GPU�������̈���m��

		void glTextureStorage2D(�Ǘ��ԍ�, �쐬���郌�x����,
		�s�N�Z���`��, ��, ����)

		------- ���x���� -------
		�~�b�v�}�b�v�Ɋ֘A��������A
		����͎g��Ȃ��̂ōŏ��l��1���w��

		------ �~�b�v�}�b�v ------
		���_����I�u�W�F�N�g�܂ł̋����ɉ����āA
		�傫���̈Ⴄ�e�N�X�`�����g��������@�\ */

		glTextureStorage2D(object, 1, format->gpuFormat, width, fboHeight);

		/* �摜�f�[�^��GPU�������ɃR�s�[

		void glTextureSubImage2D(�e�N�X�`���̊Ǘ��ԍ�, �R�s�[�惌�C���[�ԍ�,
		�R�s�[���X���W, �R�s�[���Y���W,
		�R�s�[����摜�̕�, �R�s�[����摜�̍���,
		�s�N�Z���Ɋ܂܂��v�f�Ə���, �v�f�̌^, �摜�f�[�^�̃A�h���X)

		�摜�f�[�^�̓e�N�X�`����
		(�R�s�[���X���W, �R�s�[���Y���W)�̈ʒu�ɃR�s�[�����
		�R�s�[��̍��W�w��́A
		�����̉摜�f�[�^���ЂƂ̃e�N�X�`���ɂ܂Ƃ߂����ꍇ�Ɏg�� */

		glTextureSubImage2D(object, 0, 0, 0, width, fboHeight,
			format->imageFormat, format->imageType,
			buffer.data() + tgaHeaderSize);

		id = object; // �e�N�X�`���I�u�W�F�N�g�ԍ�
		name = filename; // �t�@�C����

		// --------------------------------
		// �A���C�������g�����ɖ߂�
		// --------------------------------

		/* �A���C�������g��ύX����ƁA
		GPU�������ւ̃R�s�[���x���ቺ����̂ŁA
		�摜�f�[�^�̃R�s�[���I������猳�ɖ߂� */

		// �L�^����OpenGL�̃A���C�������g��
		// �摜�ɍ��킹���A���C�������g���Ⴄ�ꍇ
		if (alignment != imageAlignment)
		{
			// �A���C�������g���ύX����Ă���̂�
			// ���ɖ߂�
			glPixelStorei(GL_UNPACK_ALIGNMENT, alignment);
		}

		// ---------------------------
		// �����摜�ɑΉ����� 
		// ---------------------------

		/* �f�[�^�`�����uGL_RED(�ԗv�f����)�v�Ŏw�肵�Ă���̂�
		���̂܂܂��Ɛԍ��ŕ\������Ă��܂�

		�X�E�B�Y���Ƃ����@�\���g��
		(Swizzle,�u�����܂���v�Ƃ����Ӗ�)
		���̋@�\���g���ƁA�V�F�[�_�Ńe�N�X�`����ǂݎ�鎞��
		�f�[�^�̏��������ւ��邱�Ƃ��ł���

		�X�E�B�Y����ݒ肷��ɂ́AglTextureParameteri�֐����g��

		void glTextureParameteri(
		�e�N�X�`���̊Ǘ��ԍ�, �ݒ肷��p�����[�^��, �ݒ肷��l) */

		// �O���[�g�X�P�[���e�N�X�`���̏ꍇ�A
		// �Ԑ�����΂ƐɃR�s�[����ɐݒ肷��
		if (format->imageFormat == GL_RED)
		{
			glTextureParameteri(object, GL_TEXTURE_SWIZZLE_R, GL_RED);
			glTextureParameteri(object, GL_TEXTURE_SWIZZLE_G, GL_RED);
			glTextureParameteri(object, GL_TEXTURE_SWIZZLE_B, GL_RED);
		}
	}

	/// <summary>
	/// �e�N�X�`�����쐬����R���X�g���N�^
	/// (�ÖٓI�L���X�g���֎~)
	/// </summary>
	/// <param name="width"> �� </param>
	/// <param name="height"> ���� </param>
	Texture::Texture(GLsizei w, GLsizei h)
	{
		// ------------------------
		// ���ƍ�����ݒ�
		// ------------------------

		width = w;
		fboHeight = h;

		// -------------------------
		// �e�N�X�`���쐬
		// -------------------------

		glCreateTextures(GL_TEXTURE_2D, 1, &id);

		// ---------------------------------
		// �e�N�X�`���C���[�W��ݒ�
		// ---------------------------------

		// �ݒ�ł���悤�Ƀo�C���h
		glBindTexture(GL_TEXTURE_2D, id);

		// �ݒ�
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA,
			width, fboHeight, 0, GL_RGBA, GL_UNSIGNED_BYTE, 0);

		// �둀��̂Ȃ��悤�Ƀo�C���h����
		glBindTexture(GL_TEXTURE_2D, 0);

		// ----------------------------
		// ���b�s���O���@��ݒ�
		// ----------------------------

		/* �e�N�X�`���͈̔͊O���Q�Ƃ�������
		�����ɉ摜���~���l�߂��Ă���悤�Ɉ���(���b�s���O)

		����́A�e�N�X�`���̒[�̐F��~���l�߂�悤�ɐݒ肷�� */

		// ������
		glTextureParameteri(
			id, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);

		// �c����
		glTextureParameteri(
			id, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);

		// ----------------------------------
		// �t�B���^�����O���@��ݒ�
		// ----------------------------------

		/* �e�N�X�`�����g��E�k����������
		�摜������Ȃ��悤���Y��ɂ���(�t�B���^�����O)

		����́A���͂̐F���畽�ϓI�ȐF�����߂Ďg���悤�ɐݒ肷�� */

		// �g�厞
		glTextureParameteri(
			id, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

		// �k����
		glTextureParameteri(
			id, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	}

	/// <summary>
	/// �e�N�X�`�����폜����f�X�g���N�^
	/// </summary>
	Texture::~Texture()
	{
		// �e�N�X�`���I�u�W�F�N�g���폜
		glDeleteTextures(1, &id);
	}

} // namespace PokarinEngine