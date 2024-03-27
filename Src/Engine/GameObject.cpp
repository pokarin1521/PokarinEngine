/**
* @file GamaObject.cpp
*/
#include "GameObject.h"

#include "NodeScript/NodeScript.h"
#include "NodeScript/NodeEditor.h"

#include "Scene.h"

#include <algorithm>

namespace PokarinEngine
{
	/// <summary>
	/// �Q�[���I�u�W�F�N�g����폜�\��̃R���|�[�l���g���폜���� 
	/// </summary>
	void GameObject::RemoveDestroyedComponent()
	{
		if (components.empty())
		{
			return; // �R���|�[�l���g�������Ă��Ȃ���Ή������Ȃ�
		}

		// -------- �폜���������₷���悤�ɔj���\��̗L���ŐU�蕪�� ---------

		/* �ړ������邽�߂Ȃ̂�stable_partition���g��

		�j���\��̂Ȃ��R���|�[�l���g��z��̑O���ɁA
		�j���\��̂���R���|�[�l���g����둤�ɏW�߂�
		std::stable_partition(�͈͂̐擪, �͈͂̏I�[, �U�蕪������) */

		// iter�ɂ͔j���\��̂���R���|�[�l���g�̐擪�C�e���[�^������
		const auto iter = std::stable_partition(
			components.begin(), components.end(),
			[](const auto& e) {return !e->IsDestroyed(); });

		// �j���\��̃R���|�[�l���g��ʂ̔z��Ɉړ�
		std::vector<ComponentPtr> destroyList(
			std::move_iterator(iter),
			std::move_iterator(components.end()));

		// �ړ������ċ�ɂȂ����z����폜
		components.erase(iter, components.end());

		// �R���C�_�[��colliders�ɂ��o�^����Ă���̂ŁA��������폜����K�v������
		// �폜���邽�߂Ȃ̂ŁAremove_if���g��
		const auto iter2 = std::remove_if(
			colliders.begin(), colliders.end(),
			[](const auto& p) {return p->IsDestroyed(); });

		// �j���\��̂��̂��폜
		colliders.erase(iter2, colliders.end());

		// �j���\��̃R���|�[�l���g��OnDestroy�����s
		for (auto& e : destroyList)
		{
			e->OnDestroy();
		}

		// �����Ŏ��ۂɃR���|�[�l���g���폜�����(destroyList�̎������I��邽��)
	}

	/// <summary>
	/// ������
	/// </summary>
	void GameObject::Initialize()
	{
		// Transform�R���|�[�l���g��ǉ�
		if (!transform)
		{
			transform = AddComponent<Transform>();
		}

		// �m�[�h�G�f�B�^���쐬
		if (!nodeEditor)
		{
			nodeEditor = NodeEditor::Create(this);
		}
	}

	/// <summary>
	/// �X�V
	/// </summary>
	/// <param name="deltaTime"> �O��̍X�V����̌o�ߎ���(�b) </param>
	void GameObject::Update(float deltaTime)
	{
		// �R���|�[�l���g��Start���P�x�������s
		// �r���Œǉ�����邱�Ƃ�z�肵�āAUpdate���Ŏ��s
		for (auto& e : components)
		{
			if (!e->isStarted)
			{
				e->Start();
				e->isStarted = true;
			}
		}

		// �R���|�[�l���g���X�V
		for (auto& e : components)
		{
			e->Update(deltaTime);
		}

		// �R���|�[�l���g�̍폜���m�肳����
		RemoveDestroyedComponent();
	}

	/// <summary>
	/// �Փˎ��̏���
	/// </summary>
	/// <param name="self"> �Փ˂����R���|�[�l���g(����) </param>
	/// <param name="other"> �Փ˂����R���|�[�l���g(����) </param>
	void GameObject::OnCollision(const ComponentPtr& self, const ComponentPtr& other)
	{
		for (auto& e : components)
		{
			e->OnCollision(self, other);
		}
	}

	/// <summary>
	/// �폜
	/// </summary>
	void GameObject::OnDestroy()
	{
		// �R���|�[�l���g���폜
		for (auto& e : components)
		{
			e->OnDestroy();
		}
	}

	/// <summary>
	/// �m�[�h�G�f�B�^���J��
	/// </summary>
	void GameObject::OpenNodeEditor() const
	{
		NodeScript::OpenNodeEditor(nodeEditor);
	}

} // namespace PokarinEngine