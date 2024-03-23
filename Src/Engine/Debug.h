/**
* @file Debug.h
*/
#ifndef DEBUG_H_INCLUDED
#define DEBUG_H_INCLUDED

#include "glad/glad.h"

#include <GLFW/glfw3.h>

namespace PokarinEngine
{
	/// <summary>
	/// �f�o�b�O�p�̋@�\���i�[���閼�O���
	/// </summary>
	namespace Debug
	{
		// ���b�Z�[�W�̎��
		enum class Type
		{
			error,	 // �G���[
			warning, // �x��
			info,	 // ���̑��̏��
		};

		/// <summary>
		/// �����t����������f�o�b�O�E�B���h�E�ɕ\������
		/// </summary>
		/// <param name="type"> ���b�Z�[�W�̎�� </param>
		/// <param name="func"> �֐��� </param>
		/// <param name="format"> �����t�������� </param>
		void Log(Type type, const char* func, const char* format, ...);

		/// <summary>
		/// OpenGL����̃��b�Z�[�W����������R�[���o�b�N�֐�
		/// </summary>
		/// <param name="source"> ���b�Z�[�W�̔��M��(OpenGL�AWindows�A�V�F�[�_�Ȃ�) </param>
		/// <param name="type"> ���b�Z�[�W�̎��(�G���[�A�x���Ȃ�) </param>
		/// <param name="id"> ���b�Z�[�W����ʂɎ��ʂ���l </param>
		/// <param name="serity"> ���b�Z�[�W�̏d�v�x(���A���A��A�Œ�) </param>
		/// <param name="length"> ���b�Z�[�W�̕�����. �����Ȃ烁�b�Z�[�W��0�I�[����Ă��� </param>
		/// <param name="message"> ���b�Z�[�W�{�� </param>
		/// <param name="userParam"> �R�[���o�b�N�ݒ莞�Ɏw�肵���|�C���^ </param>
		void APIENTRY Callback(GLenum source, GLenum type,
			GLuint id, GLenum serity, GLsizei length,
			const GLchar* message, const void* userParam);

	} // namespace Debug

	// Relese�r���h�̎�������`�����}�N��
	// �f�o�b�O�@�\�Ȃ�
#ifndef NDEBUG

// ���̃}�N�����`����ƃf�o�b�O�o�͂��L���ɂȂ�
#define ENABLE_DEBUG_LOG 

#endif // !NDEBUG

// �f�o�b�O�o�̓}�N��
#ifdef ENABLE_DEBUG_LOG

// ���s���Ɏx������������x���̖��
#define LOG_ERROR(...)	 Debug::Log(Debug::Type::error, __func__, __VA_ARGS__)

// �ݒ�~�X�Ȃǂ̌y���Ȗ��
#define LOG_WARNING(...) Debug::Log(Debug::Type::warning, __func__, __VA_ARGS__)

// ���ł͂Ȃ����A���Ƃ��ďo�͂��������b�Z�[�W
// (�m�F�̂��߂ɁA�ǂݍ��񂾃t�@�C�������o�͂���Ȃ�)
#define LOG(...)		 Debug::Log(Debug::Type::info, __func__, __VA_ARGS__)

// �f�o�b�O�o�͂�����ƁA�Q�[���̎��s���x���x���Ȃ邽��
// �f�o�b�O���ȊO�ł͖����ɂ���
#else

// �}�N���̒��g��((void)0)�ɒu��������
// LOG_ERROR(); -> (void)0; �ɂȂ�
// �󕶂ɂ��Ȃ��̂́A�P�Ƃ́u;�v�ɑ΂��Čx�����������邽��
#define LOG_ERROR(...)	 ((void)0)
#define LOG_WARNING(...) ((void)0)
#define LOG(...)		 ((void)0)

#endif // ENABLE_DEBUG_LOG

} // namespace PokarinEngine

#endif // !DEBUG_H_INCLUDED