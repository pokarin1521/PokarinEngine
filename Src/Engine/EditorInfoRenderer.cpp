/**
* @file EitorInforSetter.cpp
*/
#include "EditorInfoRenderer.h"

#include "ImGui/imgui.h"

namespace PokarinEngine
{
	/// <summary>
	/// �G�f�B�^�ɏ���\�����邽�߂̋@�\
	/// </summary>
	namespace EditorInfoRenderer
	{
		/// �����ł����g��Ȃ��̂ŁAcpp�݂̂ɏ���
		/// <summary>
		/// �l���h���b�O����p�X���C�_�[�ŕ\������
		/// </summary>
		/// <param name="[in,out] value"> �\������l </param>
		/// <param name="[in] valueName"> �\������l�̖��O </param>
		/// <param name="[in] title"> �\��������̃^�C�g�� </param>
		/// <param name="[in] id_string"> ���ʔԍ��̕����� </param>
		void DragValue(float& value, const std::string& valueName, 
			const std::string& title, const std::string& id_string)
		{
			// ���ʗp���x��(��\���ɂ������̂�##)
			const std::string label = "##" + title + id_string + "." + valueName;

			// �h���b�O����p�X���C�_�[��ImGui�E�B���h�E���ɑ΂��銄��
			static const float sliderRatio = 6.0f;

			// �h���b�O����p�X���C�_�[�̕�
			// ImGui�E�B���h�E�̕��ɍ��킹��
			const float dragSliderWidth = ImGui::GetWindowWidth() / sliderRatio;

			// �h���b�O����̑��x
			static const float dragSpeed = 0.2f;

			// �\�����錅�����w�肷��t�H�[�}�b�g
			// ����2���܂ŕ\������
			static const char* format = "%.2f";

			// �l�̖��O��\��
			ImGui::Text(valueName.c_str());

			// �h���b�O����p�X���C�_�[��
			// ���O�Ɠ����s�ɕ\������
			ImGui::SameLine();
			ImGui::DragFloat(label.c_str(), &value, dragSpeed, 0, 0, format);
		}

		/// <summary>
		///	Vector3�^�̏����h���b�O����p�X���C�_�[�ŕ\������
		/// </summary>
		/// <param name="[in,out] info"> �\�������� </param>
		/// <param name="[in] title"> �\������^�C�g�� </param>
		/// <param name="[in] id_string"> ���ʔԍ��̕����� </param>
		/// <param name="[in] width"> �\������X���C�_�[�̕� </param>
		/// <param name="[in] startX"> �\���̊J�n�ʒu(X���W) </param>
		void DragVector3(Vector3& info, const std::string& title, const std::string& id_string,
			const float width, const float startX)
		{
			// ----------------------------
			// ���̖��O��\��
			// ----------------------------

			// ���O�Ɠ����s�ɒl��\���ł���悤�ɂ���
			ImGui::Text(title.c_str());
			ImGui::SameLine();

			// ----------------------------
			// ���ꂼ��̒l��\��
			// ----------------------------

			// �J�n�ʒu��ݒ�
			if (startX != 0)
			{
				ImGui::SetCursorPosX(startX);
			}

			// ����ݒ�
			ImGui::PushItemWidth(width);
			{
				DragValue(info.x, "X", title, id_string);

				ImGui::SameLine();
				DragValue(info.y, "Y", title, id_string);

				ImGui::SameLine();
				DragValue(info.z, "Z", title, id_string);

				ImGui::PopItemWidth();
			}
		}
	}
}