/**
* @file Collider.cpp
*/
#include "Collider.h"

#include "../../GameObject.h"
#include "../../Color.h"
#include "../../Shader/Shader.h"

#include "../../Mesh/Mesh.h"

#include "../../Configs/MeshConfig.h"
#include "../../Configs/ShaderConfig.h"

namespace PokarinEngine
{
	/// <summary>
	/// �Q�[���I�u�W�F�N�g�ɒǉ����ꂽ���̏�����
	/// </summary>
	void Collider::Awake()
	{
		// �R���C�_�[�̎��
		const Type type = GetType();

		// ��ނɍ��킹�����b�V�����擾����
		switch (type)
		{
		case Type::Box:

			staticMesh = Mesh::GetStaticMesh(StaticMeshFile::boxCollider);

			break;

		case Type::Sphere:

			staticMesh = Mesh::GetStaticMesh(StaticMeshFile::sphereCollider);

			break;
		}
	}

	/// <summary>
	/// �X�V
	/// </summary>
	void Collider::Update()
	{
		// ������ł���Q�[���I�u�W�F�N�g��
		// ���������p�R���|�[�l���g�������Ă�����Փˎ��ɓ����悤�ɂ���
		isStatic = !GetOwnerObject().HasRigidbody();
	}

	/// <summary>
	/// �`��
	/// </summary>
	void Collider::Draw()
	{
		// ���C�e�B���O�����V�F�[�_�̎��ʔԍ�
		GLuint progUnlit = Shader::GetProgram(Shader::ProgType::Unlit);

		// �`��Ɏg���V�F�[�_���w��
		glUseProgram(progUnlit);

		// �F��GPU�ɃR�s�[
		// �ΐF�ŕ`�悷��
		glProgramUniform4fv(progUnlit,
			UniformLocation::color, 1, &Color::green.r);

		// ���W�ϊ��s��
		const Matrix4x4 transformMatrix = GetTransformMatrix();

		// ���W�ϊ��s���GPU�ɃR�s�[
		glProgramUniformMatrix4fv(
			progUnlit, UniformLocation::transformMatrix,
			1, GL_FALSE, &transformMatrix[0].x);

		// ���L�}�e���A�����g����
		// �X�^�e�B�b�N���b�V����`��
		Mesh::Draw(staticMesh, progUnlit, staticMesh->GetMaterialList());
	}
}