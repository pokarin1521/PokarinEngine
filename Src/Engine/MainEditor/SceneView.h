/**
* @file SceneView.h
*/
#ifndef POKARINENGINE_SCENEVIEW_H_INCLUDED
#define POKARINENGINE_SCENEVIEW_H_INCLUDED

#include "RenderView.h"

#include "../Color.h"
#include "../Math/Vector.h"
#include "../Components/Camera.h"

namespace PokarinEngine
{
	/// <summary>
	/// �V�[���r���[(�V�[�����̏���`�悷��r���[)
	/// </summary>
	class SceneView : public RenderView
	{
	public: // ----------------- �R���X�g���N�^�E�f�X�g���N�^ ---------------------

		SceneView()
		{
			// �J�����̏����ʒu��ݒ�
			camera.SetPosition(cameraStartPos);
		}

		~SceneView() = default;

	public: // ------------------------------ ���� --------------------------------

		/// <summary>
		/// �X�V
		/// </summary>
		void Update() override;

		

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

		// �J����
		Camera camera;

		// �J�����̏����ʒu
		const Vector3 cameraStartPos = { 0.0f, 1.0f, -5.0f };

		// �w�i�F
		Color backGround = { 0.3f, 0.3f, 0.3f, 1.0f };

		// �}�E�X�J�[�\���̈ʒu
		Vector2 mousePos = Vector2(0, 0);

		// �}�E�X�J�[�\���̏����ʒu
		Vector2 startMousePos = Vector2(0, 0);

		// �J�����̉�]���x
		float cameraRotateSpeed = 0.5f;

		// �J�����̈ړ����x
		float cameraMoveSpeed = 2.0f;

		// �J���������ԂȂ�true
		bool isControlCamera = false;
	};

} // namespace PokarinEngine

#endif // !POKARINENGINE_SCENEVIEW_H_INCLUDED
