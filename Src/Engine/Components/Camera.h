/**
* @file Camera.h
*/
#ifndef POKARINENGINE_CAMERA_H_INCLUDED
#define POKARINENGINE_CAMERA_H_INCLUDED

#include "Transform.h"

#include "../Math/Angle.h"
#include "../Math/Vector.h"

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

		Camera() = default;
		~Camera() = default;

	public: // ------------------------------ �X�V -------------------------------
		
		/// <summary>
		/// �X�V
		/// </summary>
		void Update() override;

		/// <summary>
		/// GPU�ɏ����R�s�[����
		/// </summary>
		void CopyToGPU() const;

	public: // -------------------------- �X�J�C�X�t�B�A -------------------------

		/// <summary>
		/// �X�J�C�X�t�B�A��`�悷��
		/// </summary>
		void DrawSkySphere() const;

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

	private: // ---------------------------- ������ ------------------------------

		/// <summary>
		/// �ŏ��̍X�V�̒��O�ł̏�����
		/// </summary>
		void Start() override;

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

		// �X�J�C�X�t�B�A�p���f��
		StaticMeshPtr skySphere;

		// �`���̃f�B�X�v���C�ԍ�
		GLuint targetDisplayID = 0;
	};
}

#endif // !POKARINENGINE_CAMERA_H_INCLUDED
