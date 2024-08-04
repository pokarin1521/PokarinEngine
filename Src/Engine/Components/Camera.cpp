/**
* @file Camera.cpp
*/
#include "Camera.h"

#include "ImGui/imgui.h"

#include "../GameObject.h"
#include "../Window.h"
#include "../TextureGetter.h"
#include "../Shader/Shader.h"
#include "../Configs/ShaderConfig.h"

#include "../Mesh/Mesh.h"

namespace PokarinEngine
{
	/// <summary>
	/// �X�V
	/// </summary>
	void Camera::Update()
	{
		// ������ł���Q�[���I�u�W�F�N�g�̈ʒu�E��]�p�x
		TransformPtr owner = GetOwnerObject().transform;

		// �ʒu�E��]�p�x���X�V����
		owner->position = transform.position;
		owner->rotation = transform.rotation;
	}

	/// <summary>
	/// GPU�ɏ����R�s�[����
	/// </summary>
	void Camera::CopyToGPU()
	{
		// ----------------------------
		// �����擾����
		// ----------------------------
		
		// �J�����̈ʒu
		Vector3 position = transform.position;

		// ���͍�����W�n�̒l�ɂȂ��Ă��āA
		// OpenGL�͉E����W�n�Ȃ̂ŁA�E����W�n�ɂ���
		position.z *= -1;

		// �J�����̉�]�p�x
		// �I�u�W�F�N�g�̓J�����̉�]�����Ƃ͋t�ɓ������ƂɂȂ�̂ŁA�������t�ɂ���
		Vector3 rotation = -transform.rotation;

		// ----------------------------
		// GPU�ɃR�s�[����
		// ----------------------------

		// �S�ẴV�F�[�_�v���O����
		const auto& allProg = Shader::GetAllProgram();

		// �A�X�y�N�g��
		const float aspectRatio = Window::GetAspectRatio(WindowID::Main);

		// �S�ẴV�F�[�_�v���O�����ɃR�s�[����
		for (const auto& [type, prog] : allProg)
		{
			// �A�X�y�N�g��Ǝ���p�ɂ��g�嗦��ݒ�
			// GPU���ł̏��Z������邽�߁A�t���ɂ��ēn��
			glProgramUniform2f(prog, UniformLocation::aspectRatioAndScaleFov,
				1 / aspectRatio, inverseFovScale);

			// �J�����̈ʒu��GPU�ɃR�s�[
			glProgramUniform3fv(prog, UniformLocation::cameraPosition, 1, &position.x);

			// �J�����̉�]�p�x��GPU�ɃR�s�[
			glProgramUniform3fv(prog, UniformLocation::cameraRotation, 1, &rotation.x);
		}
	}

	/// <summary>
	/// �X�J�C�X�t�B�A��`�悷��
	/// </summary>
	void Camera::DrawSkySphere()
	{
		// --------------------------------------------------------
		// �X�J�C�X�t�B�A�p���f�����Ȃ��ꍇ�͕`�悵�Ȃ�
		// --------------------------------------------------------

		if (!skySphere)
		{
			return;
		}

		// -------------------------------------
		// �g�p����V�F�[�_���w�肷��
		// -------------------------------------

		// ���C�e�B���O�����̃V�F�[�_
		static const GLuint progUnlit = Shader::GetProgram(Shader::ProgType::Unlit);

		// ��Ƀ��C�e�B���O����ƕs���R�Ȃ̂�
		// �A�����b�g�V�F�[�_�ŕ`��
		glUseProgram(progUnlit);

		// �[�x�o�b�t�@�ւ̏������݂��֎~
		glDepthMask(GL_FALSE);

		// ----------------------------------------
		// ���W�ϊ��s���GPU�ɃR�s�[����
		// ----------------------------------------

		/* �X�J�C�X�t�B�A�͈ړ��Ɖ�]�͂��Ȃ��̂Ŋg�嗦������ݒ肷��

		�X�J�C�X�t�B�A�͍ł������ɕ`�悳��镨�̂Ȃ̂ŁA
		�J�������`��ł���͈͂̂����A�ł��邾�������ʒu�ɕ`�悵����

		���݂̍ő�`��͈͂��g��������
		�X�J�C�X�t�B�A���|���S�����f���Ȃ̂Ŕ����ȉ��ʂ�����
		���ʂ��͈͂���͂ݏo���Ȃ��悤��*/

		// �X�J�C�X�t�B�A�̔��a
		static const float skySphereRadius = 0.5f;

		// �g�嗦
		// �ő�`��͈͂�95%�̈ʒu�ɕ`��ł���悤�ɐݒ�
		const float scale = drawRange.far * 0.95f / skySphereRadius;

		// ���W�ϊ��s��
		// �ړ�����]�����Ȃ��̂Ŋg�嗦����
		const Matrix4x4 transformMatrix = {
			{ scale,     0,     0,     0 },
			{     0, scale,     0,     0 },
			{     0,     0, scale,     0 },
			{     0,     0,     0,     1 },
		};

		// ���W�ϊ��s���GPU�ɃR�s�[
		glProgramUniformMatrix4fv(progUnlit, UniformLocation::transformMatrix,
			1, GL_FALSE, &transformMatrix[0].x);

		// -----------------------------------
		// �F��GPU�������ɃR�s�[
		// -----------------------------------

		// �F�̓}�e���A���J���[�Œ�������̂Ŕ���ݒ�
		// (���ۂɕ`�悳���F�́u�I�u�W�F�N�g�J���[�v�Ɓu�}�e���A���J���[�v�̏�Z)
		static const Color color = { 1, 1, 1, 1 };
		glProgramUniform4fv(progUnlit, UniformLocation::color, 1, &color.r);

		// -----------------------------------
		// �J�����̍��W��GPU�ɃR�s�[
		// -----------------------------------

		// �X�J�C�X�t�B�A�͏�ɃJ�����𒆐S�ɕ`�悵�����̂ŁA
		// �J�������ꎞ�I�Ɍ��_�Ɉړ�������
		glProgramUniform3fv(progUnlit, UniformLocation::cameraPosition,
			1, &Vector3::zero.x);

		// -----------------------------------
		// �X�J�C�X�t�B�A��`�悷��
		// -----------------------------------

		// �X�J�C�X�t�B�A��`�悷��
		Mesh::Draw(skySphere, progUnlit, skySphere->GetMaterialList());

		// �J�����p�����[�^��GPU�ɃR�s�[������
		CopyToGPU();

		// �[�x�o�b�t�@�ւ̏������݂�����
		glDepthMask(GL_TRUE);

		// �W���V�F�[�_�ɖ߂�
		glUseProgram(Shader::GetProgram(Shader::ProgType::Standard));
	}

	/// <summary>
	/// ����ҏW�ł���悤�ɕ\������
	/// </summary>
	void Camera::InfoEditor()
	{
		ImGui::Text("Camera Information");
	}

	/// <summary>
	/// �R���|�[�l���g�̏���Json�^�Ɋi�[����
	/// </summary>
	/// <param name="[out] Json"> �����i�[����Json�^ </param>
	void Camera::ToJson(Json& data) const
	{

	}

	/// <summary>
	/// �R���|�[�l���g�̏���Json�^����擾����
	/// </summary>
	/// <param name="[out] data"> �����i�[���Ă���Json�^ </param>
	void Camera::FromJson(const Json& data)
	{

	}
}