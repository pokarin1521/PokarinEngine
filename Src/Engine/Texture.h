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
	public: // --------- �R���X�g���N�^�E�f�X�g���N�^ ----------

		/// <summary>
		/// �e�N�X�`���쐬�p�R���X�g���N�^
		/// </summary>
		Texture() { glCreateTextures(GL_TEXTURE_2D, 1, &id); }

		/// <summary>
		/// �e�N�X�`���폜�p�R���X�g���N�^
		/// </summary>
		~Texture() { glDeleteTextures(1, &id); }

	public: // ------------------- �֎~���� --------------------

		/* �폜�p�f�X�g���N�^�𕡐���Ă΂�Ȃ��悤�ɋ֎~���� */

		Texture(const Texture&) = delete;
		Texture& operator=(const Texture&) = delete;

	public: // --------------------- �ϊ� ----------------------

		// GLuint�^�ɕϊ�(���ʔԍ���Ԃ�)
		operator GLuint() const { return id; }

		// ImTextureID�^�ɕϊ�(���ʔԍ���Ԃ�)
		operator ImTextureID() const { return (void*)(std::intptr_t)id; }

	private: // -------------------- ��� ----------------------

		// ���ʔԍ�
		GLuint id = 0;
	};

} // namespace PokarinEngine

#endif // !POKARINENGINE_TEXTURE_H_INCLUDED
