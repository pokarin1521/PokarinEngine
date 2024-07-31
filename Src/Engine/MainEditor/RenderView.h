/**
* @file SceneView.h
*/
#ifndef POKARINENGINE_RENDERVIEW_H_INCLUDED
#define POKARINENGINE_RENDERVIEW_H_INCLUDED

#include "glad/glad.h"

#include "ImGui/imgui.h"

#include "../Components/Camera.h"

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
		void Initialize();

		/// <summary>
		/// �X�V
		/// </summary>
		virtual void Update() {}
	};

} // namespace PokarinEngine

#endif // !POKARINENGINE_RENDERVIEW_H_INCLUDED
