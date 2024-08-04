/**
* @file TextureGetter.cpp
*/
#include "TextureGetter.h"

#include <unordered_map>

namespace PokarinEngine
{
	/// <summary>
	/// �e�N�X�`�����擾����
	/// </summary>
	/// <param name="fileName"> �t�@�C���� </param>
	/// <returns> �t�@�C��������v����e�N�X�`�� </returns>
	TexturePtr TextureGetter::Get(const std::string& fileName)
	{
		// ----------------------------------------
		// �L���b�V���ɂ����
		// �L���b�V�����ꂽ�e�N�X�`����Ԃ�
		// ----------------------------------------
		
		// �e�N�X�`��������
		// �Ȃ����end������
		auto itr = textureCache.find(fileName);

		// �e�N�X�`�������������ꍇ
		if (itr != textureCache.end())
		{
			// �L���b�V�����ꂽ�e�N�X�`����Ԃ�
			return itr->second;
		}

		// --------------------------------------------
		// �L���b�V���ɂȂ����
		// �e�N�X�`����ǂݍ���ŃL���b�V���ɓo�^
		// --------------------------------------------

		/* make_shared�֐���Engine�N���X�̃����o�֐��ł͂Ȃ�
		�Ȃ̂ŁA�⏕�N���X���쐬����
		�ԐړI�ɃR���X�g���N�^�A�f�X�g���N�^���Ăяo�� */

		// �e�N�X�`��
		// �t�@�C������e�N�X�`����ǂݍ���
		TexturePtr tex = std::make_shared<TexHelper>(fileName);

		// �e�N�X�`�����L���b�V���ɓo�^
		textureCache.emplace(fileName, tex);

		// �e�N�X�`����Ԃ�
		return tex;
	}

	/// <summary>
	/// �e�N�X�`�����擾����
	/// </summary>
	/// <param name="[in] width"> �� </param>
	/// <param name="[in] height"> ���� </param>
	/// <returns> �w�肵���傫���̃e�N�X�`�� </returns>
	TexturePtr TextureGetter::Get(GLsizei width, GLsizei height)
	{
		/* make_shared�֐���Engine�N���X�̃����o�֐��ł͂Ȃ�
		�Ȃ̂ŁA�⏕�N���X���쐬����
		�ԐړI�ɃR���X�g���N�^�A�f�X�g���N�^���Ăяo�� */

		// �e�N�X�`��
		std::shared_ptr<TexHelper> tex;

		// �e�N�X�`�����쐬
		tex = std::make_shared<TexHelper>(width, height);

		// �쐬�����e�N�X�`����Ԃ�
		return tex;
	}
}