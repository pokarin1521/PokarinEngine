/**
* @file Vector.cpp
*/
#include "Vector.h"

#include "ImGui/imgui.h"
#include "Json/Json.h"

namespace PokarinEngine
{
#pragma region Editor

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
	///	�G�f�B�^�ɏ����h���b�O����p�X���C�_�[�ŕ\������
	/// </summary>
	/// <param name="[in] title"> �\������^�C�g�� </param>
	/// <param name="[in] id_string"> ���ʔԍ��̕����� </param>
	/// <param name="[in] startX"> �\���̊J�n�ʒu(X���W) </param>
	/// <param name="[in] width"> �\������X���C�_�[�̕� </param>
	void Vector3::RenderDrag(const std::string& title, const std::string& id_string,
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
			DragValue(x, "X", title, id_string);

			ImGui::SameLine();
			DragValue(y, "Y", title, id_string);

			ImGui::SameLine();
			DragValue(z, "Z", title, id_string);

			ImGui::PopItemWidth();
		}
	}

#pragma endregion

#pragma region Json

	/// <summary>
	/// ����Json�^�Ɋi�[����
	/// </summary>
	/// <param name="[out] data"> �����i�[����Json�^ </param>
	void Vector3::ToJson(Json& data) const
	{
		data["x"] = x;
		data["y"] = y;
		data["z"] = z;
	}

	/// <summary>
	/// ����Json�^����擾����
	/// </summary>
	/// <param name="[in] data"> �����i�[���Ă���Json�^ </param>
	void Vector3::FromJson(const Json& data)
	{
		x = data["x"];
		y = data["y"];
		z = data["z"];
	}

#pragma endregion
}