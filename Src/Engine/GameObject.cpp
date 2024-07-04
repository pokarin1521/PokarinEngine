/**
* @file GamaObject.cpp
*/
#include "GameObject.h"

#include "Json/Json.h"

#include "Components/Colliders/Collider.h"

#include "Math/Matrix.h"

#include "NodeScript/NodeScript.h"
#include "NodeScript/NodeEditor.h"

#include "Scene.h"
#include "Random.h"

#include <algorithm>
#include <filesystem>

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

		// ------------------------------------- 
		// �R���|�[�l���g�z�񂩂�폜 
		// -------------------------------------
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

			// �폜����R���|�[�l���g�z��
			// �폜���̏����ŃR���|�[�l���g���ǉ������\��������̂ŁA
			// �폜����R���|�[�l���g���ړ�������
			std::vector<ComponentPtr> destroyList(
				std::move_iterator(destroyStart), std::move_iterator(componentList.end()));

			// �j���\��̃R���|�[�l���g���폜
			componentList.erase(destroyStart, componentList.end());

			// �R���|�[�l���g�̍폜���̏��������s
			for (auto& destroyComponent : destroyList)
			{
				destroyComponent->OnDestroy();
			}
		}

		// ------------------------------------- 
		// �R���C�_�[�z�񂩂�폜 
		// -------------------------------------
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
			transform = AddComponent<Transform>().first;
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
	/// <param name="[in] isPlayGame"> �쐬���̃Q�[�����Đ����Ȃ�true </param>
	void GameObject::Update(bool isPlayGame)
	{
		// �R���|�[�l���g�̍X�V
		UpdateComponent(isPlayGame);

		// �Q�[�����Đ����ꂽ����s����
		if (isPlayGame)
		{
			// �m�[�h�G�f�B�^�Őݒ肵���m�[�h�̎��s
			nodeEditor->Run();
		}

		// ���W�ϊ��s��̍X�V
		UpdateMatrix();
	}

	/// <summary>
	/// �Q�[���I�u�W�F�N�g�ɂ���R���|�[�l���g���X�V����
	/// </summary>
	/// <param name="[in] isPlayGame"> �쐬���̃Q�[�����Đ����Ȃ�true </param>
	void GameObject::UpdateComponent(bool isPlayGame)
	{
		// �R���|�[�l���g��Start���P�x�������s
		// �r���Œǉ�����邱�Ƃ�z�肵�āAUpdate���Ŏ��s
		for (auto& component : componentList)
		{
			component->Initialize();
		}

		// �R���|�[�l���g���X�V
		for (auto& component : componentList)
		{
			// �X�V
			component->Update();

			// �Q�[���Đ����̍X�V����
			if (isPlayGame)
			{
				component->Update_PlayGame();
			}
		}

		// �R���|�[�l���g�̍폜���m�肳����
		RemoveDestroyedComponent();
	}

	/// <summary>
	/// �Q�[���I�u�W�F�N�g�̍��W�ϊ��s����X�V����
	/// </summary>
	void GameObject::UpdateMatrix()
	{
		// ----------------------------------
		// ���g�̍��W�ϊ��s������߂�
		// ----------------------------------

		// �I�u�W�F�N�g�̍��W
		// ������W�n�̒l�Ȃ̂ŉE����W�n�ɂ���
		Vector3 position = transform->position;
		position.z *= -1;

		// ���g�̍��W�ϊ��s��
		Matrix4x4 transformMatrix = Matrix4x4::CreateTransformMatrix(
			position, transform->rotation, transform->scale);

		// ���g�̖@���ϊ��s��
		Matrix3x3 normalMatrix = Matrix3x3::CreateRotationMatrix(transform->rotation);

		// ���W�ϊ��s���ݒ�
		transform->transformMatrix = transformMatrix;

		// �@���ϊ��s���ݒ�
		transform->normalMatrix = normalMatrix;

		// ------------------------------------
		// �e�̍��W�ϊ��s��𔽉f����
		// ------------------------------------

		// �e�����ǂ��Ă���
		for (Transform* parent = transform->GetParent(); parent; parent = parent->GetParent())
		{
			// �e�̍��W�ϊ��s��
			Matrix4x4 parentTransformMatrix = parent->transformMatrix;

			// �e�̖@���ϊ��s��
			Matrix3x3 parentNormalMatrix = parent->normalMatrix;

			// �e�̕ϊ��s����|�����킹��
			transformMatrix *= parentTransformMatrix;
			normalMatrix *= parentNormalMatrix;
		}
	}

	/// <summary>
	/// �폜
	/// </summary>
	void GameObject::OnDestroy()
	{
		// �R���|�[�l���g���폜
		for (auto& component : componentList)
		{
			component->OnDestroy();
		}

		// �m�[�h�G�f�B�^�����
		NodeScript::CloseNodeEditor(nodeEditor);
	}

	/// <summary>
	/// �R���|�[�l���g���G�f�B�^�ɕ\������
	/// </summary>
	void GameObject::RenderComponent()
	{
		// �R���|�[�l���g��\��
		for (auto& component : componentList)
		{
			component->RenderInfo();
		}
	}

	/// <summary>
	/// �m�[�h�G�f�B�^���J��
	/// </summary>
	void GameObject::OpenNodeEditor() const
	{
		NodeScript::OpenNodeEditor(nodeEditor);
	}

	/// <summary>
	/// �Q�[���I�u�W�F�N�g�̏���ۑ�����
	/// </summary>
	/// <param name="[in] sceneFolderName"> �V�[���t�H���_�� </param>
	void GameObject::SaveGameObject(const std::string& sceneFolderName) const
	{
		// ----------------------------------
		// �t�H���_���쐬����
		// ----------------------------------

		// �ۑ���̃t�H���_��
		std::string folderName = sceneFolderName + "/" + std::to_string(id);

		// �t�H���_�����݂��Ȃ��ꍇ�͍쐬����
		std::filesystem::create_directories(folderName);

		// ----------------------------------------
		// �R���|�[�l���g�̏���ۑ�����
		// ----------------------------------------

		for (auto& component : componentList)
		{
			//component->SaveInfo(folderName);
		}
	}

	/// <summary>
	/// �R���|�[�l���g���ʔԍ����擾����
	/// </summary>
	/// <returns> �d�����Ȃ����ʔԍ� </returns>
	int GameObject::GetSingleComponentID()
	{
		// �R���|�[�l���g���ʔԍ�
		int componentID = Random::Range(INT_MIN, INT_MAX);

		// �R���|�[�l���g���ʔԍ���ǉ�
		// �d�����Ă���ꍇ�́A�Ď擾����
		while (!componentIDList.emplace(componentID).second)
		{
			componentID = Random::Range(INT_MIN, INT_MAX);
		}

		return componentID;
	}

} // namespace PokarinEngine