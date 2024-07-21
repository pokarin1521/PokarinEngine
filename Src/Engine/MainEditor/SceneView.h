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
	/// シーンビュー(シーン内の情報を描画するビュー)
	/// </summary>
	class SceneView : public RenderView
	{
	public: // ----------------- コンストラクタ・デストラクタ ---------------------

		SceneView()
		{
			// カメラの初期位置を設定
			sceneCamera.position = cameraStartPos;
		}

		~SceneView() = default;

	public: // ------------------------------ 制御 --------------------------------

		/// <summary>
		/// 更新
		/// </summary>
		void Update() override;

	public: // --------------------------- カメラ取得 -----------------------------

		/// <summary>
		/// シーンビュー用カメラを取得する
		/// </summary>
		/// <returns> シーンビュー用カメラ </returns>
		const Transform& GetCamera() const
		{
			return sceneCamera;
		}

	private: // -------------------------- カメラ操作 -----------------------------

		/// <summary>
		/// カメラの位置を操作する
		/// </summary>
		void CameraMoveControl();

		/// <summary>
		/// カメラの回転を操作する
		/// </summary>
		void CameraRotateControl();

	private: // ------------------------------ 情報 -------------------------------

		// シーンビュー用カメラ
		Transform sceneCamera;

		// カメラの初期位置
		const Vector3 cameraStartPos = { 0.0f, 1.0f, -5.0f };

		// 背景色
		Color backGround = { 0.3f, 0.3f, 0.3f, 1.0f };

		// マウスカーソルの位置
		Vector2 mousePos = Vector2(0, 0);

		// マウスカーソルの初期位置
		Vector2 startMousePos = Vector2(0, 0);

		// カメラの回転速度
		float cameraRotateSpeed = 0.5f;

		// カメラの移動速度
		float cameraMoveSpeed = 1.0f;

		// カメラ操作状態ならtrue
		bool isControlCamera = false;
	};

} // namespace PokarinEngine

#endif // !SCENEVIEW_H_INCLUDED
