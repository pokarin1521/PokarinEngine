/**
* @file Camera.h
*/
#ifndef POKARINENGINE_CAMERA_H_INCLUDED
#define POKARINENGINE_CAMERA_H_INCLUDED

#include "Transform.h"

#include "../Math/Angle.h"
#include "../Math/Vector.h"

#include "../FramebufferObject.h"

#include "../Mesh/StaticMesh.h"

#include <memory>

namespace PokarinEngine
{
	/// <summary>
	/// カメラ用コンポーネント
	/// </summary>
	class Camera : public Component
	{
	public: // ------------------ コンストラクタ・デストラクタ -------------------

		// ゲームオブジェクト以外でも使うのでコンストラクタでFBOを作成する
		// FBOの作成だけならコンストラクタでも問題ない
		Camera() { fbo = FramebufferObject::Create(); }
		~Camera() = default;

	public: // ------------------------------ 制御 -------------------------------

		/// <summary>
		/// 更新
		/// </summary>
		void Update();

		/// <summary>
		/// GPUに情報をコピーする
		/// </summary>
		void CopyToGPU();

		/// <summary>
		/// スカイスフィアを描画する
		/// </summary>
		void DrawSkySphere();

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
		float GetFovScale() const { return inverseFovScale; }

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
			inverseFovScale = 1 / tan(radFovY / 2);
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

	public: // ------------------------- FBOのテクスチャ -------------------------

		/// <summary>
		/// FBOが持っているテクスチャの識別番号を取得する
		/// </summary>
		/// <returns> FBOが持っているテクスチャの識別番号 </returns>
		GLuint GetTextureID() const
		{
			return fbo->GetTextureID();
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
		void FromJson(const Json& data) override;

	public: // ------------------------- 位置・回転角度 --------------------------

		// 位置・回転角度
		// 拡大率は使わない
		Transform transform;

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
		// 視野角による拡大率は常にこの形で使うので、あらかじめ逆数にしておく
		float inverseFovScale = 1 / tan(radFovY / 2);

	private: // ----------------------------- 描画用 -----------------------------

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

		// 描画用FBO
		FramebufferObjectPtr fbo;

		// スカイスフィア用モデル
		StaticMeshPtr skySphere;
	};
}

#endif // !POKARINENGINE_CAMERA_H_INCLUDED
