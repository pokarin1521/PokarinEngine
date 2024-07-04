/**
* @file Debug.cpp
*/
#include "Debug.h"

#include <stdio.h>
#include <stdarg.h>
#include <string>

#pragma warning(push)
#pragma warning(disable:4005)
#include <Windows.h>
#pragma warning(pop)

namespace PokarinEngine
{
	namespace Debug
	{
		/// <summary>
		/// �����t����������f�o�b�O�E�B���h�E�ɕ\������
		/// </summary>
		/// <param name="[in] type"> ���b�Z�[�W�̎�� </param>
		/// <param name="[in] func"> �֐��� </param>
		/// <param name="[in] format"> �����t�������� </param>
		void Log(Type type, const char* func, const char* format, ...)
		{
			// ���b�Z�[�W�i�[�p
			char buffer[1024] = "";
			char* p = buffer;

			// ���b�Z�[�W�̃T�C�Y
			size_t size = sizeof(buffer) - 1; // ������\n��ǉ����邽��

			// ���O�̎�ނƊ֐�����ݒ�

			// ���b�Z�[�W�̎��
			static const char* const typeNames[] = { "�G���[", "�x��", "���" };

			// ���O�̎�ނƊ֐��������b�Z�[�W�Ƃ��Ċi�[
			// �i�[�������b�Z�[�W�̕��������擾
			size_t n = snprintf(p, size, "[%s] %s: ",
				typeNames[static_cast<int>(type)], func);

			// �A�����ă��b�Z�[�W�������o�������̂ŁA
			// �擪�����b�Z�[�W�̕��������炷
			p += n;

			// �c��T�C�Y�𔽉f
			size -= n;

			// ���b�Z�[�W��ݒ�
			va_list ap;

			// va_list�^�̕ϐ���������
			va_start(ap, format);

			// vsnprintf�֐��́u�o�͂����o�C�g���v��Ԃ�
			// ���̒l���o�͐���w���|�C���^p�ɉ��Z���邱�Ƃ�
			// �o�͌��ʂ��A������悤�ɂ���
			p += vsnprintf(p, size, format, ap);

			// va_list�^�̕ϐ���j��
			va_end(ap);

			// �����ɉ��s��ǉ�
			p[0] = '\n';
			p[1] = '\0';

			// �쐬������������f�o�b�O�E�B���h�E�ɕ\��
			OutputDebugString(buffer);
		}

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
			const GLchar* message, const void* userParam)
		{
			std::string s;

			if (length < 0)
			{
				s = message;
			}
			else
			{
				s.assign(message, message + length);
			}

			s += '\n'; // ���b�Z�[�W�ɂ͉��s���Ȃ��̂Œǉ�����

			// DebugCallback�ɓn����郁�b�Z�[�W�ɂ́u�G���[��ʁv���܂܂�Ă���̂ŁA
			// LOG_ERROR��LOG_WARNING���g���ƁA�G���[��ʂ���d�ɏo�͂���ĕ�����Â炢
			// LOG�Ȃ�G���[��ʂ��o�͂��Ȃ����߁A��肪�N���Ȃ�
			LOG(s.c_str());
		}

	} // namespace Debug

} // namespace PokarinEngine