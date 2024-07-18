/**
* @file Transform.cpp
*/
#include "Transform.h"

#include "ImGui/imgui.h"
#include "Json/Json.h"

#include "../Engine.h"

#include <fstream>

// Vector3�^��for��
#define ForVector3() for(int i = 0; i < 3; ++i)

namespace PokarinEngine
{
	/// <summary>
	/// �X�V
	/// </summary>
	void Transform::Update()
	{
		// -------------------------------
		// �ʒu�𐧌�
		// -------------------------------

		// �ʒu�̍ő�l
		// Unity���Q�l��10���Őݒ肷��
		static const float positionMax = 100000;

		// �ʒu���}10���͈̔͂ɂȂ�悤�ɐ�������
		ForVector3()
		{
			// �v���X�����̐���
			if (position[i] > positionMax)
			{
				position[i] = positionMax;
			}

			// �}�C�i�X�����̐���
			if (position[i] < -positionMax)
			{
				position[i] = -positionMax;
			}
		}

		// -------------------------------
		// ��]�p�x�𐧌�
		// -------------------------------

		// ��]�p�x�̍ő�l
		static const float rotationMax = Radians(360.0f);

		// ��]�p�x���}360�x�͈̔͂ɂȂ�悤�ɐ�������
		ForVector3()
		{
			if (std::abs(rotation[i]) > rotationMax)
			{
				// ����
				int laps = static_cast<int>(rotation[i] / rotationMax);

				// ���񐔂ɉ����ĉ�]�p�x�̍ő�l�����炷���ƂŁA
				// �������������ł̓K�؂Ȑ��l�����߂�
				rotation[i] -= rotationMax * laps;
			}
		}

		// -------------------------------
		// �g�嗦
		// -------------------------------

		// �g�嗦�̍ő�l
		// �ʒu�Ɠ����ɂ��Ă���
		static const float scaleMax = 100000;

		// �g�嗦���}10���͈̔͂ɂȂ�悤�ɐ�������
		ForVector3()
		{
			// �v���X�����̐���
			if (scale[i] > scaleMax)
			{
				scale[i] = scaleMax;
			}

			// �}�C�i�X�����̐���
			if (scale[i] < -scaleMax)
			{
				scale[i] = -scaleMax;
			}
		}
	}

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
	/// �e�I�u�W�F�N�g��ݒ肷��
	/// </summary>
	/// <param name="[out] parent"> 
	/// <para> �e�ɂ���Q�[���I�u�W�F�N�g�̃g�����X�t�H�[�� </para>
	/// <para> nullptr���w�肷��Ɛe�q�֌W���������� </para>
	/// </param>
	void Transform::SetParent(Transform* _parent)
	{
		// ------------------------------------------
		// �����e���w�肳�ꂽ�ꍇ�́A�������Ȃ�
		// ------------------------------------------

		if (_parent == parent)
		{
			return;
		}

		// ------------------------------------------------
		// ���ɐe������ꍇ�A���̐e�Ƃ̊֌W����������
		// ------------------------------------------------

		if (parent)
		{
			// �e�����q�I�u�W�F�N�g�z��
			auto& c = parent->children;

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
		if (_parent)
		{
			// �e�q�֌W��ݒ�
			_parent->children.push_back(this);
		}

		// �e�I�u�W�F�N�g�ɐݒ�
		this->parent = parent;
	}

	/// <summary>
	/// �e�I�u�W�F�N�g��ݒ肷��
	/// </summary>
	/// <param name="[out] parent"> 
	/// <para> �e�ɂ���Q�[���I�u�W�F�N�g�̃g�����X�t�H�[�� </para>
	/// <para> nullptr���w�肷��Ɛe�q�֌W���������� </para>
	/// </param>
	void Transform::SetParent(const TransformPtr& parent)
	{
		SetParent(parent.get());
	}

	/// <summary>
	/// ����ҏW�ł���悤�ɕ\������
	/// </summary>
	void Transform::InfoEditor()
	{
		// --------------------------------------
		// �\���̊J�n�ʒu�ƕ���ݒ�
		// --------------------------------------

		// �h���b�O����p�X���C�_�[��ImGui�E�B���h�E���ɑ΂��銄��
		static const float sliderRatio = 6.0f;

		// �h���b�O����p�X���C�_�[�̕�
		// ImGui�E�B���h�E�̕��ɍ��킹��
		const float sliderWidth = ImGui::GetWindowWidth() / sliderRatio;

		// �l�\���̊J�n�ʒu
		static const float startX = 90.0f;

		// -----------------------
		// �ʒu
		// -----------------------

		// ���ʔԍ��̕�����
		const std::string id_string = GetID_String();

		// �ʒu
		position.RenderDrag("Position", id_string, sliderWidth, startX);

		// ----------------------------
		// ��]�p�x(�x���@)
		// ----------------------------

		// ��]�p�x(�x���@)
		Vector3 rotationDeg = Degrees(rotation);

		// ������₷���悤�ɓx���@�ŕ\��
		rotationDeg.RenderDrag("Rotation", id_string, sliderWidth, startX);

		// �ʓx�@�ɕϊ�
		rotation = Radians(rotationDeg);

		// ----------------------------
		// �g�嗦
		// ----------------------------

		scale.RenderDrag("Scale", id_string, sliderWidth, startX);
	}

	/// <summary>
	/// �R���|�[�l���g�̏���Json�^�Ɋi�[����
	/// </summary>
	/// <param name="[out] Json"> �����i�[����Json�^ </param>
	void Transform::ToJson(Json& data) const
	{
		// ------------------------------------
		// ����Json�Ɋi�[����
		// ------------------------------------

		position.ToJson(data["Position"]);
		rotation.ToJson(data["Rotation"]);
		scale.ToJson(data["Scale"]);
	}

	/// <summary>
	/// �R���|�[�l���g�̏���Json�^����擾����
	/// </summary>
	/// <param name="[out] data"> �����i�[���Ă���Json�^ </param>
	void Transform::FromJson(const Json& data)
	{
		position.FromJson(data["Position"]);
		rotation.FromJson(data["Rotation"]);
		scale.FromJson(data["Scale"]);
	}

} // namespace PokarinEngine
