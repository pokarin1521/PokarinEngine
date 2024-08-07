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
	/// シーンビュー(シーン内の情報を描画するビュー)
	/// </summary>
	class SceneView
	{
	public: // ------------------ コンストラクタ・デストラクタ --------------------

		SceneView()
		{
			// カメラの初期位置を設定
			camera.transform.position = cameraStartPos;
		}

		~SceneView() = default;

	public: // ---------------------------- 禁止事項 ------------------------------

		/* エディタ管理がおかしくなるので禁止する */

		// コピーコンストラクタの禁止
		SceneView(const SceneView&) = delete;

		// 代入の禁止
		SceneView& operator=(const SceneView&) = delete;

	public: // ------------------------------ 制御 --------------------------------

		/// <summary>
		/// 更新
		/// </summary>
		void Update();

		/// <summary>
		/// 描画
		/// </summary>
		void Render();

	private: // -------------------------- カメラ操作 -----------------------------

		/// <summary>
		/// カメラの位置を操作する
		/// </summary>
		void CameraMoveControl();

		/// <summary>
		/// カメラの回転を操作する
		/// </summary>
		void CameraRotateControl();

	private: // ----------------------------- カメラ ------------------------------

		// カメラ
		Camera camera;

		// カメラの初期位置
		// 生成したオブジェクトが見える位置にする
		const Vector3 cameraStartPos = { 0.0f, 1.0f, -5.0f };

		// カメラの回転速度
		float cameraRotateSpeed = 0.5f;

		// カメラの移動速度
		float cameraMoveSpeed = 2.0f;

		// カメラ操作状態ならtrue
		bool isControlCamera = false;
		
	private: // ------------------------- マウスカーソル --------------------------

		// マウスカーソルの位置
		Vector2 mousePos = Vector2(0, 0);

		// マウスカーソルの初期位置
		Vector2 startMousePos = Vector2(0, 0);

	private: // ----------------------------- 描画用 ------------------------------

		// 描画用FBO
		FramebufferObjectPtr fbo;
	};

} // namespace PokarinEngine

#endif // !POKARINENGINE_SCENEVIEW_H_INCLUDED
