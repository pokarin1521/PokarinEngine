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
	// 前方宣言
	// ----------------------

	class Engine;
	class RenderView;

	// -----------------------------
	// 型の別名を定義
	// -----------------------------

	using RenderViewPtr = std::shared_ptr<RenderView>;

	/// <summary>
	/// 描画用ビューの基底クラス
	/// </summary>
	class RenderView
	{
	public: // ---------- コンストラクタ・デストラクタ -----------

		RenderView() = default;
		virtual ~RenderView() = default;

	public: // --------------------- 制御 ------------------------

		/// <summary>
		/// 初期化
		/// </summary>
		void Initialize(Engine& engine);

		/// <summary>
		/// 更新
		/// </summary>
		virtual void Update() {}

	public: // -------------------- バインド ----------------------

		/// <summary>
		/// FBOをバインドする
		/// </summary>
		void BindFBO() const
		{
			fbo->Bind();
		}

		/// <summary>
		/// FBOのバインドを解除する
		/// </summary>
		void UnBindFBO() const
		{

			fbo->UnBind();
		}

	protected: // --------------------- 描画用 -------------------------

		// FBO
		FramebufferObjectPtr fbo;

		// 描画用テクスチャ
		ImTextureID texture = 0;
	};

} // namespace PokarinEngine

#endif // !SCENEVIEW_H_INCLUDED
