/**
* @file LightData.h
*/
#ifndef LIGHTDATA_H_INCLUDED
#define LIGHTDATA_H_INCLUDED

#include "Math/Vector.h"
#include "Math/Angle.h"
#include "GameObject.h"

#include <GLFW/glfw3.h>
#include <vector>

namespace PokarinEngine
{
	/// <summary>
	/// ���C�g�̃p�����[�^�Ǘ��p
	/// </summary>
	namespace LightParameter
	{
		// -------------------------
		// ���C�g
		// -------------------------

		/// <summary>
		/// ���C�g�̎��
		/// </summary>
		enum class Type
		{
			pointLight,	 // �_����
			spotLight,	 // �X�|�b�g����
			directional, // ���s����
		};

		/// <summary>
		/// ���C�g�̏��
		/// </summary>
		struct LightData
		{
			// ------------------
			// ���C�g�̎��
			// ------------------

			// ���C�g�̎��
			Type type = Type::pointLight;

			// ----------------- 
			// ���C�g�S�� 
			// -----------------

			Color color = { 1,1,1,1 };	    // �F
			float intensity = 5;		    // ���邳
			Vector3 position = { 0,0,0 };	// �ʒu
			float range = 10;			    // ���C�g���͂��ő�͈�(���a)

			// ---------------------
			// �X�|�b�g���C�g 
			// ---------------------

			// ���C�g�̌���
			Vector3 direction = { 0,0,0 };

			// �X�|�b�g���C�g�̍ő�Ǝˊp�x
			// 0�ȉ��Ȃ�_�����Ƃ��Ĉ���
			float spotAngle = Radians(30);

			// �X�|�b�g���C�g�̌����J�n�p�x
			float falloffAngle = Radians(20);
		};

		// ------------------
		// �֐�
		// ------------------

		/// <summary>
		/// ���C�g����GPU�ɃR�s�[����
		/// </summary>
		/// <param name="mainCamera"> ���C���J���� </param>
		void CopyGPU(const GameObject& mainCamera);

		/// <summary>
		/// ���C�g�f�[�^��ǉ�����
		/// </summary>
		/// <param name="lightData"> �ǉ����郉�C�g�f�[�^ </param>
		void AddLightData(LightData& lightData);

		/// <summary>
		/// ���C�g�f�[�^���폜����
		/// </summary>
		/// <param name="lightData"> �폜���郉�C�g�f�[�^ </param>
		void EraseLightData(LightData& lightData);

	} // namespace LightData

} // namespace PokarinEngine

#endif // !LIGHTDATA_H_INCLUDED
