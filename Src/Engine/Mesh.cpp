/**
* @file Mesh.cpp
*/
#define _CRT_SECURE_NO_WARNINGS

#include "Mesh.h"
#include "Debug.h" 

#include "Configs/ShaderConfig.h"

#include <numeric>

#include <algorithm>
#include <filesystem>
#include <fstream>

namespace PokarinEngine
{
	/// <summary>
	/// ���b�V����`�悷��
	/// </summary>
	/// <param name="[in] mesh"> �`�悷��X�^�e�B�b�N���b�V�� </param>
	/// <param name="[in] program"> �g�p����V�F�[�_�v���O�����̊Ǘ��ԍ� </param>
	/// <param name="[in] materials"> �g�p����}�e���A���z�� </param>
	void Draw(const StaticMeshPtr& mesh, GLuint program, const MaterialList& materials)
	{
		// �J���[�p�����[�^���擾
		Color objectColor = Color(0);

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

		for (const auto& drawParameter : mesh->drawParamList)
		{
			// �}�e���A����ݒ�
			// �}�e���A�������邩�m�F
			if (drawParameter.materialNo >= 0 && drawParameter.materialNo < materials.size())
			{
				// �}�e���A���擾
				const Material& material = *materials[drawParameter.materialNo];

				if (program)
				{
					// �}�e���A���𔽉f�����I�u�W�F�N�g�̐F
					const Color color = objectColor * material.baseColor;

					// �}�e���A���𔽉f�����I�u�W�F�N�g�̐F��
					// GPU�ɃR�s�[
					glProgramUniform4fv(program, UniformLocation::color, 1, &color.r);

					// �����F��
					// �G�~�b�V�����e�N�X�`���̊Ǘ��ԍ���GPU�ɃR�s�[
					glProgramUniform4f(program, UniformLocation::emissionColor,
						material.emission.r,
						material.emission.g,
						material.emission.b,
						static_cast<bool>(material.texEmission));
				}

				// �}�e���A���Ƀe�N�X�`�����ݒ肳��Ă���Ȃ�
				if (material.texBaseColor)
				{
					// �e�N�X�`���̊Ǘ��ԍ�
					const GLuint tex = *material.texBaseColor;

					// �ʏ�̐F�p�e�N�X�`�����o�C���h
					glBindTextures(TextureBinding::color, 1, &tex);
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
	}

	/// <summary>
	/// ���L�}�e���A���z����R�s�[
	/// </summary>
	/// <param name="[in] original"> �R�s�[���}�e���A���z��̎����� </param>
	/// <returns> ���L�}�e���A���z��̃N���[�� </returns>
	MaterialList CloneMaterialList(const StaticMeshPtr& original)
	{
		// ���L�}�e���A���z��̃N���[��
		MaterialList clone(original->materials.size());

		// ���L�}�e���A���z����R�s�[
		for (int i = 0; i < clone.size(); ++i)
		{
			clone[i] = std::make_shared<Material>(*original->materials[i]);
		}

		return clone;
	}

	/// <summary>
	/// �����Ă���@����₤
	/// </summary>
	/// <param name="[in,out] vertices"> ���_�z�� </param>
	/// <param name="[in] vertexCount"> ���_�z��̗v�f�� </param>
	/// <param name="[in] indices"> �C���f�b�N�X�z�� </param>
	/// <param name="[in] indexCount"> �C���f�b�N�X�z��̗v�f�� </param>
	void FillMissingNormals(Vertex* vertices, size_t vertexCount,
		const uint16_t* indices, size_t indexCount)
	{
		// ---------------------------------------
		// �@�����ݒ肳��Ă��Ȃ����_��������
		// ---------------------------------------

		// �@���m�F�p
		// false : �@�����ݒ肳��Ă���
		// true : �@�����ݒ肳��Ă��Ȃ�
		std::vector<bool> missingNormals(vertexCount, false);

		for (int i = 0; i < vertexCount; ++i)
		{
			// �@���x�N�g��
			const Vector3& n = vertices[i].normal;

			// �@���̒�����0�̏ꍇ���u�ݒ肳��Ă��Ȃ��v�Ƃ݂Ȃ�
			if (n.x == 0 && n.y == 0 && n.z == 0)
			{
				missingNormals[i] = true;
			}
		}

		// ----------------
		// �@�����v�Z
		// ----------------

		/* ���񋁂߂�@���́u���_���܂ރ|���S���ʂ̖@���̕��ρv�ƒ�`����

		���_�̖@�����v�Z���邽�߂ɁA�܂��u�ʂ̖@���v�����߂�

		�ʂ̖@���́u�ʂɑ΂��Đ����ȃx�N�g���v��
		����́u�O�ρv�ɂ���Čv�Z�ł��� */

		// �O�ς����߂邽�߂ɁA
		// ���_���W����ʂ��\������2�ӂ����߂�
		for (int i = 0; i < indexCount; i += 3)
		{
			// ���_3�̃C���f�b�N�X�擾
			const int i0 = indices[i + 0];
			const int i1 = indices[i + 1];
			const int i2 = indices[i + 2];

			// ���_3�̍��W���擾
			const Vector3& v0 = vertices[i0].position;
			const Vector3& v1 = vertices[i1].position;
			const Vector3& v2 = vertices[i2].position;

			// �擾�������_���W����
			// �ʂ��\������2��a, b�����߂�
			const Vector3 a = { v1.x - v0.x, v1.y - v0.y, v1.z - v0.z };
			const Vector3 b = { v2.x - v0.x, v2.y - v0.y, v2.z - v0.z };

			// �O�ςɂ����a��b�ɐ����ȃx�N�g��(�@��)�����߂�
			const float cx = a.y * b.z - a.z * b.y;
			const float cy = a.z * b.x - a.x * b.z;
			const float cz = a.x * b.y - a.y * b.x;

			// �@���𐳋K�����ĒP�ʃx�N�g���ɂ���
			// �@���x�N�g���́u������\���x�N�g���v�Ȃ̂ŁA
			// �P�ʃx�N�g���ɂ��Ă����ƍ���̌v�Z���y�ɂȂ�

			// �x�N�g���̑傫��
			const float l = sqrt(cx * cx + cy * cy + cz * cz);

			// ���K�������@���x�N�g��
			const Vector3 normal = { cx / l, cy / l, cz / l };

			// �@�����ݒ肳��Ă��Ȃ����_�ɂ����@�������Z
			// �����̖ʂ̖@�������Z���邱�ƂŁA�@���̌����𕽋ω�����
			if (missingNormals[i0])
			{
				vertices[i0].normal += normal;
			}

			if (missingNormals[i1])
			{
				vertices[i1].normal += normal;
			}

			if (missingNormals[i2])
			{
				vertices[i2].normal += normal;
			}
		}

		// ----------------
		// �@���𐳋K��  
		// ----------------

		// �@�������Z�������ƂŁA���K������Ă��Ȃ���ԂɂȂ����̂�
		// ���ω����ꂽ�@���𐳋K������
		for (int i = 0; i < vertexCount; ++i)
		{
			if (missingNormals[i])
			{
				// �@���x�N�g��
				Vector3& n = vertices[i].normal;

				// �x�N�g���̑傫��
				const float l = sqrt(n.x * n.x + n.y * n.y + n.z * n.z);

				// �@���x�N�g���𐳋K��
				n = { n.x / l, n.y / l, n.z / l };
			}

		} // for i
	}

	/// <summary>
	/// �R���X�g���N�^
	/// </summary>
	/// <param name="[in] bufferSize"> �o�b�t�@�T�C�Y(�o�C�g��) </param>
	MeshBuffer::MeshBuffer(size_t bufferSize)
	{
		// �o�b�t�@�I�u�W�F�N�g���쐬
		// ���I�Ƀ��������m�ۏo����悤�Ƀt���O��ݒ肷��
		buffer = BufferObject::Create(
			bufferSize, nullptr, GL_DYNAMIC_STORAGE_BIT);

		// VAO���쐬
		vao = VertexArrayObject::Create();

		/* ��{�I��OpenGL�̊֐��́A
		OpenGL�R���e�L�X�g�Ɋ��蓖�Ă��Ă���I�u�W�F�N�g�𑀍삷��d�g��
		���̂��߁A�֐������s����O�ɁA
		���삵�����I�u�W�F�N�g��OpenGL�R���e�L�X�g�Ɋ��蓖�Ă� */

		// VAO��OpenGL�R���e�L�X�g�Ƀo�C���h
		glBindVertexArray(*vao);

		/* VBO�͌X�̒��_�����Ƀo�C���h�����
		����͒��_�������ƂɈقȂ�VBO���o�C���h�o����Ƃ�������

		�o�C���h�́uglVertexAttribArray�֐������s���ꂽ�Ƃ��v�ɍs����
		glBindBuffer�֐��́uVBO��OpenGL�R���e�L�X�g�Ƀo�C���h�v���邾���ŁA
		���_�����ɂ̓o�C���h����Ȃ�

		IBO�̏ꍇ��glBindBuffer�֐��ɂ����VAO�Ƀo�C���h����� */

		// VBO��IBO��
		// VAO��OpenGL�R���e�L�X�g�Ƀo�C���h
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, *buffer);
		glBindBuffer(GL_ARRAY_BUFFER, *buffer);

		// ���_������ݒ�
		vao->SetAttribute(0, 3, sizeof(Vertex), offsetof(Vertex, position));
		vao->SetAttribute(1, 2, sizeof(Vertex), offsetof(Vertex, texcoord));
		vao->SetAttribute(2, 3, sizeof(Vertex), offsetof(Vertex, normal));

		// ��������삪�s���Ȃ��悤�Ƀo�C���h������
		glBindVertexArray(0);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
		glBindBuffer(GL_ARRAY_BUFFER, 0);

		// �X�^�e�B�b�N���b�V���̗e�ʂ�\��
		staticMeshList.reserve(100);

		// �`��p�����[�^�̗e�ʂ�\��
		drawParamList.reserve(100);
	}

	/// <summary>
	/// MTL�t�@�C����ǂݍ���
	/// </summary>
	/// <param name="[in] folderName"> OBJ�t�@�C���̂���t�H���_�� </param>
	/// <param name="[in] fileName"> MTL�t�@�C���� </param>
	/// <returns> MTL�t�@�C���Ɋ܂܂��}�e���A���z�� </returns>
	std::vector<MaterialPtr> MeshBuffer::LoadMTL(
		const std::string& folderName, const char* fileName)
	{
		// ---------------------
		// MTL�t�@�C�����J��
		// ---------------------

		// MTL�t�@�C���̃p�X
		// OBJ�t�@�C���ɂ����āAMTL�t�@�C�����͑��΃p�X�Ŏw�肳���
		// ���̂��߁AOBJ�t�@�C���̃t�H���_����⑫����K�v������
		const std::string fullpath = folderName + fileName;
		std::ifstream file(fullpath);

		// �t�@�C�����J���Ȃ�
		if (!file)
		{
			// �G���[�\��
			LOG_ERROR("%s���J���܂���", fullpath.c_str());

			// �ǂݍ��ݒ��f
			return{};
		}

		// -----------------------
		// MTL�t�@�C������͂���
		// -----------------------

		// �}�e���A���z��
		std::vector<MaterialPtr> materials;

		// �}�e���A��
		MaterialPtr pMaterial;

		// �t�@�C���̏I�[�܂�
		while (!file.eof())
		{
			// 1�s���ǂݍ���
			std::string line;
			std::getline(file, line);

			// -------- �}�e���A����`�̓ǂݎ������݂� ----------

			// �}�e���A�����i�[�p
			char name[1000] = { 0 };

			// %s�ŕ������ǂݎ��Ƃ��A%13s�̂悤��s�̎�O�ɐ����������ƁA
			// �ǂݎ�镶�����𐧌����邱�Ƃ��ł���(%13s = �ő�13����)
			// ������\0���ǉ�����邱�Ƃ��l�����āA�u�z��T�C�Y - 1�v�̒l���w�肷��
			// �����name�z��̃T�C�Y�ɍ��킹�čő�999�����Ƃ��Ă���

			// �}�e���A������ǂݎ��
			if (sscanf(line.data(), " newmtl %999s", name) == 1)
			{
				// �ǂݎ�����}�e���A����ǉ�
				pMaterial = std::make_shared<Material>();
				pMaterial->name = name;
				materials.push_back(pMaterial);

				// ���̍s��
				continue;
			}

			// �}�e���A������`����Ă��Ȃ��ꍇ�͍s�𖳎�����
			if (!pMaterial)
			{
				// ���̍s��
				continue;
			}

			// ---------- ��{�F�̓ǂݎ������݂� ------------

			// ��{�F��ǂݎ��
			if (sscanf(line.data(), " Kd %f %f %f",
				&pMaterial->baseColor.r,
				&pMaterial->baseColor.g,
				&pMaterial->baseColor.b) == 3)
			{
				// ���̍s��
				continue;
			}

			// ---------- �����x�̓ǂݎ������݂� ------------

			// �����x��ǂݎ��
			if (sscanf(line.data(), " d %f", &pMaterial->baseColor.a) == 1)
			{
				// ���̍s��
				continue;
			}

			// ---------- ��{�F�e�N�X�`�����̓ǂݎ������݂� ---------

			// �e�N�X�`�����i�[�p
			char textureName[1000] = { 0 };

			// ��{�F�e�N�X�`������ǂݎ��
			// �z��T�C�Y�𒴂��Ȃ��悤�ɁA�ǂݍ��ޕ������𐧌�
			if (sscanf(line.data(), " map_Kd %999s", &textureName) == 1)
			{
				// �e�N�X�`���t�@�C���̃p�X
				// MTL�t�@�C�����Ɠ��l�ɁAmap_Kd�\���̃e�N�X�`�����͑��΃p�X�Ŏw�肳���
				// ���̂��߁AOBJ�t�@�C���̃t�H���_����⑫����
				const std::string fullpath = folderName + textureName;

				// �w�肵���t�@�C���܂��̓t�H���_��
				// ���݂���
				if (std::filesystem::exists(fullpath))
				{
					// �e�N�X�`����ǂݍ���
					pMaterial->texBaseColor = textureCallback(fullpath.c_str());
				}
				// ���݂��Ȃ�
				else
				{
					// �x����\��
					LOG_WARNING("%s���J���܂���", fullpath.c_str());
				}

				// ���̍s��
				continue;
			}

			// --------------- �����F�̓ǂݎ������݂� ----------------

			if (sscanf(line.data(), " Ke %f %f %f",
				&pMaterial->emission.r, &pMaterial->emission.g,
				&pMaterial->emission.b) == 3)
			{
				// ���̍s��
				continue;
			}

			// --------- �����F�e�N�X�`�����̓ǂݎ������݂� ----------

			if (sscanf(line.data(), " map_Ke %999s", &textureName) == 1)
			{
				// �e�N�X�`���t�@�C��
				const std::string filename = folderName + textureName;

				// �t�@�C�������݂���
				if (std::filesystem::exists(filename))
				{
					// �G�~�b�V�����e�N�X�`����ǂݍ���
					pMaterial->texEmission = textureCallback(filename.c_str());
				}
				// �t�@�C�������݂��Ȃ�
				else
				{
					// �x��
					LOG_WARNING("%s���J���܂���", filename.c_str());
				}

				// ���̍s��
				continue;

			} // if map_Ke
		}

		// �ǂݍ��񂾃}�e���A���̔z���Ԃ�
		return materials;
	}

	/// <summary>
	/// OBJ�t�@�C����ǂݍ���
	/// </summary>
	/// <param name="[in] fileName"> OBJ�t�@�C���� </param>
	/// <returns> 
	/// <para> filename����쐬�����X�^�e�B�b�N���b�V�� </para>
	/// <para> �t�@�C�����J���Ȃ������ꍇ��nullptr��Ԃ� </para>
	/// </returns>
	StaticMeshPtr MeshBuffer::LoadOBJ(const char* fileName)
	{
		// --------------------------------------------------------
		// �ȑO�ɓǂݍ��񂾃t�@�C���Ȃ�A�쐬�ς݂̃��b�V����Ԃ�
		// --------------------------------------------------------
		{
			// �t�@�C�����Ɗ֘A�t����ꂽ�f�[�^�����邩���ׂ�

			// �L�[�Ō���
			auto itr = staticMeshList.find(fileName);

			// ���������烁�b�V����Ԃ�
			if (itr != staticMeshList.end())
			{
				return itr->second;
			}

		}

		// ------------------------------
		// OBJ�t�@�C�����J��
		// ------------------------------
		std::ifstream file(fileName);

		// �t�@�C�����J���Ȃ�
		if (!file)
		{
			// �t�@�C���ǂݍ��݂𒆒f
			return nullptr;
		}

		// ------ OBJ�t�@�C���̂���t�H���_�����擾 -------

		// OBJ�t�@�C���̂���t�H���_��
		std::string foldername(fileName);
		{
			// ��ԍŌ�̋�؂蕶���̈ʒu��T��
			const size_t p = foldername.find_last_of("\\ /");

			// ��؂蕶�������������ꍇ
			if (p != std::string::npos)
			{
				// ��؂蕶���̂Ƃ���܂ŃT�C�Y���k�߂邱�Ƃ�
				// �t�H���_�����擾����
				// �v�f�ԍ���0�Ԃ���Ȃ̂�+1����
				foldername.resize(p + 1);
			}
		}

		// -------------------------------------------
		// OBJ�t�@�C������͂��āA
		// ���_�f�[�^�ƃC���f�b�N�X�f�[�^��ǂݍ���
		// -------------------------------------------

		/* �f�[�^�\�����Ⴄ���߁A
		OBJ�t�@�C���̃f�[�^�����̂܂ܕ`��Ɏg�����Ƃ͂ł��Ȃ�
		���̂��߁A�Ƃ肠�����S�Ẵf�[�^��ǂݍ���ł����āA
		���ꂩ��OpenGL�ŕ`��ł���f�[�^�ɕϊ����� */

		// ���_���W
		std::vector<Vector3> positions;

		// �e�N�X�`�����W
		std::vector<Vector2> texcoords;

		// �@���x�N�g��
		std::vector<Vector3> normals;

		// ���_���W, �e�N�X�`�����W, �@���x�N�g����
		// �C���f�b�N�X�f�[�^�̑g
		struct IndexSet
		{
			// v = ���_���W�̗v�f�ԍ�, 
			// vt = �e�N�X�`�����W�̗v�f�ԍ�
			// vn = �@���x�N�g���̗v�f�ԍ�(0 : �@���Ȃ�)
			int v, vt, vn;
		};

		// �C���f�b�N�X�f�[�^�̑g���Ǘ�����z��
		std::vector<IndexSet> indexSets;

		// �z��̗e�ʂ�\��
		// �C���f�b�N�X�f�[�^��3��1�̎O�p�`�����̂ŁA
		// ���_���Ɠ������̎O�p�`��������悤�ɁA
		// �C���f�b�N�X�f�[�^�̏����e�ʂ͒��_�f�[�^��3�{�ɂ���
		positions.reserve(20'000);
		texcoords.reserve(20'000);
		normals.reserve(20'000);
		indexSets.reserve((size_t)20'000 * 3);

		// �}�e���A���z��
		std::vector<MaterialPtr> materials;

		// �z��̗e�ʂ�\��
		materials.reserve(100);

		// �}�e���A���̎g�p�͈�
		struct UseMaterial
		{
			std::string name;	// �}�e���A����
			size_t startOffset; // ���蓖�Ĕ͈͂̐擪�ʒu
		};

		// �g�p�}�e���A���z��
		std::vector<UseMaterial> usemtls;
		usemtls.reserve(100);

		// ���f�[�^��ǉ�
		// �}�e���A���w�肪�Ȃ��ꍇ�A
		// �f�t�H���g�̃}�e���A����ݒ�ł���悤�ɂ��邽��
		usemtls.push_back({ std::string(), 0 });

		// --------- �t�@�C���̏I�[�܂œǂݍ��� ----------

		while (!file.eof())
		{
			// 1�s���ǂݎ��
			std::string line;
			std::getline(file, line);
			const char* p = line.c_str();

			/* sscanf(�ǂݍ��݃o�b�t�@, �����w�蕶����, �f�[�^�i�[��A�h���X, ...);

			scanf�́u�W�����́v����f�[�^��ǂݎ�邪�A
			sscanf�́u�������Ŏw�肵���o�b�t�@�v����f�[�^��ǂݎ��
			����ȊO�͓�������

			�߂�l�́u�ǂݎ��ɐ��������f�[�^�̐��v*/

			// -----------------------------
			// ���_���W�̓ǂݎ������݂�
			// -----------------------------

			// �f�[�^�i�[�p(���_���W)
			Vector3 v(0);

			//�u�ǂݎ�����f�[�^�̐��v���u�f�[�^�i�[�p�ϐ��̐��v�ƈ�v������
			// �ǂݎ�萬���Ɣ��f
			// 
			// (�����w�蕶����̐擪�ɋ󔒂�����̂́A
			// �s���̋󔒂�^�u���΂��Ă���邽��
			// �C���f���g������ꍇ�ɑΉ��ł���)
			if (sscanf(p, " v %f %f %f", &v.x, &v.y, &v.z) == 3)
			{
				// �ǂݎ�����f�[�^��ǉ�
				positions.push_back(v);

				// ���̍s�̏�����
				continue;
			}

			// ------------------------------------
			// �e�N�X�`�����W�̓ǂݎ������݂�
			// ------------------------------------

			// �f�[�^�i�[�p(�e�N�X�`�����W)
			Vector2 vt(0);

			// �u�ǂݎ�����f�[�^�̐��v���u�f�[�^�i�[�p�ϐ��̐��v�ƈ�v������
			// �ǂݎ�萬���Ɣ��f		
			//
			// (�����w�蕶����̐擪�ɋ󔒂�����̂́A
			//  �s���̋󔒂�^�u���΂��Ă���邽��
			//  �C���f���g������ꍇ�ɑΉ��ł���)
			if (sscanf(p, " vt %f %f", &vt.x, &vt.y) == 2)
			{
				// �ǂݎ�����f�[�^��ǉ�
				texcoords.push_back(vt);

				// ���̍s�̏�����
				continue;
			}

			// --------------------------
			// �@���̓ǂݎ������݂�
			// --------------------------

			// �f�[�^�i�[�p(�@���x�N�g��)
			Vector3 vn(0);

			// �@����ǂݎ��
			if (sscanf(p, " vn %f %f %f", &vn.x, &vn.y, &vn.z) == 3)
			{
				// �@���x�N�g����ǉ�
				normals.push_back(vn);

				// ���̍s��
				continue;
			}

			// ----------------------------------------
			// �C���f�b�N�X�f�[�^�̓ǂݎ������݂�
			// ----------------------------------------

			/* f�\��(GL_TRIANGLES)��3�ȏ�̃C���f�b�N�X�f�[�^�̑g������

			�܂�1,2,3�Ԗڂ�3�ŎO�p�`�����A
			����1,3,4�ԖځA���̎���1,4,5�ԖڂƂ����悤��
			1�Ԗڂ̒��_�����L���Ď��X�ɎO�p�`���`����

			���̓�����������邽�߂ɁA
			�܂��ŏ���sscanf��1,2�Ԗڂ̃f�[�^���擾��
			����sscanf��3�Ԗڈȍ~�̃f�[�^���擾���O�p�`���쐬���� */

			// �f�[�^�i�[�p
			IndexSet f0, f1, f2;
			f0 = f1 = f2 = { 0 };

			// �ǂݎ�����������̊i�[�p
			int readByte;

			// -------- ���_���W + �e�N�X�`�����W + �@�� ----------

			// 1,2�Ԗڂ̃f�[�^���擾
			// %n�́u%n�������܂łɓǂݎ�����������������Ɋi�[����v�Ƃ�������
			if (sscanf(p, " f %u/%u/%u %u/%u/%u%n",
				&f0.v, &f0.vt, &f0.vn, &f1.v, &f1.vt, &f1.vn, &readByte) == 6)
			{
				p += readByte; // �ǂݎ��ʒu���X�V

				// 3�Ԗڈȍ~�̃f�[�^���擾���O�p�`���쐬����
				for (;;)
				{
					// �f�[�^���ǂݎ��Ȃ�������I��
					if (sscanf(p, " %u/%u/%u%n", &f2.v, &f2.vt, &f2.vn, &readByte) != 3)
					{
						break;
					}

					p += readByte; // �ǂݎ��ʒu���X�V

					// �ǂݎ�����f�[�^��ǉ�
					indexSets.push_back(f0);
					indexSets.push_back(f1);
					indexSets.push_back(f2);

					f1 = f2; // ���̎O�p�`�̂��߂Ƀf�[�^���ړ�
				}

				continue;
			}

			// -------- ���_���W + �e�N�X�`�����W ----------

			// 1,2�Ԗڂ̃f�[�^���擾
			// %n�́u%n�������܂łɓǂݎ�����������������Ɋi�[����v�Ƃ�������
			if (sscanf(p, " f %u/%u %u/%u%n",
				&f0.v, &f0.vt, &f1.v, &f1.vt, &readByte) == 4)
			{
				f0.vn = f1.vn = 0; // �@���Ȃ�
				p += readByte; // �ǂݎ��ʒu���X�V

				// 3�Ԗڈȍ~�̃f�[�^���擾���O�p�`���쐬����
				for (;;)
				{
					// �f�[�^���ǂݎ��Ȃ�������I��
					if (sscanf(p, " %u/%u%n", &f2.v, &f2.vt, &readByte) != 2)
					{
						break;
					}

					f2.vn = 0; // �@���Ȃ�
					p += readByte; // �ǂݎ��ʒu���X�V

					// �ǂݎ�����f�[�^��ǉ�
					indexSets.push_back(f0);
					indexSets.push_back(f1);
					indexSets.push_back(f2);

					f1 = f2; // ���̎O�p�`�̂��߂Ƀf�[�^���ړ�
				}

				continue;
			}

			// --------------------------------
			// MTL�t�@�C���̓ǂݎ������݂�
			// --------------------------------

			// MTL�t�@�C����
			char mtlFilename[1000] = "";

			// MTL�t�@�C������ǂݎ��
			if (sscanf(line.data(), " mtllib %999s", mtlFilename) == 1)
			{
				// �ǂݎ�������O��MTL�t�@�C����ǂݍ���
				const auto tmp = LoadMTL(foldername, mtlFilename);

				// �}�e���A���z��ɒǉ�
				materials.insert(materials.end(), tmp.begin(), tmp.end());

				continue;
			}

			// ------------------------------------
			// �g�p�}�e���A�����̓ǂݎ������݂�
			// ------------------------------------

			// �g�p�}�e���A����
			char mtlName[1000] = "";

			// �g�p�}�e���A������ǂݎ��
			if (sscanf(line.data(), " usemtl %999s", mtlName) == 1)
			{
				// �ǂݎ�����g�p�}�e���A������
				// usemtl�������������_�ł�
				// �C���f�b�N�X�f�[�^�z��T�C�Y��ǉ�
				// (�}�e���A�����蓖�Ĕ͈͂��m�F����̂Ɏg��)
				usemtls.push_back({ mtlName, indexSets.size() });

				// ���̍s��
				continue;
			}
		} // while eof

		// --------------- �ԕ��̒ǉ� ----------------

		/* �Ō���ɂ���}�e���A���̊��蓖�Ĕ͈͂�ݒ�ł���悤��
		�����ɔԕ���ǉ�
		�ԕ���ǉ��������̃T�C�Y���ǂݍ��ݏI���l�ƂȂ�
		(�ԕ� : �z��⃊�X�g�̏I�[�ɒu������ȃf�[�^) */

		usemtls.push_back({ std::string(), indexSets.size() });

		// ---------- f�\���ƒ��_�C���f�b�N�X�́u�Ή��\�v-----------

		/* ------ �Ή��\ --------
		f�\����Y�����A���_�C���f�b�N�X��l�Ƃ���z��

		f�\���̒l��Y�����ɂ��āA
		���_�C���f�b�N�X��l�Ƃ��ċL�^�����
		v, vn, vt�̂��ꂼ�ꂪint�^�ŕێ�����A
		�g�ݍ��킹�̑������u20�� �~ 20�� �~ 20���v�Ƃ����c��Ȑ��ɂȂ�

		������݂��邽�߂ɁA
		int�^��32bit�̓��A����20bit��������
		20bit�͖�100���܂ň�����̂ŁA
		�Q�[���Ɏg��3D���f���ɂƂ��Ă͏\�� */

		// OBJ�t�@�C����f�\��(�C���f�b�N�X�f�[�^�̑g)��
		// OpenGL�̒��_�C���f�b�N�X�z��̑Ή��\
		// (�L�[�́uv,vt,vn (20bit)�v, �l�͒��_�C���f�b�N�X)
		std::unordered_map<uint64_t, uint16_t> indexMap;
		indexMap.reserve(10'000);

		// --------------------------------------------------
		// �ǂݍ��񂾃f�[�^���AOpenGL�Ŏg����f�[�^�ɕϊ�
		// --------------------------------------------------

		// ���_�f�[�^�z��
		std::vector<Vertex> vertices;
		vertices.reserve(indexSets.size());

		// �C���f�b�N�X�f�[�^�z��
		std::vector<uint16_t> indices;
		indices.reserve(indexSets.size());

		// �C���f�b�N�X�f�[�^�ɑΉ����钸�_�f�[�^����
		// Vertex�^�̍쐬�ƁA���ۂ̕`��Ŏg�p����C���f�b�N�X�f�[�^�̍쐬
		for (const auto& indexSet : indexSets)
		{
			// -------- f�\���̒l��64�r�b�g�́u�L�[�v�ɕϊ� ---------

			// �Ή��\�̃L�[
			// 20bit�����炵�Đݒ�
			const uint64_t key = static_cast<uint64_t>(indexSet.v) +
				(static_cast<uint64_t>(indexSet.vt) << 20) +
				(static_cast<uint64_t>(indexSet.vn) << 40);

			// -------- ���_�C���f�b�N�X��z��ɒǉ� ---------

			// �Ή��\����L�[�Ɉ�v����f�[�^������
			// ������Ȃ������ꍇ�Aend()������
			const auto itr = indexMap.find(key);

			// �f�[�^���Ή��\�ɂ���̂�
			if (itr != indexMap.end())
			{
				// �����̒��_�C���f�b�N�X���g��
				indices.push_back(itr->second);
			}
			// �f�[�^���Ή��\�ɂȂ�
			else
			{
				// �V�������_�f�[�^���쐬
				Vertex v = { Vector3(0), Vector2(0), Vector3(0) };

				/* �Y�����u-1�v���Ă���̂́A
				OBJ�t�@�C���̃C���f�b�N�X��1����n�܂�̂ɑ΂��āA
				C++����̓Y����0����n�܂邽�� */

				// ���_���W�̗v�f�ԍ�
				size_t vertexIndex = static_cast<size_t>(indexSet.v - 1);

				// �e�N�X�`�����W�̗v�f�ԍ�
				size_t texIndex = static_cast<size_t>(indexSet.vt - 1);

				// �@���x�N�g���̗v�f�ԍ�
				size_t normalIndex = static_cast<size_t>(indexSet.vn - 1);

				v.position = positions[vertexIndex];
				v.texcoord = texcoords[texIndex];

				// �@�����ݒ肳��Ă��Ȃ��ꍇ��0��ݒ�(���ƂŌv�Z)
				if (indexSet.vn == 0)
				{
					v.normal = { 0, 0, 0 };
				}
				// �@�����ݒ肳��Ă���ꍇ�͎擾
				else
				{
					v.normal = normals[normalIndex];
				}

				// �쐬�������_�f�[�^��ǉ�
				vertices.push_back(v);

				// �V�������_�f�[�^�̃C���f�b�N�X���A
				// ���_�C���f�b�N�X�z��ɒǉ�
				const uint16_t index = static_cast<uint16_t>(vertices.size() - 1);
				indices.push_back(index);

				// �L�[�ƒ��_�C���f�b�N�X�̃y�A��Ή��\�ɒǉ�
				indexMap.emplace(key, index);
			}
		}

		// �ݒ肳��Ă��Ȃ��@����₤
		FillMissingNormals(vertices.data(), vertices.size(),
			indices.data(), indices.size());

		// �ϊ������f�[�^���o�b�t�@�ɒǉ�
		AddVertexData(
			vertices.data(), vertices.size() * sizeof(Vertex),
			indices.data(), indices.size() * sizeof(uint16_t));

		// ---------------------
		// ���b�V�����쐬
		// ---------------------

		// ���b�V��
		auto pMesh = std::make_shared<StaticMesh>();

		// �f�[�^�̈ʒu
		const void* indexOffset = drawParamList.back().indices;

		// �C���f�b�N�X0�ƂȂ钸�_�z��̗v�f�ԍ�
		const GLuint baseVertex = drawParamList.back().baseVertex;

		// ------- �}�e���A���ɑΉ������`��p�����[�^���쐬 -------

		size_t i = 0;

		// ���f�[�^�Ɣԕ��ȊO�̃}�e���A��������ꍇ�A���f�[�^���΂�
		if (usemtls.size() > 2)
		{
			i = 1;
		}

		// ���̗v�f�𒲂ׂ�̂ŁAsize() - 1�܂�
		for (; i < usemtls.size() - 1; ++i)
		{
			// �g�p���̃}�e���A��
			const UseMaterial& cur = usemtls[i];

			// ���̃}�e���A��
			const UseMaterial& next = usemtls[i + 1];

			// �C���f�b�N�X�f�[�^���Ȃ��ꍇ�͔�΂�
			if (next.startOffset == cur.startOffset)
			{
				continue;
			}

			// �`��p�����[�^���쐬
			DrawParameter params;

			// �v���~�e�B�u�̎��
			params.mode = GL_TRIANGLES;

			// �`�悷��C���f�b�N�X��
			// ���̃}�e���A�����蓖�Ĕ͈͂̐擪�ʒu�܂�
			params.count = static_cast<GLsizei>(next.startOffset - cur.startOffset);

			// �`��J�n�C���f�b�N�X�̈ʒu
			params.indices = indexOffset;

			// �C���f�b�N�X0�ƂȂ钸�_�z��̗v�f�ԍ�
			params.baseVertex = baseVertex;
			params.materialNo = 0; // �f�t�H���g�l��ݒ�

			// ���O�̈�v����}�e���A����T��
			for (int i = 0; i < materials.size(); ++i)
			{
				if (materials[i]->name == cur.name)
				{
					// ���O�̈�v����}�e���A����ݒ�
					params.materialNo = i;

					break;
				}
			}

			// �쐬�����`��p�����[�^��ǉ�
			pMesh->drawParamList.push_back(params);

			// �C���f�b�N�X�I�t�Z�b�g��ύX
			// �`�悵�������炷
			indexOffset = reinterpret_cast<void*>(
				reinterpret_cast<size_t>(indexOffset) + sizeof(uint16_t) * params.count);
		}

		// �}�e���A���z�񂪋�̏ꍇ
		if (materials.empty())
		{
			// �f�t�H���g�}�e���A����ǉ�
			pMesh->materials.push_back(std::make_shared<Material>());
		}
		// �}�e���A��������ꍇ
		else
		{
			// �}�e���A�����R�s�[
			pMesh->materials.assign(materials.begin(), materials.end());
		}

		// ------------------------
		// �쐬�������b�V����ǉ�
		// ------------------------

		// OBJ�t�@�C������o�^
		pMesh->filename = fileName;

		// �쐬�������b�V�����Ǘ��p�z��ɒǉ�
		staticMeshList.emplace(pMesh->filename, pMesh);

		// �ǂݍ��݂�ʒm
		LOG("%s��ǂݍ��݂܂���(���_�� = %d, �C���f�b�N�X�� = %d)",
			fileName, vertices.size(), indices.size());

		// �쐬�������b�V����Ԃ�
		return pMesh;
	}

	/// <summary>
	/// ���_�f�[�^�̒ǉ�
	/// </summary>
	/// <param name="[in] vertices"> ���_�f�[�^ </param>
	/// <param name="[in] vertexBytes"> ���_�f�[�^�̃o�C�g�� </param>
	/// <param name="[in] indices"> �C���f�b�N�X�f�[�^ </param>
	/// <param name="[in] indexBytes"> �C���f�b�N�X�f�[�^�̃o�C�g�� </param>
	/// <param name="[in] mode"> �v���~�e�B�u�̎��(�f�t�H���g�ŎO�p�`) </param>
	void MeshBuffer::AddVertexData(const Vertex* vertices, size_t vertexBytes,
		const uint16_t* indices, size_t indexBytes, GLenum mode)
	{
		// �󂫗e�ʂ�����Ă��邱�Ƃ��m�F
		// �K�v�ȃo�C�g�����󂫃o�C�g�����傫���ꍇ�͒ǉ��ł��Ȃ�

		// �g�p����e��(�o�C�g)
		// (�ǉ����钸�_�f�[�^�ƃC���f�b�N�X�f�[�^�̍��v�T�C�Y)
		const size_t totalBytes = vertexBytes + indexBytes;

		// �g�p�ł���e��(�o�C�g)
		const size_t freeBytes = buffer->GetSize() - usedBytes;

		// �g�p����e�ʂ��g�p�ł���e�ʂ𒴂�����
		if (totalBytes > freeBytes)
		{
			// �e�ʕs���̃G���[
			LOG_ERROR("�e�ʂ�����܂���(�v���T�C�Y = %d, �g�p��/�ő�e�� = %d/%d, �󂫗e�� = %d)",
				totalBytes, usedBytes, buffer->GetSize(), freeBytes);

			// �֐����I��
			return;
		}

		/* glCopyNamedBufferSubData�֐���
		�}�`�f�[�^���Ƃ̒��_�f�[�^�ƃC���f�b�N�X�f�[�^��GPU�ɃR�s�[

		glCopyNamedBufferSubData�֐���
		GPU�������ɂ���o�b�t�@�̊Ԃł����R�s�[�ł��Ȃ��̂ŁA
		glNamedBufferStorage�֐���CPU����������GPU�������Ƀf�[�^���R�s�[����K�v������

		glNamedBufferStorage�֐���
		�V����GPU�������̈���m�ۂ���̂ŁA�����̃o�b�t�@�ɏ㏑���ł��Ȃ�
		(�㏑��������@�͂��邪�A�`�摬�x���ቺ���Ă��܂�)
		���̂��߁A�o�b�t�@�ԂŃf�[�^���R�s�[����K�v������ */

		// ���_�f�[�^��GPU�������ɃR�s�[
		GLuint tmp[2];

		// �R�s�[����f�[�^������ꎞ�I��GPU���o�b�t�@���쐬
		// ���_�f�[�^�ƃC���f�b�N�X�f�[�^���R�s�[����̂�2���
		glCreateBuffers(2, tmp);

		// �쐬�����ꎞ�I��GPU���o�b�t�@�ɁACPU���ɂ���f�[�^���R�s�[
		glNamedBufferStorage(tmp[0], vertexBytes, vertices, 0);
		glNamedBufferStorage(tmp[1], indexBytes, indices, 0);

		/* void glCopyNamedBufferSubData(�R�s�[���o�b�t�@�̊Ǘ��ԍ�,
		�R�s�[��o�b�t�@�̊Ǘ��ԍ�, �R�s�[���̓ǂݎ��J�n�ʒu,
		�R�s�[��̏������݊J�n�ʒu, �R�s�[����o�C�g��) */

		// �ꎞ�I�ȃo�b�t�@����A�����̃o�b�t�@�ɃR�s�[
		// ���ɒǉ����Ă��������̂ŁA�������݊J�n�ʒu���T�C�Y�����炷
		// ����͓����o�b�t�@�ɃR�s�[����̂ŁA
		// ���_�f�[�^���R�s�[������A���_�f�[�^�̃T�C�Y�����炵�Ă���
		// �C���f�b�N�X�f�[�^���R�s�[
		glCopyNamedBufferSubData(tmp[0], *buffer, 0, usedBytes, vertexBytes);
		glCopyNamedBufferSubData(tmp[1], *buffer, 0, usedBytes + vertexBytes, indexBytes);

		// �ꎞ�I�ȃo�b�t�@���폜
		glDeleteBuffers(2, tmp);

		// �ǉ������}�`�̕`��p�����[�^���쐬
		DrawParameter newParams;

		// �v���~�e�B�u�̎��
		newParams.mode = mode;

		// �`�悷��C���f�b�N�X��
		newParams.count = static_cast<GLsizei>(indexBytes / sizeof(uint16_t));

		// �C���f�b�N�X�f�[�^�̈ʒu(�o�C�g���Ŏw��)��
		// �R�s�[�������l������̂ŁA���_�f�[�^�̒���
		newParams.indices = reinterpret_cast<void*>(usedBytes + vertexBytes);

		// ���_�f�[�^�̈ʒu�͒��_�f�[�^���Ŏw�肷��
		newParams.baseVertex = static_cast<GLuint>(usedBytes / sizeof(Vertex));

		// �쐬�����`��p�����[�^��z��ɒǉ�
		drawParamList.push_back(newParams);

		// ���̃f�[�^�i�[�J�n�ʒu���v�Z
		// �R�s�[��̈ʒu���A���_�f�[�^�ƃC���f�b�N�X�f�[�^�̋��ʂ̋��E�ō��킹��
		// ���ʂ̋��E�ɍ��킹�邱�ƂŁA�ǂ�������������E�Ƀf�[�^���R�s�[�ł���

		// �C���f�b�N�X�f�[�^��uint16_t�^�Ȃ̂ŁA���E�T�C�Y��2�o�C�g
		// ���_�f�[�^��Vertex�\���̂Ȃ̂ŁA���E�T�C�Y��20�o�C�g
		// ���ʂ̋��E�́u�ŏ����{���v�Ɉ�v����̂�20�o�C�g�ɂȂ�

		// ���ʂ̋��E�T�C�Y(�ŏ����{��)
		constexpr size_t a = std::lcm(sizeof(uint16_t), sizeof(Vertex));

		// �g���������g�p�ςݗe�ʂɉ�����
		// 
		// ���E�T�C�Y��������̂ŁA
		// ���E�T�C�Y�ɍ��킹�����������߂�
		// ((���̒l + ���E�T�C�Y - 1) / ���E�T�C�Y) * ���E�T�C�Y
		// 
		// �u�����^�̏��Z�͂��܂��؂�̂Ă�v�Ƃ��������𗘗p���āA
		// �u���̒l�ȏォ�A�ŏ��̋��E�T�C�Y�̔{���v�����߂Ă���
		//
		// ��ɋ��E�T�C�Y�ɍ��킹���l�����Z���邱�Ƃ�
		// �uusedBytes����ɋ��E�T�C�Y�ƈ�v����v���Ƃ��ۏ؏o����
		usedBytes += ((totalBytes + a - 1) / a) * a;
	}

	/// <summary>
	/// �S�Ă̒��_�f�[�^���폜
	/// </summary>
	void MeshBuffer::Clear()
	{
		// usedBytes��0�ɂ��邱�ƂŁA���Ƀf�[�^��GPU�������ɃR�s�[����Ƃ��A
		// �o�b�t�@�I�u�W�F�N�g�̐擪����R�s�[���s����
		// �Ȃ̂ŁAGPU�������ɃR�s�[�������_�f�[�^���폜����K�v�͂Ȃ�
		usedBytes = 0;

		// �X�^�e�B�b�N���b�V���̍폜
		staticMeshList.clear();

		// �`��p�����[�^���폜
		drawParamList.clear();
	}

	/// <summary>
	/// �X�^�e�B�b�N���b�V�����擾����
	/// </summary>
	/// <param name="[in] fileName"> �t�@�C���� </param>
	/// <returns> �t�@�C��������v����X�^�e�B�b�N���b�V�� </returns>
	StaticMeshPtr MeshBuffer::GetStaticMesh(const char* fileName)
	{
		// �X�^�e�B�b�N���b�V�����o�^����Ă���Ȃ猟��
		if (!staticMeshList.empty())
		{
			// ��������
			auto itr = staticMeshList.find(fileName);

			// �v�f����������
			if (itr != staticMeshList.end())
			{
				// �X�^�e�B�b�N���b�V��(�l)��Ԃ�
				return itr->second;
			}
		}

		// ������Ȃ��ꍇ
		// ���������ɓǂݍ��߂Ă��Ȃ��\��������̂ōēǂݍ���
		StaticMeshPtr obj = LoadOBJ(fileName);

		// �ǂݍ��񂾃X�^�e�B�b�N���b�V����Ԃ�
		return obj;
	}

} // namespace PokarinEngine
