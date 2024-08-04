/**
* @file Mesh.cpp
*/
#include "Mesh.h"

#include "MeshBuffer.h"
#include "Vertex.h"

#include "../Debug.h" 
#include "../TextureGetter.h"
#include "../BufferObject.h"
#include "../VertexArrayObject.h"

#include "../Configs/ShaderConfig.h"
#include "../Configs/MeshConfig.h"

namespace PokarinEngine
{
#pragma region Mesh

	/// <summary>
	/// ������
	/// </summary>
	void Mesh::Initialize()
	{
		// -----------------------------------
		// ���b�V���o�b�t�@���쐬����
		// -----------------------------------

		// ���b�V���o�b�t�@�̃T�C�Y
		// 32���K�o�C�g�Ƃ��A����Ȃ��ꍇ�͑傫�����邩���b�V���o�b�t�@�𑝂₷
		static const size_t meshBufferSize = 32'000'000;

		// ���b�V���o�b�t�@�쐬�p�⏕�N���X
		struct MeshBufferHelper : public MeshBuffer
		{
			MeshBufferHelper(size_t bufferSize) : MeshBuffer(bufferSize) {}
		};

		// �⏕�N���X��p����
		// ���b�V���o�b�t�@���쐬����
		meshBuffer = std::make_shared<MeshBufferHelper>(meshBufferSize);

		// -----------------------------------
		// OBJ�t�@�C����ǂݍ���
		// -----------------------------------

		// �S�Ă�OBJ�t�@�C����ǂݍ���
		for (const char* obj : StaticMeshFile_OBJ::allObj)
		{
			meshBuffer->LoadOBJ(obj);
		}
	}

	/// <summary>
	/// �X�^�e�B�b�N���b�V�����擾����
	/// </summary>
	/// <param name="fileName"> �X�^�e�B�b�N���b�V���̃t�@�C���� </param>
	/// <returns> �X�^�e�B�b�N���b�V�� </returns>
	StaticMeshPtr Mesh::GetStaticMesh(const std::string& fileName)
	{
		return meshBuffer->GetStaticMesh(fileName);
	}

	/// <summary>
	/// ���b�V����`�悷��
	/// </summary>
	/// <param name="[in] mesh"> �`�悷��X�^�e�B�b�N���b�V�� </param>
	/// <param name="[in] program"> �g�p����V�F�[�_�v���O�����̎��ʔԍ� </param>
	/// <param name="[in] materialList"> �g�p����}�e���A���z�� </param>
	void Mesh::Draw(const StaticMeshPtr& mesh, GLuint program, const MaterialList& materialList)
	{
		// ���b�V�����Ȃ���Ή������Ȃ�
		if (!mesh)
		{
			return;
		}

		// �J���[�p�����[�^���擾
		Color objectColor = Color::white;

		// �V�F�[�_����I�u�W�F�N�g�̐F���擾
		if (program)
		{
			glGetUniformfv(program, 100, &objectColor.r);
		}

		/* 1��OBJ�t�@�C���ɂ́A
		�قȂ�}�e���A�����g�������̐}�`���`���邱�Ƃ��ł���
		�������AglDrawElementsBaseVertex�֐��Ŏg����}�e���A����1��ނ���

		�Ȃ̂ŁA�}�e���A�����Ƃɕ`��f�[�^���`���A
		�`��f�[�^�P�ʂŕ`�悷��K�v������

		����ɑΉ����邽�߁AStaticMesh�\���̂�
		DrawParams��z��ŊǗ�����悤�ɂ��Ă��� */

		// ���b�V���̕`��p�����[�^�z��
		const DrawParameterList drawParameterList = mesh->GetDrawParameterList();

		// VAO���o�C���h
		glBindVertexArray(*meshBuffer->GetVAO());

		// �p�����[�^���Ƃɕ`��
		for (const auto& drawParameter : drawParameterList)
		{
			// �}�e���A����ݒ�
			// �}�e���A�������邩�m�F
			if (drawParameter.materialNo >= 0 && drawParameter.materialNo < materialList.size())
			{
				// �}�e���A���擾
				const Material& material = *materialList[drawParameter.materialNo];

				if (program)
				{
					// �}�e���A���𔽉f�����I�u�W�F�N�g�̐F
					const Color color = objectColor * material.baseColor;

					// �}�e���A���𔽉f�����I�u�W�F�N�g�̐F��
					// GPU�ɃR�s�[
					glProgramUniform4fv(program, UniformLocation::color, 1, &color.r);

					// �����F��
					// �G�~�b�V�����e�N�X�`���̎��ʔԍ���GPU�ɃR�s�[
					glProgramUniform4f(program, UniformLocation::emissionColor,
						material.emission.r,
						material.emission.g,
						material.emission.b,
						static_cast<bool>(material.texEmission));
				}

				// �}�e���A���Ƀe�N�X�`�����ݒ肳��Ă���Ȃ�
				if (material.texBaseColor)
				{
					// �e�N�X�`���̎��ʔԍ�
					const GLuint tex = *material.texBaseColor;

					// �ʏ�̐F�p�e�N�X�`�����o�C���h
					glBindTextures(TextureBinding::color, 1, &tex);

				}
				else
				{
					// �ʏ�̐F�p�e�N�X�`�����o�C���h
					glBindTextures(TextureBinding::color, 1, 0);
				}

				// �G�~�b�V�����e�N�X�`��������
				if (material.texEmission)
				{
					// �G�~�b�V�����e�N�X�`�����o�C���h
					const GLuint tex = *material.texEmission;
					glBindTextures(TextureBinding::emission, 1, &tex);
				}
				// �G�~�b�V�����e�N�X�`�����Ȃ�
				else
				{
					// �e�N�X�`��1�𖢐ݒ�ɂ���
					glBindTextures(TextureBinding::emission, 1, nullptr);
				}
			}

			// �`��
			glDrawElementsBaseVertex(
				drawParameter.mode, drawParameter.count, GL_UNSIGNED_SHORT, drawParameter.indices, drawParameter.baseVertex);
		}

		// VAO�̃o�C���h����
		glBindVertexArray(0);
	}

#pragma endregion

} // namespace PokarinEngine
