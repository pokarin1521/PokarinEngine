/**
* @file ShaderConfig.h
*/
#ifndef POKARINENGINE_SHADERCONFIG_H_INCLUDED
#define POKARINENGINE_SHADERCONFIG_H_INCLUDED

#include "glad/glad.h"

namespace PokarinEngine
{
	/// <summary>
	/// �V�F�[�_�Ŏg�����
	/// </summary>
	namespace MaxShaderCount
	{
		// ���C�g�̐�
		inline constexpr size_t light = 16;
	}

	/// <summary>
	/// �e�N�X�`���C���[�W���j�b�g�̔ԍ�
	/// </summary>
	namespace TextureBinding
	{
		// �ʏ�̐F�p
		inline constexpr GLuint color = 0;

		// �����F�p
		inline constexpr GLuint emission = 1;
	}

	/// <summary>
	/// �V�F�[�_�̃��j�t�H�[���ϐ��̃��P�[�V�����ԍ�
	/// </summary>
	namespace UniformLocation
	{
		// --------------------------------
		// ���W�A��]�p�x�A�g�嗦
		// --------------------------------

		// ���W�ϊ��x�N�g���̍s��
		inline constexpr GLint transformMatrix = 0;

		// �@���ϊ��x�N�g���̍s��
		inline constexpr GLint normalMatrix = 1;

		// �A�X�y�N�g��Ǝ���p�ɂ��g�嗦
		inline constexpr GLint aspectRatioAndScaleFov = 3;

		// �J�����̍��W
		inline constexpr GLint cameraPosition = 4;

		// �J�����̉�]�p�x
		inline constexpr GLint cameraRotation = 5;

		// -------------------------
		// �I�u�W�F�N�g�F
		// -------------------------

		// �I�u�W�F�N�g�̐F
		inline constexpr GLint color = 100;

		// �I�u�W�F�N�g�̔����F
		inline constexpr GLint emissionColor = 101;

		// -------------------
		// ���C�g
		// -------------------

		// ����
		// �����������Ȃ��̂ŐF����
		inline constexpr GLint ambientLight = 107;

		// ���s�����̐F
		inline constexpr GLint directionalLightColor = 108;

		// ���s�����̌���
		inline constexpr GLint directionalLightDirection = 109;

		// �g�p���郉�C�g�̐�
		inline constexpr GLint lightCount = 110;

		// ���C�g�̐F
		// �z��̃C���f�b�N�X0��
		inline constexpr GLint lightColorAndFalloffAngle = 111;

		/* ���C�g�\���̗̂v�f�͔z��Ȃ̂�
		�z��̐������P�[�V�����ԍ������炷�K�v������ */

		// ���C�g�̍��W�Ɣ͈͂̔��a
		// �z��̃C���f�b�N�X0��
		inline constexpr GLint lightPositionAndRange =
			lightColorAndFalloffAngle + MaxShaderCount::light;

		// ���C�g
		inline constexpr GLint lightDirectionAndSpotAngle =
			lightColorAndFalloffAngle + MaxShaderCount::light * 2;

	} // UniformLocation

} // namespace PokarinEngine

#endif // !POKARINENGINE_SHADERCONFIG_H_INCLUDED
