/**
* @file GamaObject.cpp
*/
#include "GameObject.h"

#include "Json/Json.h"

#include "Components/Colliders/Collider.h"

#include "Components/ComponentAdder.h"

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
				componentIDList.erase(destroyComponent->GetID());
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
	/// <param name="[in] scene"> ������ł���V�[�� </param>
	/// <param name="[in] objectID"> ���ʔԍ� </param>
	/// <param name="[in] meshFile"> �X�^�e�B�b�N���b�V���̃t�@�C���� </param>
	/// <param name="[in] objectName"> ���O </param>
	/// <param name="[in] position"> �ʒu </param>
	/// <param name="[in] rotation"> ��]�p�x </param>
	void GameObject::Initialize(Scene& scene, int objectID,
		const std::string& meshFile, const std::string& objectName,
		const Vector3& position, const Vector3& rotation)
	{
		// ----------------------------------
		// ���̐ݒ�
		// ----------------------------------

		// ������ł���V�[����o�^
		ownerScene = &scene;

		// ���ʔԍ���ݒ�
		id = objectID;

		// ���O��ݒ�
		name = objectName;

		// �X�^�e�B�b�N���b�V����ݒ�
		staticMesh = scene.GetStaticMesh(meshFile);

		// �X�^�e�B�b�N���b�V��������Ȃ�ŗL�}�e���A����ݒ�
		if (staticMesh)
		{
			// ���L�}�e���A���̃R�s�[��
			// �ŗL�}�e���A���Ƃ��Đݒ肷��
			materials = CloneMaterialList(staticMesh);
		}

		// ------------------------------------
		// �R���|�[�l���g�̒ǉ�
		// ------------------------------------

		// �ʒu�Ȃǂ̐���p�R���|�[�l���g��ǉ�
		if (!transform)
		{
			transform = AddComponent<Transform>();
		}

		// �m�[�h�G�f�B�^���쐬
		if (!nodeEditor)
		{
			nodeEditor = std::make_shared<NodeEditor>(*this);
		}

		// --------------------------------------
		// �ʒu�E��]�p�x�̐ݒ�
		// --------------------------------------

		transform->position = position;
		transform->rotation = rotation;
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
	/// �R���C�_�[��`�悷��
	/// </summary>
	void GameObject::DrawCollider()
	{
		for (auto& collider : colliderList)
		{
			collider->Draw();
		}
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
	/// �Q�[���I�u�W�F�N�g�̏���Json�^�Ɋi�[����
	/// </summary>
	/// <param name="[out] data"> �����i�[����Json�^ </param>
	void GameObject::ToJson(Json& data) const
	{
		// -----------------------------------------------
		// �Q�[���I�u�W�F�N�g�̏���Json�^�Ɋi�[����
		// -----------------------------------------------

		// ���O
		data["Name"] = name;

		// �X�^�e�B�b�N���b�V���̃t�@�C����
		// �Ȃ���΁unull�v�ɂ���
		if (staticMesh)
		{
			data["StaticMeshFile"] = staticMesh->filename;
		}
		else
		{
			data["StaticMeshFile"] = "null";
		}

		// -------------------------------------------
		// �R���|�[�l���g�̏���Json�^�Ɋi�[����
		// -------------------------------------------

		// �R���|�[�l���g���ʔԍ�(������)�̔z��
		// ���Ԃ��ێ����邽�߂�vector�^�ɂ���
		std::vector<std::string> stringIDList;

		// �R���|�[�l���g�̏����i�[
		for (auto& component : componentList)
		{
			// �R���|�[�l���g�̎��ʔԍ�(������)
			const std::string id_string = component->GetID_String();

			// �R���|�[�l���g�̖��O
			data[id_string]["Name"] = component->GetName();

			// �e�R���|�[�l���g�̏��
			component->ToJson(data[id_string]);

			// ���ʔԍ�
			stringIDList.push_back(component->GetID_String());
		}

		// �R���|�[�l���g���ʔԍ��̊Ǘ��p�z��
		data["ComponentIDList"] = stringIDList;
	}

	/// <summary>
	/// �Q�[���I�u�W�F�N�g�̏���Json�^����擾����
	/// </summary>
	/// <param name="[in] data"> �����i�[���Ă���Json�^ </param>
	void GameObject::FromJson(const Json& data)
	{
		// --------------------------------------------------
		// �Q�[���I�u�W�F�N�g�̏���Json�^����擾����
		// --------------------------------------------------

		// ���O
		name = data["Name"];

		// �X�^�e�B�b�N���b�V���̃t�@�C����
		const std::string& fileName = data["StaticMeshFile"];
		staticMesh = ownerScene->GetStaticMesh(fileName);

		// --------------------------------------------------
		// �R���|�[�l���g�̏���Json�^����擾����
		// --------------------------------------------------

		for (const std::string& id_string : data["ComponentIDList"])
		{
			// �R���|�[�l���g�̖��O
			const std::string componentName = data[id_string]["Name"];

			// ���O�ɑΉ������R���|�[�l���g��ǉ�
			ComponentPtr component = ComponentAdder::AddComponent(componentName, *this);

			// �ǉ������R���|�[�l���g�̏���Json�^����擾
			component->FromJson(data[id_string]);
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