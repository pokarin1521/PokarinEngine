/**
* @file Collider.cpp
*/
#include "Collider.h"

#include "../../GameObject.h"

namespace PokarinEngine
{
	/// <summary>
	/// �X�V
	/// </summary>
	void Collider::Update()
	{
		// ������ł���Q�[���I�u�W�F�N�g��
		// ���������p�R���|�[�l���g�������Ă�����Փˎ��ɓ����悤�ɂ���
		isStatic = !GetOwnerObject().HasRigidbody();
	}
}