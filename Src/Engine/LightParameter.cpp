/**
* @file LightData.cpp
*/
#include "LightParameter.h"

#include "Settings/ShaderSettings.h"
#include "Shader/Shader.h"

#include <algorithm>
#include <set>

namespace PokarinEngine
{
	/// <summary>
	/// ���C�g�̃p�����[�^�Ǘ��p
	/// </summary>
	namespace LightParameter
	{
		// ---------------------------
		// �^�̕ʖ����`
		// ---------------------------

		using LightDataList = std::set<LightData*>;

		// �����ɂȂ�悤��set�^���g��
		using LightIndexList = std::set<int>;

		// ----------------------------
		// �ϐ�
		// ----------------------------

		// ���C�g�f�[�^�z��
		LightDataList lightList;

		// ���s����
		LightData* directionalLight = nullptr;

		// ����(����C���[�W)
		// �����������Ȃ��̂ŐF����
		Color ambientLight = { 0.05f, 0.15f, 0.25f, 1.0f };

		// -----------------------------
		// �֐�
		// -----------------------------

		/// <summary>
		/// ���C�g����GPU�ɃR�s�[����
		/// </summary>
		/// <param name="mainCamera"> �J���� </param>
		void CopyGPU(const GameObject& mainCamera)
		{
			// ------------------------------------
			// ������GPU�������[�ɃR�s�[
			// ------------------------------------

			// �V�F�[�_�v���O����(�W��)�̊Ǘ��ԍ�
			const GLuint progStandard = Shader::GetProgram(Shader::ProgType::Standard);

			// -------------------------
			// �g�p�����C�g���m�F
			// -------------------------

			// �g�p���̃��C�g���Ȃ���΃R�s�[���郉�C�g����0�ɐݒ�
			if (lightList.empty())
			{
				glProgramUniform1i(progStandard, UniformLocation::lightCount, 0);

				return;
			}

			// -------------------------------
			// �J�����̐��ʃx�N�g�����v�Z
			// -------------------------------

			// �J�����̐��ʃx�N�g��
			const Vector3 front = {
				-sin(mainCamera.transform->rotation.y),
				0,
				-cos(mainCamera.transform->rotation.y) };

			// -----------------------------------------
			// �J�������烉�C�g�܂ł̋������v�Z
			// -----------------------------------------

			// -------------- ������ ----------------

			// �J�������烉�C�g�܂ł̋������Ǘ�����\����
			struct Distance
			{
				// �J�������烉�C�g�܂ł̋���
				float distance = 0;

				// ���C�g�f�[�^
				const LightData* lightData = nullptr;
			};

			// �J�������烉�C�g�܂ł̋������i�[����z��
			std::vector<Distance> distanceList;

			// ���C�g�̐������e�ʂ��m�ۂ���
			distanceList.reserve(lightList.size());

			// ----------- ���C�g�܂ł̋�����z��Ɋi�[���� ------------

			// �g�p�����C�g�̗v�f�ԍ�
			for (auto& light : lightList)
			{
				// ���s�����͋����֌W�Ȃ��g���̂ŕʂ̕ϐ��ŕێ�
				if (light->type == Type::directional)
				{
					directionalLight = light;
					continue;
				}

				// �J�������烉�C�g�܂ł̃x�N�g��
				const Vector3 v = light->position - mainCamera.transform->position;

				// �J�����̌��ŁA���C�g�͈̔͊O�������ꍇ
				if (front.Dot(v) <= -light->range)
				{
					// �v�Z�̕K�v���Ȃ��̂�
					// ���̃��C�g��
					continue;
				}

				// �J��������̃��C�g�̔��a�܂ł̋���
				const float distance = v.Length() - light->range;

				// �z��ɒǉ�
				distanceList.push_back({ distance, light });

			} // for indexs

			// ------------ ���s�����̃f�[�^��GPU�ɃR�s�[ -------------

			if (directionalLight)
			{
				// �F
				const Color color = directionalLight->color * directionalLight->intensity;

				// GPU�ɃR�s�[
				glProgramUniform3fv(progStandard,
					UniformLocation::directionalLightColor, 1, &color.r);

				glProgramUniform3fv(progStandard,
					UniformLocation::directionalLightDirection, 1, &directionalLight->direction.x);
			}


			glProgramUniform3fv(progStandard,
				UniformLocation::ambientLight, 1, &ambientLight.r);

			// ----------- ��ʂɉe�����郉�C�g���Ȃ���� -------------
			// ----------- ���C�g����0�ɐݒ肷��		  -------------

			// ��ʓ��Ƀ��C�g���Ȃ�
			if (distanceList.empty())
			{
				glProgramUniform1i(progStandard, UniformLocation::lightCount, 0);
				return;
			}

			// -------------------------------------
			// �J�����ɋ߂����C�g��D�悷��
			// -------------------------------------

			// �J��������̋������Z�����ɂ���
			std::stable_sort(distanceList.begin(), distanceList.end(),
				[](const auto& a, const auto& b) { return a.distance < b.distance; });

			// -------------------------------------
			// ���C�g�f�[�^��GPU�������ɃR�s�[
			// -------------------------------------

			// ------------ ������ ---------------

			// �g�p���郉�C�g�̐�
			const int lightCount = static_cast<int>(
				std::min(distanceList.size(), MaxShaderCount::light));

			// ���C�g�̐F�ƌ����J�n�p�x
			std::vector<Vector4> colorAndFalloffAngle(lightCount);

			// ���C�g�̍��W�Ɣ͈�(���a)
			std::vector<Vector4> positionAndRange(lightCount);

			// ���C�g�����ƍő�Ǝˊp�x
			std::vector<Vector4> directionAndSpotAngle(lightCount);

			// -------- GPU�ɃR�s�[���郉�C�g�f�[�^��ݒ肷�� ---------

			for (int i = 0; i < lightCount; ++i)
			{
				// ���C�g
				const LightData* lightData = distanceList[i].lightData;

				// ���邳�𔽉f�����F
				const Color color = lightData->color * lightData->intensity;

				// �F�ƌ����J�n�p�x��ݒ�
				colorAndFalloffAngle[i] = {
					color.r, color.g, color.b, lightData->falloffAngle };

				// ���W�Ɣ͈�(���a)��ݒ�
				positionAndRange[i] = {
					lightData->position.x, lightData->position.y, lightData->position.z, lightData->range };

				// ���C�g�̕����ƍő�Ǝˊp�x��ݒ�
				directionAndSpotAngle[i] = {
					lightData->direction.x, lightData->direction.y, lightData->direction.z, lightData->spotAngle };
			}

			// ----------- GPU�Ƀ��C�g�f�[�^���R�s�[ ------------

			// �F�ƌ����J�n�p�x
			glProgramUniform4fv(progStandard, UniformLocation::lightColorAndFalloffAngle,
				lightCount, &colorAndFalloffAngle[0].x);

			// ���W�Ɣ͈�(���a)
			glProgramUniform4fv(progStandard, UniformLocation::lightPositionAndRange,
				lightCount, &positionAndRange[0].x);

			// �����ƍő�Ǝˊp�x
			glProgramUniform4fv(progStandard, UniformLocation::lightDirectionAndSpotAngle,
				lightCount, &directionAndSpotAngle[0].x);

			// �g�p���郉�C�g�̐�
			glProgramUniform1i(progStandard, UniformLocation::lightCount, lightCount);
		}

		/// <summary>
		/// ���C�g�f�[�^��ǉ�����
		/// </summary>
		/// <param name="lightData"> �ǉ����郉�C�g�f�[�^ </param>
		void AddLightData(LightData& lightData)
		{
			lightList.emplace(&lightData);
		}

		/// <summary>
		/// ���C�g�f�[�^���폜����
		/// </summary>
		/// <param name="lightData"> �폜���郉�C�g�f�[�^ </param>
		void EraseLightData(LightData& lightData)
		{
			lightList.erase(&lightData);
		}

	} // namespace LightData

} // namespace PokarinEngine