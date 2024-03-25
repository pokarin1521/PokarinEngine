/**
* @file LightData.cpp
*/
#include "LightManager.h"

#include "Settings/ShaderSettings.h"

#include <algorithm>

namespace PokarinEngine
{
	namespace LightManager
	{
		// ------------------
		// �ϐ�
		// ------------------

		// ���C�g�f�[�^�z��
		LightDataList lights;

		// �g�p�����C�g�̃C���f�b�N�X�z��
		std::vector<int> usedLights;

		// ���g�p���C�g�̃C���f�b�N�X�z��
		std::vector<int> freeLights;

		// ��x�ɑ��₷���C�g��
		static constexpr size_t lightResizeCount = 100;

		// ---------------
		// �֐�
		// ---------------

		/// <summary>
		/// ���C�g�z���������
		/// </summary>
		void InitializeLight()
		{
			// -----------------------------------------------
			// �w�肳�ꂽ���̃��C�g���̗e�ʁE�v�f�����m��
			// -----------------------------------------------

			// ���C�g�f�[�^�̗v�f�����m��
			lights.resize(lightResizeCount);

			// �g�p���̃��C�g�̗e�ʂ��m��
			usedLights.reserve(lightResizeCount);

			// --------------------------------------
			// �S�Ẵ��C�g�𖢎g�p���C�g�z���
			//�u�t���v�Œǉ�
			// --------------------------------------

			/* ���C�g���u�t���v�Œǉ�����̂́A
			�V�������C�g���擾����Ƃ��A
			���C�g�z��̖�������擾�����邽�� */

			// ���g�p���C�g�̗v�f�����m��
			freeLights.resize(lightResizeCount);

			// �t���Ɋi�[(������)���Ă���
			for (int i = 0; i < lightResizeCount; ++i)
			{
				freeLights[i] = static_cast<int>(lightResizeCount - i - 1);
			}
		}

		/// <summary>
		/// ���C�g���̍X�V
		/// �J�����ɋ߂����C�g��I���GPU�������ɃR�s�[����
		/// </summary>
		/// <param name="progStandard"> �V�F�[�_�v���O����(�W��)�̊Ǘ��ԍ� </param>
		/// <param name="camera"> �J���� </param>
		/// <param name="lights"> ���C�g�f�[�^�z�� </param>
		/// <param name="usedLights"> �g�p�����C�g </param>
		void Update(GLuint progStandard, const GameObject& mainCamera)
		{
			// -------------------------
			// �g�p�����C�g���m�F
			// -------------------------

			// �g�p���̃��C�g���Ȃ���΃R�s�[���郉�C�g����0�ɐݒ�
			if (usedLights.empty())
			{
				glProgramUniform1i(progStandard, LocationNum::lightCount, 0);

				return;
			}

			// ------------------------------------
			// �g�p�����C�g�̔z�񂩂�A
			// ���g�p�ɂȂ������C�g�����O����
			// ------------------------------------

			// ----------- �g�p���łȂ����C�g��   ------------
			// ----------- �z��̌��ɏW�߂č폜 ------------

			// �g�p���łȂ����C�g�̐擪���擾
			const auto itrUnused = std::remove_if(
				usedLights.begin(), usedLights.end(),
				[&](int i) { return !lights[i].used; });

			// �g�p���łȂ����C�g���폜
			usedLights.erase(itrUnused, usedLights.end());

			// ----------- �d������ԍ������� -------------

			// �g�p�����C�g�̃C���f�b�N�X�𐮗�
			std::sort(usedLights.begin(), usedLights.end());

			// unique�֐��ŏd������ԍ������ɏW�߁A
			// erase�֐��ō폜����
			auto itr = std::unique(usedLights.begin(), usedLights.end());
			usedLights.erase(itr, usedLights.end());

			// -------------------------------
			// �J�����̐��ʃx�N�g�����v�Z
			// -------------------------------

			// �J�����̐��ʃx�N�g��
			const Vec3 front = {
				-sin(mainCamera.transform->rotation.y),
				0,
				-cos(mainCamera.transform->rotation.y) };

			// -------------------------------------
			// �J�������烉�C�g�܂ł̋������v�Z
			// -------------------------------------

			// -------------- ������ ----------------

			// �J�������烉�C�g�܂ł̋���
			struct Distance
			{
				// �J�������烉�C�g�̔��a�܂ł̋���
				float distance;

				// ���C�g�̃A�h���X
				const LightData* p;
			};

			// ���C�g�܂ł̋������i�[����z��
			std::vector<Distance> distanceList;

			// ���C�g�̐������e�ʂ��m��
			distanceList.reserve(lights.size());

			// ----------- ���C�g�܂ł̋�����z��Ɋi�[���� ------------

			// �g�p�����C�g�̃C���f�b�N�X
			for (auto index : usedLights)
			{
				// �g�p���̃��C�g
				const auto& light = lights[index];

				// �J�������烉�C�g�܂ł̃x�N�g��
				const Vec3 v = light.position - mainCamera.transform->position;

				// �J�����̌��ŁA���C�g�͈̔͊O�������ꍇ
				if (Vec3_Function::Dot(front, v) <= -light.radius)
				{
					// �v�Z�̕K�v���Ȃ��̂�
					// ���̃��C�g��
					continue;
				}

				// �J��������̃��C�g�̔��a�܂ł̋���
				const float d = Vec3_Function::Length(v) - light.radius;

				// �z��ɒǉ�
				distanceList.push_back({ d, &light });

			} // for indexs

			// ----------- ��ʂɉe�����郉�C�g���Ȃ���� -------------
			// ----------- ���C�g����0�ɐݒ肷��		  -------------

			// ��ʓ��Ƀ��C�g���Ȃ�
			if (distanceList.empty())
			{
				glProgramUniform1i(progStandard, LocationNum::lightCount, 0);
				return;
			}

			// -----------------------------------
			// �J�����ɋ߂����C�g��D�悷��
			// -----------------------------------

			// �������߂����ɕ��ׂ�
			std::stable_sort(distanceList.begin(), distanceList.end(),
				[&](const auto& a, const auto& b) {
					return a.distance < b.distance; });

			// -------------------------------------
			// ���C�g�f�[�^��GPU�������ɃR�s�[
			// -------------------------------------

			// ------------ ������ ---------------

			// �g�p���郉�C�g�̐�
			const int lightCount = static_cast<int>(
				std::min(distanceList.size(), MaxShaderCount::light));

			// ���C�g�̐F�ƌ����J�n�p�x
			std::vector<Vec4> colorAndFalloffAngle(lightCount);

			// ���C�g�̍��W�Ɣ��a
			std::vector<Vec4> positionAndRadius(lightCount);

			// ���C�g�����ƍő�Ǝˊp�x
			std::vector<Vec4> directionAndConeAngle(lightCount);

			// -------- GPU�ɃR�s�[���郉�C�g�f�[�^��ݒ肷�� ---------

			for (int i = 0; i < lightCount; ++i)
			{
				// ���C�g
				const LightData* p = distanceList[i].p;

				// ���邳�𔽉f�����F
				const Vec3 color = p->color * p->intensity;

				// �F�ƌ����J�n�p�x��ݒ�
				colorAndFalloffAngle[i] = {
					color.x, color.y, color.z, p->falloffAngle };

				// ���W�Ɣ��a��ݒ�
				positionAndRadius[i] = {
					p->position.x, p->position.y, p->position.z, p->radius };

				// ���C�g�̕����ƍő�Ǝˊp�x��ݒ�
				directionAndConeAngle[i] = {
					p->directoin.x, p->directoin.y, p->directoin.z, p->coneAngle };
			}

			// ----------- GPU�Ƀ��C�g�f�[�^���R�s�[ ------------

			// �F�ƌ����J�n�p�x
			glProgramUniform4fv(progStandard, LocationNum::lightColorAndFalloffAngle,
				lightCount, &colorAndFalloffAngle[0].x);

			// ���W�Ɣ��a
			glProgramUniform4fv(progStandard, LocationNum::lightPositionAndRadius,
				lightCount, &positionAndRadius[0].x);

			// �����ƍő�Ǝˊp�x
			glProgramUniform4fv(progStandard, LocationNum::lightDirectionAndConeAngle,
				lightCount, &directionAndConeAngle[0].x);

			// �g�p���郉�C�g�̐�
			glProgramUniform1i(progStandard, LocationNum::lightCount, lightCount);
		}

