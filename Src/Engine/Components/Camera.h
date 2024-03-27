/**
* @file Camera.h
*/
#ifndef CAMERA_H_INCLUDED
#define CAMERA_H_INCLUDED

#include "Component.h"

#include "../VecMath.h"

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
		virtual ~Camera() = default;

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

	public: // -------------------------- ����p�̐ݒ� ---------------------------

		/// <summary>
		/// ��������p��ݒ肷��
		/// </summary>
		/// <param name="fovY"> �ݒ肷�鐂������p(�x���@) </param>
		void SetFovY(float fovY)
		{
			// ��������p(�x���@)��ݒ�
			degFovY = fovY;

			// �ʓx�@�ɕϊ�
			radFovY = Radians(fovY);

			// ����p�ɂ��g�嗦�̋t��
			fovScale = 1 / tan(radFovY / 2);
		}

	public: // --------------------------- �G�f�B�^�p ----------------------------

		/// <summary>
		/// �G�f�B�^�ɏ���\������
		/// </summary>
		void RenderEditor() override;

	private: // ---------------------------- ����p ------------------------------

		// ��������p(�x���@)
		float degFovY = 60;

		// ��������p(�ʓx�@)
		float radFovY = Radians(degFovY);

		// ����p�ɂ��g�嗦�̋t��
		// (����p�ɂ��g�嗦�͏�ɂ��̌`�Ŏg���̂ŁA���炩���ߋt���ɂ��Ă���)
		float fovScale = 1 / tan(radFovY / 2);
	};
}

#endif // !CAMERA_H_INCLUDED
