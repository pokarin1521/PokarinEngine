/**
* @file ComponentAdder.cpp
*/
#include "ComponentAdder.h"

#include "Colliders/AabbCollider.h"
#include "Colliders/SphereCollider.h"

#include "Camera.h"
#include "Light.h"
#include "Rigidbody.h"
#include "Transform.h"

#include "../GameObject.h"

#include <functional>

namespace PokarinEngine
{
	// -----------------------------
	// �ϐ�
	// -----------------------------

	// ���O���ʗp
	// <�^(typeid), �R���|�[�l���g�̖��O>
	ComponentAdder::NameList ComponentAdder::nameList = {
		{typeid(AabbCollider), "Aabb Collider"},
		{typeid(SphereCollider), "Sphere Collider"},

		{typeid(Camera), "Camera"},
		{typeid(Light), "Light"},
		{typeid(Rigidbody), "Rigidbody"},
		{typeid(Transform), "Transform"},
	};

	// �R���|�[�l���g�ǉ��p�֐��̔z��
	// <�R���|�[�l���g�̖��O, �R���|�[�l���g�ǉ��p�֐�>
	ComponentAdder::FunctionList ComponentAdder::functionList = {
		{"Aabb Collider", &GameObject::AddComponent<AabbCollider>},
		{"Sphere Collider", &GameObject::AddComponent<SphereCollider>},

		{"Camera", &GameObject::AddComponent<Camera>},
		{"Light", &GameObject::AddComponent<Light>},
		{"Rigidbody", &GameObject::AddComponent<Rigidbody>},
		{"Transform", &GameObject::AddComponent<Transform>},
	};

	// --------------------------------
	// �֐�
	// --------------------------------

	/// �����ł����g��Ȃ��̂ŁAcpp�݂̂ɏ���
	/// �w�b�_�[�ɃC���N���[�h�������Ȃ��̂ŁA�����o�֐��ɂ��Ȃ�
	/// <summary>
	/// �R���|�[�l���g�ǉ��p�{�^��
	/// </summary>
	/// <typeparam name="T"> �ǉ�����R���|�[�l���g </typeparam>
	/// <param name="[out] object"> �R���|�[�l���g�̒ǉ��Ώ� </param>
	template<class T>
	void AddButton(GameObjectPtr& object)
	{
		// �ǉ��p�{�^����\��
		if (ImGui::Button(ComponentAdder::GetName<T>().c_str()))
		{
			// �{�^���������ꂽ��ǉ�����
			object->AddComponent<T>();

			// �ǉ�������s�v�ɂȂ�̂ŁA�|�b�v�A�b�v�����
			ImGui::CloseCurrentPopup();
		}
	}

	/// <summary>
	/// �R���|�[�l���g�ǉ��p���X�g��\������
	/// </summary>
	/// <param name="[out] gameObject"> �Q�[���I�u�W�F�N�g </param>
	void ComponentAdder::RenderList(GameObjectPtr gameObject)
	{
		AddButton<Rigidbody>(gameObject);
		AddButton<Light>(gameObject);
		AddButton<AabbCollider>(gameObject);
		AddButton<SphereCollider>(gameObject);
	}

	/// <summary>
	/// �Q�[���I�u�W�F�N�g�ɃR���|�[�l���g��ǉ�����
	/// </summary>
	/// <param name="[in] name"> �ǉ�����R���|�[�l���g�̖��O </param>
	/// <param name="[out] gameObject"> �Q�[���I�u�W�F�N�g </param>
	/// <returns> �ǉ������R���|�[�l���g </returns>
	ComponentPtr ComponentAdder::AddComponent(const std::string& name, GameObject& gameObject)
	{
		if (functionList.find(name) == functionList.end())
		{
			return nullptr;
		}

		return functionList[name](gameObject).first;
	}
}