/**
* @file LightData.h
*/
#ifndef LIGHTDATA_H_INCLUDED
#define LIGHTDATA_H_INCLUDED

#include "VecMath.h"
#include "GameObject.h"

#include <GLFW/glfw3.h>
#include <vector>

namespace PokarinEngine
{
	namespace LightManager
	{
		// ----------------------
		// �O���錾
		// ----------------------

		struct LightData;

		// ---------------------------
		// �^�̕ʖ����`
		// ---------------------------

		// ���C�g�f�[�^�z��
		using LightDataList = std::vector<LightData>;

		// ----------------
		// �\����
		// ----------------

		/// <summary>
		/// ���C�g�̏��
		/// </summary>
		struct LightData
		{
			// ----------------- 
			// ���C�g�S�� 
			// -----------------

			Vec3 color = { 0,0,0 };		// �F
			float intensity = 0;		// ���邳
			Vec3 position = { 0,0,0 };	// �ʒu
			float radius = 0;			// ���C�g���͂��ő唼�a
			bool used = false;			// �g�p�����ǂ���

			// --------------------- 
			// �X�|�b�g���C�g 
			// ---------------------

			// ���C�g�̌���
			Vec3 directoin = { 0,0,0 };

			// �X�|�b�g���C�g�̍ő�Ǝˊp�x
			// 0�ȉ��Ȃ�_�����Ƃ��Ĉ���
			float coneAngle = 0;

			// �X�|�b�g���C�g�̌����J�n�p�x
			float falloffAngle = 0;
		};

		// ------------------
		// �֐�
		// ------------------

		/// <summary>
		/// ���C�g�z���������
		/// </summary>
		void InitializeLight();

		/// <summary>
		/// ���C�g���̍X�V
		/// �J�����ɋ߂����C�g��I���GPU�������ɃR�s�[����
		/// </summary>
		/// <param name="progStandard"> �V�F�[�_�v���O����(�W��)�̊Ǘ��ԍ� </param>
		/// <param name="camera"> �J���� </param>
		/// <param name="lights"> ���C�g�f�[�^�z�� </param>
		/// <param name="usedLights"> �g�p�����C�g </param>
		void Update(GLuint progStandard, const GameObject& mainCamera);

		/// <summary>
		/// �V�������C�g���擾����
		/// </summary>
		/// <returns> ���C�g�̃C���f�b�N�X </returns>
		int AllocateLight();

		/// <summary>
		/// ���C�g���������
		/// </summary>
		/// <param name="index"> ���C�g�̃C���f�b�N�X </param>
		void DeallocateLight(int index);

		/// <summary>
		/// �C���f�b�N�X�ɑΉ����郉�C�g�f�[�^���擾����
		/// </summary>
		/// <param name="index"> ���C�g�̃C���f�b�N�X </param>
		/// <returns>
		/// <para> nullptr�ȊO : index�ɑΉ����郉�C�g�f�[�^�̃A�h���X </para>
		/// <para> nullptr : �Ή����郉�C�g�f�[�^�����݂��Ȃ� </para>
		/// </returns>
		LightData* GetLight(int index);

	} // namespace LightData

} // namespace PokarinEngine

#endif // !LIGHTDATA_H_INCLUDED
