/**
* @file Hierarchy.h
*/
#ifndef HIERARCHY_H_INCLUDED
#define HIERARCHY_H_INCLUDED

#include "../UsingNames/UsingGameObject.h"

#include <memory>
#include <vector>

namespace PokarinEngine
{
	// ---------------------
	// �O���錾
	// ---------------------

	class Engine;

	/// <summary>
	/// �q�G�����L�[(�V�[�����̃I�u�W�F�N�g�Ǘ��p�E�B���h�E)
	/// </summary>
	namespace Hierarchy
	{
		/// <summary>
		/// ������
		/// </summary>
		void Initialize(Engine& e);

		/// <summary>
		/// �X�V
		/// </summary>
		void Update();

		/// <summary>
		/// �I�𒆂̃I�u�W�F�N�g�̗v�f�ԍ����擾����
		/// </summary>
		/// <returns> �E�B���h�E���őI�𒆂̃I�u�W�F�N�g�̗v�f�ԍ� </returns>
		GameObjectPtr GetSelectObject();
	} 

} // namespace PokarinEngine

#endif // !HIERARCHY_H_INCLUDED
