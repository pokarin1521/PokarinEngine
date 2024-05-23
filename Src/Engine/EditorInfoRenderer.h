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
		/// <summary>
		///	Vector3�^�̏����h���b�O����p�X���C�_�[�ŕ\������
		/// </summary>
		/// <param name="info"> �\�������� </param>
		/// <param name="width"> �\������X���C�_�[�̕� </param>
		/// <param name="infoName"> �\��������̖��O </param>
		/// <param name="startX"> �\���̊J�n�ʒu(X���W) </param>
		void DragVector3(Vector3& info, const float width, const std::string& infoName, const float startX = 0);
	}
}

#endif // !EDITORINFORSETTER_H_INCLUDED

