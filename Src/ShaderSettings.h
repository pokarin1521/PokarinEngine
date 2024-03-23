/**
* @file ShaderSettings.h
*/
#ifndef SHADERSETTINGS_H_INCLUDED
#define SHADERSETTINGS_H_INCLUDED

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
	/// �V�F�[�_�̃��P�[�V�����ԍ�
	/// </summary>
	namespace LocationNum
	{
		// ------------------------
		// �I�u�W�F�N�g, �J����
		// ------------------------

		// �I�u�W�F�N�g�̊g�嗦
		inline constexpr GLint scale = 0;

		// �I�u�W�F�N�g�̍��W
		inline constexpr GLint position = 1;

		// �I�u�W�F�N�g��X,Y����]
		inline constexpr GLint sinCosXY = 2;

		// �A�X�y�N�g��Ǝ���p�ɂ��g�嗦
		inline constexpr GLint aspectRatioAndScaleFov = 3;

		// �J�����̍��W
		inline constexpr GLint cameraPosition = 4;

		// �J������Y����]
		inline constexpr GLint cameraRotation = 5;

		// �I�u�W�F�N�g�̐F
		inline constexpr GLint color = 100;

		// �I�u�W�F�N�g�̔����F
		inline constexpr GLint emissionColor = 101;

		// ---------------
		// ���C�g
		// ---------------

		// �g�p���郉�C�g�̐�
		inline constexpr GLint lightCount = 110;

		// ���C�g�̐F
		// �z��̃C���f�b�N�X0��
		inline constexpr GLint lightColorAndFalloffAngle = 111;

		// ���C�g�̍��W�Ɣ͈͂̔��a
		// �z��̃C���f�b�N�X0��
		inline constexpr GLint lightPositionAndRadius =
			lightColorAndFalloffAngle + MaxShaderCount::light;

		// ���C�g
		inline constexpr GLint lightDirectionAndConeAngle =
			lightColorAndFalloffAngle + MaxShaderCount::light * 2;

	} // LocationNum

} // namespace PokarinEngine

#endif // !SHADERSETTINGS_H_INCLUDED
