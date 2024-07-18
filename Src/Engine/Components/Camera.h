/**
* @file Camera.h
*/
#ifndef CAMERA_H_INCLUDED
#define CAMERA_H_INCLUDED

#include "Component.h"

#include "../Math/Angle.h"

#include <memory>

namespace PokarinEngine
{
	class Camera;

	using CameraPtr = std::shared_ptr<Camera>;

	/// <summary>
	/// �J�����p�R���|�[�l���g
	/// </summary>
	class Camera : public Component
	{
	public: // ------------------ �R���X�g���N�^�E�f�X�g���N�^ -------------------

		Camera() = default;
		~Camera() = default;

	public: // -------------------------- ����p�̎擾 ---------------------------

		/// <summary>
		/// ��������p���擾
		/// </summary>
		/// <returns> ��������p(�x���@) </returns>
		float GetFovY() const { return degFovY; }

		/// <summary>
		/// ����p�ɂ��g�嗦���擾
		/// </summary>
		/// <returns> ����p�ɂ��g�嗦�̋t�� </returns>
		float GetFovScale() const { return fovScale; }

		/// <summary>
		/// ��������p��ݒ肷��
		/// </summary>
		/// <param name="[in] fovY"> �ݒ肷�鐂������p(�x���@) </param>
		void SetFovY(float fovY)
		{
			// ��������p(�x���@)��ݒ�
			degFovY = fovY;

			// �ʓx�@�ɕϊ�
			radFovY = Radians(fovY);

			// ����p�ɂ��g�嗦�̋t��
			fovScale = 1 / tan(radFovY / 2);
		}

	public: // ------------------------- �`��͈͂̎擾 --------------------------

		/// <summary>
		/// �ŏ��`��͈͂��擾����
		/// </summary>
		/// <returns> �`��͈͂̍ŏ��l </returns>
		float GetDrawNear() const
		{
			return drawRange.near;
		}

		/// <summary>
		/// �ő�`��͈͂��擾����
		/// </summary>
		/// <returns> �`��͈͂̍ő�l </returns>
		float GetDrawFar() const
		{
			return drawRange.far;
		}

	public: // ------------------------------ Json -------------------------------

		/// <summary>
		/// �R���|�[�l���g�̏���Json�^�Ɋi�[����
		/// </summary>
		/// <param name="[out] Json"> �����i�[����Json�^ </param>
		void ToJson(Json& data) const override;

		/// <summary>
		/// �R���|�[�l���g�̏���Json�^����擾����
		/// </summary>
		/// <param name="[out] data"> �����i�[���Ă���Json�^ </param>
		void FromJson(const Json& data) override {}

	private: // -------------------------- �G�f�B�^�p ----------------------------

		/// <summary>
		/// ����ҏW�ł���悤�ɕ\������
		/// </summary>
		void InfoEditor() override;

	private: // ----------------------------- ����p -----------------------------

		// ��������p(�x���@)
		float degFovY = 60;

		// ��������p(�ʓx�@)
		float radFovY = Radians(degFovY);

		// ����p�ɂ��g�嗦�̋t��
		// (����p�ɂ��g�嗦�͏�ɂ��̌`�Ŏg���̂ŁA���炩���ߋt���ɂ��Ă���)
		float fovScale = 1 / tan(radFovY / 2);

	private: // ---------------------------- �`��͈� ----------------------------

		/// <summary>
		/// �`��͈�
		/// </summary>
		struct DrawRange
		{
			// �ŏ��`��͈�
			float near = 0.35f;

			// �ő�`��͈�
			float far = 1000.0f;
		};

		// �`��͈�
		DrawRange drawRange;
	};
}

#endif // !CAMERA_H_INCLUDED