		/// <summary>
		/// �V�������C�g���擾����
		/// </summary>
		/// <returns> ���C�g�̃C���f�b�N�X </returns>
		int AllocateLight()
		{
			// --------------------------------
			// ���g�p�̃��C�g���Ȃ����
			// ���C�g�z����g������
			// --------------------------------

			// ���g�p�̃��C�g���Ȃ�
			if (freeLights.empty())
			{
				// ���̃T�C�Y
				const size_t oldSize = lights.size();

				// �ǉ����镪�A�v�f���𑝂₷
				lights.resize(oldSize + lightResizeCount);

				// �g���������C�g�𖢎g�p���C�g�z��Ɂu�t���v�Œǉ�
				for (size_t i = lights.size() - 1; i >= oldSize; --i)
				{
					freeLights.push_back(static_cast<int>(i));
				}
			}

			// ----------------------------------
			// ���g�p���C�g�z��̖�������
			// �C���f�b�N�X�����o��
			// ----------------------------------

			// ���ꂩ��g�p���郉�C�g�̃C���f�b�N�X
			const int index = freeLights.back();

			// ���o�������A�폜
			freeLights.pop_back();

			// ���o�����C���f�b�N�X���g�p�����C�g�z��ɒǉ�
			usedLights.push_back(index);

			// ���C�g�̏�Ԃ��u�g�p���v�ɂ���
			lights[index].used = true;

			// ���o�����C���f�b�N�X��Ԃ�
			return index;
		}

		/// <summary>
		/// ���C�g���������
		/// </summary>
		/// <param name="index"> ���C�g�̃C���f�b�N�X </param>
		void DeallocateLight(int index)
		{
			// �L���ȃC���f�b�N�X�ł���Ή������
			if (index >= 0 && index < lights.size())
			{
				// �C���f�b�N�X�𖢎g�p���C�g�z��ɒǉ�
				freeLights.push_back(index);

				// ���C�g�̏�Ԃ��u���g�p�v�ɂ���
				lights[index].used = false;
			}
		}

		/// <summary>
		/// �C���f�b�N�X�ɑΉ����郉�C�g�f�[�^���擾����
		/// </summary>
		/// <param name="index"> ���C�g�̃C���f�b�N�X </param>
		/// <returns>
		/// <para> nullptr�ȊO : index�ɑΉ����郉�C�g�f�[�^�̃A�h���X </para>
		/// <para> nullptr : �Ή����郉�C�g�f�[�^�����݂��Ȃ� </para>
		/// </returns>
		LightData* GetLight(int index)
		{
			// �L���ȃC���f�b�N�X�ŁA�g�p���̃��C�g
			if (index >= 0 && index < lights.size() && lights[index].used)
			{
				// ���C�g�f�[�^��Ԃ�
				return &lights[index];
			}

			// �����ȃC���f�b�N�X�A�܂��͖��g�p�̃��C�g
			return nullptr;
		}

	} // namespace LightData

} // namespace PokarinEngine