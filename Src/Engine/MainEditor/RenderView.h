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
		void Initialize();

		/// <summary>
		/// 更新
		/// </summary>
		virtual void Update() {}
	};

} // namespace PokarinEngine

#endif // !POKARINENGINE_RENDERVIEW_H_INCLUDED
