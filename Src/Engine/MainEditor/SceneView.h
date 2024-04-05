/**
* @file SceneView.h
*/
#ifndef SCENEVIEW_H_INCLUDED
#define SCENEVIEW_H_INCLUDED

#include "RenderView.h"

#include "../Color.h"
#include "../Math/Vector.h"
#include "../Components/Transform.h"

namespace PokarinEngine
{
	/// <summary>
	/// �V�[���r���[(�V�[�����̏���`�悷��r���[)
	/// </summary>
	class SceneView : public RenderView
	{
	public: // ----------------- �R���X�g���N�^�E�f�X�g���N�^ ---------------------

		SceneView() = default;
		~SceneView() = default;

	public: // ------------------------------ ���� --------------------------------

		/// <summary>
		/// �X�V
		/// </summary>
		void Update() override;

	public: // --------------------------- �J�����擾 -----------------------------

		/// <summary>
		/// �V�[���r���[�p�J�������擾����
		/// </summary>
		/// <returns> �V�[���r���[�p�J���� </returns>
		const Transform& GetCamera() const
		{
			return sceneCamera;
		}

	private: // -------------------------- �J�������� -----------------------------

		/// <summary>
		/// �J�����̈ʒu�𑀍삷��
		/// </summary>
		void CameraMoveControl();

		/// <summary>
		/// �J�����̉�]�𑀍삷��
		/// </summary>
		void CameraRotateControl();

	private: // ------------------------------ ��� -------------------------------

		// �V�[���r���[�p�J����
		Transform sceneCamera;

		// �w�i�F
		Color backGround = { 0.3f, 0.3f, 0.3f, 1.0f };

		// �}�E�X�J�[�\���̈ʒu
		Vector2 mousePos = Vector2(0, 0);

		// �}�E�X�J�[�\���̏����ʒu
		Vector2 startMousePos = Vector2(0, 0);

		// �J�����̉�]���x
		float cameraRotateSpeed = 0.005f;

		// �J�����̈ړ����x
		float cameraMoveSpeed = 0.03f;

		// �J���������ԂȂ�true
		bool isControlCamera = false;
	};

} // namespace PokarinEngine

#endif // !SCENEVIEW_H_INCLUDED
