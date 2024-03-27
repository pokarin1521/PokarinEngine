/**
* @file UILayout.cpp
*/
#include "UILayout.h"
#include "../Engine.h"
#include "../Scene.h"

namespace PokarinEngine
{
	/// <summary> 
	/// UI���C�A�E�g�̎��s���J�n���� 
	/// </summary>
	void UILayout::Start()
	{
		// �ŏ��Ɏw�肳�ꂽ���W���u�J�������猩�����W�v�Ƃ���
		basePosition = GetOwner().transform->position;
	}

	/// <summary>
	/// �X�V�C�x���g
	/// </summary>
	/// <param name="deltaTime"> 
	/// �O��̍X�V����̌o�ߎ���(�b) 
	/// </param>
	void UILayout::Update(float deltaTime)
	{
		/* ��ɃJ�����̕��������悤��
		�Q�[���I�u�W�F�N�g�̌����𒲐�(�r���{�[�h�Ɠ���) */

		// �R���|�[�l���g�̎�����
		const GameObject& owner = GetOwner();

		// ��������Ǘ����Ă���V�[��
		const Scene& scene = owner.GetScene();

		// �Q�[���I�u�W�F�N�g�̊p�x���J�����̊p�x�ɍ��킹��
		const GameObject& mainCamera = scene.GetMainCamera();
		owner.transform->rotation.y = mainCamera.transform->rotation.y;

		/* ��ɃJ�����̐��ʂɈʒu����悤��
		�Q�[���I�u�W�F�N�g�̍��W�𒲐�

		��]�̌����ɂ���ăJ�����Ƃ̑��΍��W�����߂� */

		// �p�x0�x�̏ꍇ�̍��W
		Vec3 pos = basePosition;

		// Y���W��-1�`+1���A
		// �J�����Ɏʂ�͈͂̏�[�Ɖ��[�ɂȂ�悤�ɁAZ���W�𒲐�
		// ����p���L����΋߂��ɁA
		// �����Ȃ�Ή����ɔz�u���邱�ƂŃT�C�Y���ς��Ȃ��悤�ɂ���
		pos.z = -scene.GetCameraInfo().GetFovScale();

		// ��]�̌v�Z�p
		const float s = sin(mainCamera.transform->rotation.y);
		const float c = cos(mainCamera.transform->rotation.y);

		// ���W�̉�] 
		pos = {
			pos.x * c + pos.z * s,
			pos.y,
			pos.x * -s + pos.z * c,
		};

		// �J�����Ƃ̑��΍��W
		owner.transform->position = mainCamera.transform->position + pos;
	}

} // namespace PokarinEngine
