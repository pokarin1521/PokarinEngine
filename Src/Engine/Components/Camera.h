/**
* @file Camera.h
*/
#ifndef POKARINENGINE_CAMERA_H_INCLUDED
#define POKARINENGINE_CAMERA_H_INCLUDED

#include "Transform.h"

#include "../Math/Angle.h"
#include "../Math/Vector.h"

#include "../FramebufferObject.h"

#include "../Mesh/StaticMesh.h"

#include <memory>

namespace PokarinEngine
{
	/// <summary>
	/// �J�����p�R���|�[�l���g
	/// </summary>
	class Camera : public Component
	{
	public: // ------------------ �R���X�g���N�^�E�f�X�g���N�^ -------------------

		// �Q�[���I�u�W�F�N�g�ȊO�ł��g���̂ŃR���X�g���N�^��FBO���쐬����
		// FBO�̍쐬�����Ȃ�R���X�g���N�^�ł����Ȃ�
		Camera() { fbo = FramebufferObject::Create(); }
		~Camera() = default;

	public: // ------------------------------ ���� -------------------------------

		/// <summary>
		/// �X�V
		/// </summary>
		void Update();

		/// <summary>
		/// GPU�ɏ����R�s�[����
		/// </summary>
		void CopyToGPU();

		/// <summary>
		/// �X�J�C�X�t�B�A��`�悷��
		/// </summary>
		void DrawSkySphere();

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
		float GetFovScale() const { return inverseFovScale; }

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
			inverseFovScale = 1 / tan(radFovY / 2);
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

	public: // ------------------------- FBO�̃e�N�X�`�� -------------------------

		/// <summary>
		/// FBO�������Ă���e�N�X�`���̎��ʔԍ����擾����
		/// </summary>
		/// <returns> FBO�������Ă���e�N�X�`���̎��ʔԍ� </returns>
		GLuint GetTextureID() const
		{
			return fbo->GetTextureID();
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
		void FromJson(const Json& data) override;

	public: // ------------------------- �ʒu�E��]�p�x --------------------------

		// �ʒu�E��]�p�x
		// �g�嗦�͎g��Ȃ�
		Transform transform;

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
		// ����p�ɂ��g�嗦�͏�ɂ��̌`�Ŏg���̂ŁA���炩���ߋt���ɂ��Ă���
		float inverseFovScale = 1 / tan(radFovY / 2);

	private: // ----------------------------- �`��p -----------------------------

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

		// �`��pFBO
		FramebufferObjectPtr fbo;

		// �X�J�C�X�t�B�A�p���f��
		StaticMeshPtr skySphere;
	};
}

#endif // !POKARINENGINE_CAMERA_H_INCLUDED
