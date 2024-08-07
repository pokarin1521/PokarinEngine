/**
* @file Camera.h
*/
#ifndef POKARINENGINE_CAMERA_H_INCLUDED
#define POKARINENGINE_CAMERA_H_INCLUDED

#include "Transform.h"

#include "../Math/Angle.h"
#include "../Math/Vector.h"

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

		Camera() = default;
		~Camera() = default;

	public: // ------------------------------ 更新 -------------------------------
		
		/// <summary>
		/// 更新
		/// </summary>
		void Update() override;

		/// <summary>
		/// GPUに情報をコピーする
		/// </summary>
		void CopyToGPU() const;

	public: // -------------------------- スカイスフィア -------------------------

		/// <summary>
		/// スカイスフィアを描画する
		/// </summary>
		void DrawSkySphere() const;

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

	private: // ---------------------------- 初期化 ------------------------------

		/// <summary>
		/// 最初の更新の直前での初期化
		/// </summary>
		void Start() override;

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

		// スカイスフィア用モデル
		StaticMeshPtr skySphere;

		// 描画先のディスプレイ番号
		GLuint targetDisplayID = 0;
	};
}

#endif // !POKARINENGINE_CAMERA_H_INCLUDED
