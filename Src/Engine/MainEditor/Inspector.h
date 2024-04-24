/**
* @file Inspector.h
*/
#ifndef INSPECTOR_H_INCLUDED
#define INSPECTOR_H_INCLUDED

#include "ImGui/imgui.h"

#include "../UsingNames/UsingGameObject.h"

#include <string>
#include <memory>

namespace PokarinEngine
{
	/// <summary>
	/// �C���X�y�N�^�[(�V�[�����̃I�u�W�F�N�g����p�E�B���h�E)
	/// </summary>
	namespace Inspector
	{
		/// <summary>
		/// �X�V
		/// </summary>
		/// <param name="selectObject"> �q�G�����L�[�őI�𒆂̃I�u�W�F�N�g </param>
		void Update(GameObjectPtr selectObject);
	}

} // namespace PokarinEngine

#endif // !INSPECTOR_H_INCLUDED

