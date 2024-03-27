/**
* @file Camera.h
*/
#ifndef CAMERA_H_INCLUDED
#define CAMERA_H_INCLUDED

#include "Component.h"

#include "../VecMath.h"

#include <memory>

namespace PokarinEngine
{
	class Camera;

	using CameraPtr = std::shared_ptr<Camera>;

	/// <summary>
	/// カメラ用コンポーネント
	/// </summary>
	class Camera : public Component
	{
	public: // ------------------ コンストラクタ・デストラクタ -------------------

		Camera() = default;
		virtual ~Camera() = default;

	public: // -------------------------- 視野角の取得 ---------------------------

		/// <summary>
		/// 垂直視野角を取得
		/// </summary>
		/// <returns> 垂直視野角(度数法) </returns>
		float GetFovY() const { return degFovY; }

		/// <summary>
		/// 視野角による拡大率を取得
		/// </summary>
		/// <returns> 視野角による拡大率の逆数 </returns>
		float GetFovScale() const { return fovScale; }

	public: // -------------------------- 視野角の設定 ---------------------------

		/// <summary>
		/// 垂直視野角を設定する
		/// </summary>
		/// <param name="fovY"> 設定する垂直視野角(度数法) </param>
		void SetFovY(float fovY)
		{
			// 垂直視野角(度数法)を設定
			degFovY = fovY;

			// 弧度法に変換
			radFovY = Radians(fovY);

			// 視野角による拡大率の逆数
			fovScale = 1 / tan(radFovY / 2);
		}

	public: // --------------------------- エディタ用 ----------------------------

		/// <summary>
		/// エディタに情報を表示する
		/// </summary>
		void RenderEditor() override;

	private: // ---------------------------- 視野角 ------------------------------

		// 垂直視野角(度数法)
		float degFovY = 60;

		// 垂直視野角(弧度法)
		float radFovY = Radians(degFovY);

		// 視野角による拡大率の逆数
		// (視野角による拡大率は常にこの形で使うので、あらかじめ逆数にしておく)
		float fovScale = 1 / tan(radFovY / 2);
	};
}

#endif // !CAMERA_H_INCLUDED
