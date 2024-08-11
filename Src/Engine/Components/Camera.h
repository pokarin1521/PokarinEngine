/**
* @file Camera.h
*/
#ifndef POKARINENGINE_CAMERA_H_INCLUDED
#define POKARINENGINE_CAMERA_H_INCLUDED

#include "Transform.h"

#include "../Math/Angle.h"
#include "../Math/Vector.h"

#include "../UsingNames/UsingStaticMesh.h"

#include <memory>

namespace PokarinEngine
{
	/// <summary>
	/// カメラ用コンポーネント
	/// </summary>
	class Camera : public Component
	{
	public: // ------------------ コンストラクタ・デストラクタ -------------------

		/// <summary>
		/// カメラを追加するコンストラクタ
		/// </summary>
		Camera();

		/// <summary>
		/// カメラを削除するデストラクタ
		/// </summary>
		~Camera();

	public: // ----------------------------- コピー ------------------------------

		/// <summary>
		/// GPUに情報をコピーする
		/// </summary>
		void CopyToGPU() const;

	public: // ---------------------------- 描画範囲 -----------------------------

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

		/// <summary>
		/// 描画範囲を取得する
		/// </summary>
		/// <returns> 描画範囲 </returns>
		const DrawRange& GetDrawRange() const
		{
			return drawRange;
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
		/// <param name="[in] data"> 情報を格納しているJson型 </param>
		void FromJson(const Json& data) override;

	public: // ------------------------- 位置・回転角度 --------------------------

		// 位置・回転角度
		// 拡大率は使わない
		TransformPtr transform;

	private: // ---------------------------- 初期化 ------------------------------

		/// <summary>
		/// ゲームオブジェクトに追加された時の初期化
		/// </summary>
		void Awake() override;

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

		// 描画範囲
		DrawRange drawRange;

		// スカイスフィア用モデル
		StaticMeshPtr skySphere;
	};
}

#endif // !POKARINENGINE_CAMERA_H_INCLUDED
