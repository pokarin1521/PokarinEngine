/**
* @file Transform.cpp
*/
#include "Transform.h"

#include "../Engine.h"

namespace PokarinEngine
{
	/// <summary>
	/// �Q�[���I�u�W�F�N�g���폜���ꂽ���̏���
	/// </summary>
	void Transform::OnDestroy()
	{
		// �e�I�u�W�F�N�g�Ƃ̐e�q�֌W����������
		SetParent(nullptr);

		// �q�I�u�W�F�N�g�Ƃ̐e�q�֌W����������
		for (auto child : children)
		{
			child->parent = nullptr;
		}
	}

	/// <summary>
	/// point�𒆐S��Y����]
	/// </summary>
	/// <param name="point"> ���S�̈ʒu </param>
	/// <param name="rotY"> Y����]������p�x(�ʓx�@) </param>
	/// <param name="range"> ���S����̋��� </param>
	void Transform::RotateAroundY(
		const Vector3& point, float rotY, float distance)
	{
		float sinY = std::sin(rotY);
		float cosY = std::cos(rotY);

		position.z = distance * cosY + point.z;
		position.x = distance * sinY + point.x;
	}

	/// <summary>
	/// �e�I�u�W�F�N�g��ݒ�
	/// </summary>
	/// <param name="parent"> 
	/// <para> �e�ɂ���Q�[���I�u�W�F�N�g�̃g�����X�t�H�[�� </para>
	/// <para> nullptr���w�肷��Ɛe�q�֌W���������� </para>
	/// </param>
	void Transform::SetParent(Transform* parent)
	{
		// ------------------------------------------
		// �����e���w�肳�ꂽ�ꍇ�́A�������Ȃ�
		// ------------------------------------------

		if (parent == this->parent)
		{
			return;
		}

		// ------------------------------------------------
		// ���ɐe������ꍇ�A���̐e�Ƃ̊֌W����������
		// ------------------------------------------------

		if (this->parent)
		{
			// �e�����q�I�u�W�F�N�g�z��
			auto& c = this->parent->children;

			// �����̈ʒu������
			auto itr = std::find(c.begin(), c.end(), this);

			// �������q�I�u�W�F�N�g�Ƃ��ēo�^����Ă���
			if (itr != c.end())
			{
				// �z�񂩂玩�����폜
				c.erase(itr);
			}
		}

		// ---------------------------
		// �V���Ȑe�q�֌W��ݒ�
		// ---------------------------

		// �V���Ȑe�̑��݊m�F
		if (parent)
		{
			// �e�q�֌W��ݒ�
			parent->children.push_back(this);
		}

		// �e�I�u�W�F�N�g�ɐݒ�
		this->parent = parent;
	}

	/// <summary>
	/// �e�I�u�W�F�N�g��ݒ�
	/// </summary>
	/// <param name="parent"> 
	/// <para> �e�ɂ���Q�[���I�u�W�F�N�g�̃g�����X�t�H�[�� </para>
	/// <para> nullptr���w�肷��Ɛe�q�֌W���������� </para>
	/// </param>
	void Transform::SetParent(const TransformPtr& parent)
	{
		SetParent(parent.get());
	}

	/// <summary>
	/// �l���h���b�O����p�X���C�_�[�ŕ\������
	/// </summary>
	/// <param name="axisName"> �\������l�̖��O </param>
	/// <param name="axis"> �\������l </param>
	void DragValue(std::string valueName, float& value, std::string info = "")
	{
		// ���ʗp���x��(��\���ɂ������̂�##)
		const std::string label = "##" + info + "." + valueName;

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

		// �h���b�O����p�X���C�_�[�̕���ݒ�
		ImGui::PushItemWidth(dragSliderWidth);
		{
			// �h���b�O����p�X���C�_�[��
			// ���O�Ɠ����s�ɕ\������
			ImGui::SameLine();
			ImGui::DragFloat(label.c_str(), &value, dragSpeed, 0, 0, format);

			// ���̐ݒ���I��
			ImGui::PopItemWidth();
		}
	}

	/// <summary>
	/// �����h���b�O����p�X���C�_�[�ŕ\������
	/// </summary>
	/// <param name="infoName"> �\��������̖��O </param>
	/// <param name="info"> �\�������� </param>
	void DragInformation(std::string infoName, Vector3& info)
	{
		// ----------------------------
		// ���̖��O��\��
		// ----------------------------

		ImGui::Text(infoName.c_str());

		// ----------------------------
		// ���ꂼ��̒l��\��
		// ----------------------------

		// �l��\�����n�߂�ʒu
		const float dragValueX = 90;

		ImGui::SameLine();
		ImGui::SetCursorPosX(dragValueX);
		DragValue("x", info.x, infoName);

		ImGui::SameLine();
		DragValue("y", info.y, infoName);

		ImGui::SameLine();
		DragValue("z", info.z, infoName);
	}

	/// <summary>
	/// �G�f�B�^�ɏ���\������
	/// </summary>
	void Transform::RenderInfo()
	{
		// �܂肽���݉\�ȃw�b�_�[��\��
		// �ŏ�����W�J���Ă���
		if (ImGui::CollapsingHeader("Transform",
			ImGuiTreeNodeFlags_::ImGuiTreeNodeFlags_DefaultOpen))
		{
			// -----------------------
			// �ʒu
			// -----------------------

			// �ʒu
			DragInformation("Position", position);

			// ----------------------------
			// ��]�p�x(�x���@)
			// ----------------------------

			DragInformation("Rotation", rotationDeg);

			// �ʓx�@�ɕϊ�
			rotation = Radians(rotationDeg);

			// ----------------------------
			// �g�嗦
			// ----------------------------

			DragInformation("Scale", scale);
		}
	}

} // namespace PokarinEngine
