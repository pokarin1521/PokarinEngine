/**
* @file ComponentAdder.cpp
*/
#include "Inspector.h"

#include "../GameObject.h"

#include "../Components/Rigidbody.h"
#include "../Components/Light.h"
#include "../Components/Colliders/AabbCollider.h"
#include "../Components/Colliders/SphereCollider.h"

#include "../Components/ComponentName.h"

namespace PokarinEngine
{
	/// <summary>
	/// �R���|�[�l���g�ǉ��p�{�^��
	/// </summary>
	/// <typeparam name="T"> �ǉ�����R���|�[�l���g </typeparam>
	/// <param name="[out] object"> �R���|�[�l���g�̒ǉ��Ώ� </param>
	template<class T>
	void AddButton(GameObjectPtr& object)
	{
		// �ǉ��p�{�^����\��
		if (ImGui::Button(ComponentName::Get<T>().c_str()))
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
	void Inspector::AddComponentList()
	{
		AddButton<Rigidbody>(selectObject);
		AddButton<Light>(selectObject);
		AddButton<AabbCollider>(selectObject);
		AddButton<SphereCollider>(selectObject);
	}
}