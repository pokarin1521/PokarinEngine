/**
* @file ComponentAdder.h
*/
#ifndef POKARINENGINE_COMPONENTADDER_H_INCLUDED
#define POKARINENGINE_COMPONENTADDER_H_INCLUDED

#include "../UsingNames/UsingComponent.h"
#include "../UsingNames/UsingGameObject.h"

#include <string>
#include <typeindex>
#include <unordered_map>
#include <cassert>
#include <functional>

namespace PokarinEngine
{
	/// <summary>
	/// �R���|�[�l���g�ǉ��Ɋւ��鏈���p
	/// </summary>
	class ComponentAdder
	{
	public: // --------------- �R���X�g���N�^�E�f�X�g���N�^ ----------------

		ComponentAdder() = default;
		~ComponentAdder() = default;

	public: // ---------------------------- �擾 ---------------------------

		/// <summary>
		/// �R���|�[�l���g�̖��O���擾����
		/// </summary>
		/// <typeparam name="T"> �R���|�[�l���g </typeparam>
		/// <returns> �R���|�[�l���g�̖��O </returns>
		template <class T>
		static std::string GetName()
		{
			// �R���|�[�l���g�ȊO���w�肳�ꂽ��G���[���o��
			// ���s�O�ɒm�肽���̂ŁAstatic_assert
			static_assert(std::is_base_of_v<Component, T>);

			// �擾�����R���|�[�l���g����Ԃ�
			return nameList[typeid(T)];
		}

		/// <summary>
		/// �Q�[���I�u�W�F�N�g�ɃR���|�[�l���g��ǉ�����
		/// </summary>
		/// <param name="[in] name"> �ǉ�����R���|�[�l���g�̖��O </param>
		/// <param name="[out] gameObject"> �Q�[���I�u�W�F�N�g </param>
		/// <returns> �ǉ������R���|�[�l���g </returns>
		static ComponentPtr AddComponent(const std::string& name, GameObject& gameObject);

		/// <summary>
		/// �R���|�[�l���g�ǉ��p���X�g��\������
		/// </summary>
		/// <param name="[out] gameObject"> �Q�[���I�u�W�F�N�g </param>
		static void RenderList(GameObjectPtr gameObject);

	private: // ------------------- �R���|�[�l���g�̖��O -------------------

		// <�^(typeid), �R���|�[�l���g�̖��O>
		using NameList = std::unordered_map<std::type_index, std::string>;

		// <�R���|�[�l���g�̖��O, �R���|�[�l���g�ǉ��p�֐�>
		using FunctionList = std::unordered_map<
			std::string, std::function<ComponentPtr(GameObject&)>>;

		// ���O���ʗp
		// <�^(typeid), �R���|�[�l���g�̖��O>
		static NameList nameList;

		// �R���|�[�l���g�ǉ��p�֐��̔z��
		// <�R���|�[�l���g�̖��O, �R���|�[�l���g�ǉ��p�֐�>
		static FunctionList functionList;
	};
}

#endif // !POKARINENGINE_COMPONENTADDER_H_INCLUDED
