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
		// �R���|�[�l���g�������Ă��Ȃ���Ή������Ȃ�
		if (componentList.empty())
		{
			return;
		}

		// --------------------------------------- 
		// �R���|�[�l���g���폜
		// --------------------------------------- 

		// --------------- �R���|�[�l���g�z�񂩂�폜 ------------------
		{
			/* �폜���̏��������s�o����悤��stable_partition���g��
			(remove_if���ƍ폜�����\��������)

			�j���\��̂Ȃ��R���|�[�l���g��z��̑O���ɁA
			�j���\��̂���R���|�[�l���g����둤�ɏW�߂�
			std::stable_partition(�͈͂̐擪, �͈͂̏I�[, �U�蕪������) */

			// �j���\��̃R���|�[�l���g�̐擪�C�e���[�^
			auto destroyStart = std::stable_partition(
				componentList.begin(), componentList.end(),
				[](const auto& e) {return !e->IsDestroyed(); });

			// �폜���̏��������s
			for (auto& itr = destroyStart; itr != componentList.end(); ++itr)
			{
				auto& destroyComponent = *itr;
				destroyComponent->OnDestroy();
			}

			// �j���\��̃R���|�[�l���g���폜
			componentList.erase(destroyStart, componentList.end());
		}

		// ----------------- �R���C�_�[�z�񂩂�폜 --------------------
		{
			// �R���C�_�[�̓R���C�_�[�z��ɂ��o�^����Ă���̂ŁA��������폜����K�v������
			// �폜���邾���Ȃ̂ŁAremove_if���g��
			const auto destroyStart = std::remove_if(
				colliderList.begin(), colliderList.end(),
				[](const auto& p) {return p->IsDestroyed(); });

			// �j���\��̂��̂��폜
			colliderList.erase(destroyStart, colliderList.end());
		}
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
			nodeEditor = std::make_shared<NodeEditor>(*this);
		}
	}

	/// <summary>
	/// �X�V
	/// </summary>
	void GameObject::Update()
	{
		// �R���|�[�l���g��Start���P�x�������s
		// �r���Œǉ�����邱�Ƃ�z�肵�āAUpdate���Ŏ��s
		for (auto& e : componentList)
		{
			if (!e->isStarted)
			{
				e->Start();
				e->isStarted = true;
			}
		}

		// �R���|�[�l���g���X�V
		for (auto& e : componentList)
		{
			e->Update();
		}

		nodeEditor->Run();

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
		for (auto& e : componentList)
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
		for (auto& e : componentList)
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