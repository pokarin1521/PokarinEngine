/**
* @file SceneView.h
*/
#ifndef RENDERVIEW_H_INCLUDED
#define RENDERVIEW_H_INCLUDED

#include "glad/glad.h"

#include "ImGui/imgui.h"

#include "../FramebufferObject.h"

#include <memory>

namespace PokarinEngine
{
	// ----------------------
	// �O���錾
	// ----------------------

	class Engine;
	class RenderView;

	// -----------------------------
	// �^�̕ʖ����`
	// -----------------------------

	using RenderViewPtr = std::shared_ptr<RenderView>;

	/// <summary>
	/// �`��p�r���[�̊��N���X
	/// </summary>
	class RenderView
	{
	public: // ---------- �R���X�g���N�^�E�f�X�g���N�^ -----------

		RenderView() = default;
		virtual ~RenderView() = default;

	public: // --------------------- ���� ------------------------

		/// <summary>
		/// ������
		/// </summary>
		void Initialize(Engine& engine);

		/// <summary>
		/// �X�V
		/// </summary>
		virtual void Update() {}

	public: // -------------------- �o�C���h ----------------------

		/// <summary>
		/// FBO���o�C���h����
		/// </summary>
		void BindFBO() const
		{
			fbo->Bind();
		}

		/// <summary>
		/// FBO�̃o�C���h����������
		/// </summary>
		void UnBindFBO() const
		{

			fbo->UnBind();
		}

	protected: // --------------------- �`��p -------------------------

		// FBO
		FramebufferObjectPtr fbo;

		// �`��p�e�N�X�`��
		ImTextureID texture = 0;
	};

} // namespace PokarinEngine

#endif // !SCENEVIEW_H_INCLUDED
