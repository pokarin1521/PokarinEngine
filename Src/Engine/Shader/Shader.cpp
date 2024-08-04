/**
* @file Shader.cpp
*/
#include "Shader.h"

#include "../Debug.h"

#include <fstream>
#include <filesystem>

namespace PokarinEngine
{
	/// <summary>
	/// �V�F�[�_�Ǘ��p
	/// </summary>
	namespace Shader
	{
		// ------------------------------
		// �ϐ�
		// ------------------------------

		// �V�F�[�_�v���O�����̎��ʔԍ��z��
		ProgList progList;

		// ------------------------------
		// �֐�
		// ------------------------------

		/// �����ł����g��Ȃ��̂ŁAcpp�݂̂ɏ���
		/// <summary>
		/// �V�F�[�_�t�@�C����ǂݍ���ŃR���p�C������
		/// </summary>
		/// <param name="[in] type"> �V�F�[�_�̎�� </param>
		/// <param name="[in] fileName"> �V�F�[�_�t�@�C���� </param>
		/// <returns> �V�F�[�_�̎��ʔԍ� </returns>
		GLuint Compile(GLenum type, const char* fileName)
		{
			// �o�C�i�����[�h���w��
			// ���s�����̕ϊ������s��Ȃ��̂ŁA�ǂݍ��݂�����
			std::ifstream file(fileName, std::ios::binary);

			if (!file)
			{
				LOG_ERROR("%s���J���܂���", fileName);

				return 0;
			}

			//	�t�@�C����ǂݍ���
			// �t�@�C���S�̂���C�ɓǂݍ��ނ��߂ɁA�t�@�C���T�C�Y�擾
			const size_t filesize = std::filesystem::file_size(fileName);
			std::vector<char> buffer(filesize);

			// �t�@�C����ǂݍ��݁Abuffer�Ɋi�[
			file.read(buffer.data(), filesize);
			file.close();

			const char* source[] = { buffer.data() };
			const GLint length[] = { int(buffer.size()) };

			// �쐬�����V�F�[�_�I�u�W�F�N�g�̎��ʔԍ�
			const GLuint object = glCreateShader(type);

			// �\�[�X�R�[�h��ݒ肵�R���p�C��
			glShaderSource(object, 1, source, length);
			glCompileShader(object);

			return object;
		}

		/// �����ł����g��Ȃ��̂ŁAcpp�݂̂ɏ���
		/// <summary>
		/// ���_�V�F�[�_�ƃt���O�����g�V�F�[�_�������N
		/// </summary>
		/// <param name="[in] prog"> �v���O�����I�u�W�F�N�g�̎��ʔԍ� </param>
		/// <param name="[in] vertex"> ���_�V�F�[�_�̎��ʔԍ� </param>
		/// <param name="[in] fragment"> �t���O�����g�V�F�[�_�̎��ʔԍ� </param>
		void Link(GLuint prog, GLuint vertex, GLuint fragment)
		{
			// ���_�V�F�[�_���w��
			glAttachShader(prog, vertex);

			// �t���O�����g�V�F�[�_���w��
			glAttachShader(prog, fragment);

			// �w�肵���V�F�[�_�������N
			glLinkProgram(prog);
		}

		/// <summary>
		/// �V�F�[�_��������
		/// </summary>
		void Initialize()
		{
			// -----------------------------
			// �V�F�[�_�̃R���p�C��
			// -----------------------------

			// ----------- �X�^���_�[�h�V�F�[�_ ------------

			// ���_�V�F�[�_
			GLuint vertexShader = Compile(
				GL_VERTEX_SHADER, "Res/Shader/standard.vert");

			// �t���O�����g�V�F�[�_
			GLuint fragmentShader = Compile(
				GL_FRAGMENT_SHADER, "Res/Shader/standard.frag");

			// ------------ �A�����b�g�V�F�[�_ -------------

			// ���_�V�F�[�_
			GLuint vertexShaderUnlit = Compile(
				GL_VERTEX_SHADER, "Res/Shader/Unlit.vert");

			// �t���O�����g�V�F�[�_�V�F�[�_
			GLuint fragmentShaderUnlit = Compile(
				GL_FRAGMENT_SHADER, "Res/Shader/Unlit.frag");

			// --------------------------
			// �V�F�[�_�������N
			// --------------------------

			// ----------- �X�^���_�[�h�V�F�[�_ ------------

			// �쐬�����V�F�[�_�v���O�����̎��ʔԍ���ݒ�
			GLuint progStandard = glCreateProgram();

			// ���_�V�F�[�_�ƃt���O�����g�V�F�[�_�������N
			Link(progStandard, vertexShader, fragmentShader);

			// �쐬�����V�F�[�_��z��ɒǉ�
			progList.emplace(ProgType::Standard, progStandard);

			// ------------ �A�����b�g�V�F�[�_ -------------

			// �쐬�����V�F�[�_�v���O�����̎��ʔԍ���ݒ�
			GLuint progUnlit = glCreateProgram();

			// ���_�V�F�[�_�ƃt���O�����g�V�F�[�_�������N
			Link(progUnlit, vertexShaderUnlit, fragmentShaderUnlit);

			// �쐬�����V�F�[�_��z��ɒǉ�
			progList.emplace(ProgType::Unlit, progUnlit);
		}

		/// <summary>
		/// �V�F�[�_�v���O�����̎��ʔԍ����擾����
		/// </summary>
		/// <param name="[in] progType"> �V�F�[�_�v���O�����̎�� </param>
		/// <returns> �w�肵����ނ̃V�F�[�_�v���O�����̎��ʔԍ� </returns>
		GLuint GetProgram(ProgType progType)
		{
			return progList[progType];
		}

		/// <summary>
		/// �S�ẴV�F�[�_�v���O�����̎��ʔԍ����擾����
		/// </summary>
		/// <returns> 
		/// <para> �S�ẴV�F�[�_�v���O�����̎��ʔԍ� </para>
		/// <para> [�V�F�[�_�v���O�����̎��, �V�F�[�_�v���O�����̎��ʔԍ�] </para>
		/// </returns>
		const ProgList& GetAllProgram()
		{
			return progList;
		}

	} // namespace Shader

} // namespace PokarinEngine