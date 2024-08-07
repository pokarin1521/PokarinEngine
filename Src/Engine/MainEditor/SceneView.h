/**
* @file SceneView.h
*/
#ifndef POKARINENGINE_SCENEVIEW_H_INCLUDED
#define POKARINENGINE_SCENEVIEW_H_INCLUDED

#include "../Color.h"
#include "../Math/Vector.h"

#include "../Components/Camera.h"

#include "../UsingNames/UsingFramebufferObject.h"

namespace PokarinEngine
{
	/// <summary>
	/// �V�[���r���[(�V�[�����̏���`�悷��r���[)
	/// </summary>
	class SceneView
	{
	public: // ------------------ �R���X�g���N�^�E�f�X�g���N�^ --------------------

		SceneView()
		{
			// �J�����̏����ʒu��ݒ�
			camera.transform.position = cameraStartPos;
		}

		~SceneView() = default;

	public: // ---------------------------- �֎~���� ------------------------------

		/* �G�f�B�^�Ǘ������������Ȃ�̂ŋ֎~���� */

		// �R�s�[�R���X�g���N�^�̋֎~
		SceneView(const SceneView&) = delete;

		// ����̋֎~
		SceneView& operator=(const SceneView&) = delete;

	public: // ------------------------------ ���� --------------------------------

		/// <summary>
		/// �X�V
		/// </summary>
		void Update();

		/// <summary>
		/// �`��
		/// </summary>
		void Render();

	private: // -------------------------- �J�������� -----------------------------

		/// <summary>
		/// �J�����̈ʒu�𑀍삷��
		/// </summary>
		void CameraMoveControl();

		/// <summary>
		/// �J�����̉�]�𑀍삷��
		/// </summary>
		void CameraRotateControl();

	private: // ----------------------------- �J���� ------------------------------

		// �J����
		Camera camera;

		// �J�����̏����ʒu
		// ���������I�u�W�F�N�g��������ʒu�ɂ���
		const Vector3 cameraStartPos = { 0.0f, 1.0f, -5.0f };

		// �J�����̉�]���x
		float cameraRotateSpeed = 0.5f;

		// �J�����̈ړ����x
		float cameraMoveSpeed = 2.0f;

		// �J���������ԂȂ�true
		bool isControlCamera = false;
		
	private: // ------------------------- �}�E�X�J�[�\�� --------------------------

		// �}�E�X�J�[�\���̈ʒu
		Vector2 mousePos = Vector2(0, 0);

		// �}�E�X�J�[�\���̏����ʒu
		Vector2 startMousePos = Vector2(0, 0);

	private: // ----------------------------- �`��p ------------------------------

		// �`��pFBO
		FramebufferObjectPtr fbo;
	};

} // namespace PokarinEngine

#endif // !POKARINENGINE_SCENEVIEW_H_INCLUDED
