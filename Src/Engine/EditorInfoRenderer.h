/**
* @file EditorInforSetter.h
*/
#ifndef EDITORINFORSETTER_H_INCLUDED
#define EDITORINFORSETTER_H_INCLUDED

#include "Math/Vector.h"

#include <string>

namespace PokarinEngine
{
	/// <summary>
	/// �G�f�B�^�ɏ���\�����邽�߂̋@�\
	/// </summary>
	namespace EditorInfoRenderer
	{
		///// <summary>
		///// �`�F�b�N�{�b�N�X��\������
		///// </summary>
		///// <param name="name"> �`�F�b�N�{�b�N�X�̖��O </param>
		///// <param name="isCheck">  </param>
		//void CheckBox(const std::string& name, bool& isCheck)
		//{
		//	//ImGui::Checkbox(name.c_str(), &isCheck);
		//}

		/// <summary>
		///	Vector3�^�̏����h���b�O����p�X���C�_�[�ŕ\������
		/// </summary>
		/// <param name="[in,out] info"> �\�������� </param>
		/// <param name="[in] title"> �\������^�C�g�� </param>
		/// <param name="[in] id_string"> ���ʔԍ��̕����� </param>
		/// <param name="[in] startX"> �\���̊J�n�ʒu(X���W) </param>
		/// <param name="[in] width"> �\������X���C�_�[�̕� </param>
		void DragVector3(Vector3& info, const std::string& title, const std::string& id_string,
			const float width, const float startX = 0);
	}
}

#endif // !EDITORINFORSETTER_H_INCLUDED

