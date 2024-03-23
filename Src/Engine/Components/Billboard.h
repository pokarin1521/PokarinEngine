/**
* @file Billboard.h
*/
#ifndef BILLBOARD_H_INCLUDED
#define BILLBOARD_H_INCLUDED

#include "Component.h"
#include "../Engine.h"

namespace PokarinEngine
{
	/**
	* �r���{�[�h���R���|�[�l���g
	*/
	class Billboard : public Component
	{
	public: // �R���X�g���N�^�E�f�X�g���N�^

		Billboard() = default;
		virtual ~Billboard() = default;

	public: // �C�x���g�֐�

		/// <summary>
		/// �X�V�C�x���g
		/// </summary>
		/// <param name="deltaTime"> �O��̍X�V����̌o�ߎ���(�b) </param>
		virtual void Update(float deltaTime) override
		{
			/* ��ɃJ�����̕��������悤�Ƀ��b�Z�[�W�I�u�W�F�N�g�̌����𒲐� */

			// �����傩��G���W�����擾
			const GameObject& owner = GetOwner(); // ������
			const Engine& engine = owner.GetEngine(); // �G���W��

			// �J�����̊p�x�Ǝ�����̊p�x�����킹��
			const GameObject& mainCamera = engine.GetCurrentScene().GetMainCamera();
			owner.transform->rotation.y = mainCamera.transform->rotation.y;
		}
	};

} // namespace PokarinEngine

#endif // !BILLBOARD_H_INCLUDED