/**
* @file Camera.h
*/
#ifndef CAMERA_H_INCLUDED
#define CAMERA_H_INCLUDED

#include "Component.h"

#include "../Math/Angle.h"

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
		~Camera() = default;

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

		/// <summary>
		/// 垂直視野角を設定する
		/// </summary>
		/// <param name="[in] fovY"> 設定する垂直視野角(度数法) </param>
		void SetFovY(float fovY)
		{
			// 垂直視野角(度数法)を設定
			degFovY = fovY;

			// 弧度法に変換
			radFovY = Radians(fovY);

			// 視野角による拡大率の逆数
			fovScale = 1 / tan(radFovY / 2);
		}

	public: // ------------------------- 描画範囲の取得 --------------------------

		/// <summary>
		/// 最小描画範囲を取得する
		/// </summary>
		/// <returns> 描画範囲の最小値 </returns>
		float GetDrawNear() const
		{
			return drawRange.near;
		}

		/// <summary>
		/// 最大描画範囲を取得する
		/// </summary>
		/// <returns> 描画範囲の最大値 </returns>
		float GetDrawFar() const
		{
			return drawRange.far;
		}

	public: // ------------------------------ Json -------------------------------

		/// <summary>
		/// コンポーネントの情報をJson型に格納する
		/// </summary>
		/// <param name="[out] Json"> 情報を格納するJson型 </param>
		void ToJson(Json& data) const override;

		/// <summary>
		/// コンポーネントの情報をJson型から取得する
		/// </summary>
		/// <param name="[out] data"> 情報を格納しているJson型 </param>
		void FromJson(const Json& data) override {}

	private: // -------------------------- エディタ用 ----------------------------

		/// <summary>
		/// 情報を編集できるように表示する
		/// </summary>
		void InfoEditor() override;

	private: // ----------------------------- 視野角 -----------------------------

		// 垂直視野角(度数法)
		float degFovY = 60;

		// 垂直視野角(弧度法)
		float radFovY = Radians(degFovY);

		// 視野角による拡大率の逆数
		// (視野角による拡大率は常にこの形で使うので、あらかじめ逆数にしておく)
		float fovScale = 1 / tan(radFovY / 2);

	private: // ---------------------------- 描画範囲 ----------------------------

		/// <summary>
		/// 描画範囲
		/// </summary>
		struct DrawRange
		{
			// 最小描画範囲
			float near = 0.35f;

			// 最大描画範囲
			float far = 1000.0f;
		};

		// 描画範囲
		DrawRange drawRange;
	};
}

#endif // !CAMERA_H_INCLUDED
