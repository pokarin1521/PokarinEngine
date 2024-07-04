/**
* @file ComponentName.h
*/
#ifndef COMPONENTNAME_H_INCLUDED
#define COMPONENTNAME_H_INCLUDED

#include <string>
#include <typeindex>
#include <unordered_map>
#include <cassert>

namespace PokarinEngine
{
	// ---------------------
	// �O���錾
	// ---------------------

	class Component;

	/// <summary>
	/// �R���|�[�l���g�̖��O�Ǘ��p
	/// </summary>
	class ComponentName
	{
	public: // --------------- �R���X�g���N�^�E�f�X�g���N�^ ----------------

		ComponentName() = default;
		~ComponentName() = default;

	public: // ---------------------------- �擾 ---------------------------

		/// <summary>
		/// �R���|�[�l���g�̖��O���擾����
		/// </summary>
		/// <typeparam name="T"> �R���|�[�l���g </typeparam>
		/// <returns> �R���|�[�l���g�̖��O </returns>
		template <class T>
		static std::string Get()
		{
			// �R���|�[�l���g�ȊO���w�肳�ꂽ��G���[���o��
			// ���s�O�ɒm�肽���̂ŁAstatic_assert
			static_assert(std::is_base_of_v<Component, T>);

			// �擾�����R���|�[�l���g����Ԃ�
			return nameList[typeid(T)];
		}

	private: // ------------------- �R���|�[�l���g�̖��O -------------------

		// <�^(typeid), �R���|�[�l���g�̖��O>
		using NameList = std::unordered_map<std::type_index, std::string>;

		// <�R���|�[�l���g�̖��O, �^(typeid)>
		using TypeList = std::unordered_map<std::string, std::type_index>;

		// ���O���ʗp
		// <�^(typeid), �R���|�[�l���g�̖��O>
		static NameList nameList;

		// �^���ʗp
		// <�R���|�[�l���g�̖��O, �^(typeid)>
		static TypeList typeList;
	};
}

#endif // !COMPONENTNAME_H_INCLUDED
