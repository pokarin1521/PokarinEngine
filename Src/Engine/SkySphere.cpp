/**
* @file SkySphere.cpp
*/
#include "SkySphere.h"

#include "Mesh/Mesh.h"
#include "Shader/Shader.h"
#include "Math/Matrix.h"
#include "Components/Camera.h"

namespace PokarinEngine
{
	/// <summary>
	/// スカイスフィア管理用
	/// </summary>
	namespace SkySphere
	{
		// -------------------------
		// 変数
		// -------------------------

		// スカイスフィア用メッシュ
		StaticMeshPtr skySphere;

		// -------------------------
		// 関数
		// -------------------------

		/// <summary>
		/// スカイスフィアを描画する
		/// </summary>
		void Draw(const Camera& camera)
		{
			// ----------------------------------------------
			// スカイスフィア用メッシュを取得する
			// ----------------------------------------------

			// スカイスフィア用メッシュを取得済みならtrue
			static bool isGetMesh = false;

			// まだスタティックメッシュを取得していない
			if (!isGetMesh)
			{
				// スカイスフィア用メッシュを取得する
				skySphere = Mesh::GetStaticMesh("Res/MeshData/SkySphere/SkySphere.obj");

				// 取得済みにする
				isGetMesh = true;
			}

			// --------------------------------------------------------
			// スカイスフィア用モデルがない場合は描画しない
			// --------------------------------------------------------

			if (!skySphere)
			{
				return;
			}

			// -------------------------------------
			// 使用するシェーダを指定する
			// -------------------------------------

			// ライティング無しのシェーダ
			static const Shader::ProgType progUnlit = Shader::ProgType::Unlit;

			// 空にライティングすると不自然なので
			// アンリットシェーダで描画
			Shader::UseProgram(progUnlit);

			// 深度バッファへの書き込みを禁止
			glDepthMask(GL_FALSE);

			// ----------------------------------------
			// 座標変換行列をシェーダに設定する
			// ----------------------------------------

			/* スカイスフィアは移動と回転はしないので拡大率だけを設定する

			スカイスフィアは最も遠くに描画される物体なので、
			カメラが描画できる範囲のうち、できるだけ遠い位置に描画したい

			現在の最大描画範囲を使いたいが
			スカイスフィアもポリゴンモデルなので微妙な凹凸がある
			凹凸が範囲からはみ出さないように*/

			// スカイスフィアの半径
			static const float skySphereRadius = 0.5f;

			// 拡大率
			// 最大描画範囲の95%の位置に描画できるように設定
			const float scale = camera.GetDrawRange().far * 0.95f / skySphereRadius;

			// 座標変換行列
			// 移動も回転もしないので拡大率だけ
			const Matrix4x4 transformMatrix = {
				{ scale,     0,     0,     0 },
				{     0, scale,     0,     0 },
				{     0,     0, scale,     0 },
				{     0,     0,     0,     1 },
			};

			// 座標変換行列をシェーダに設定する
			Shader::SetMatrix4x4(progUnlit, UniformMatrix4x4::transformMatrix, transformMatrix);

			// -----------------------------------
			// 色をシェーダに設定する
			// -----------------------------------

			// 色はマテリアルカラーで調整するので白を設定
			// (実際に描画される色は「オブジェクトカラー」と「マテリアルカラー」の乗算)
			Shader::SetVector4(progUnlit, UniformVector4::color, Color::white);

			// -------------------------------------
			// カメラの座標をシェーダに設定する
			// -------------------------------------

			// スカイスフィアは常にカメラを中心に描画したいので、
			// カメラを一時的に原点に移動させる
			Shader::SetVector3(progUnlit, UniformVector3::cameraPosition, Vector3::zero);

			// -----------------------------------
			// スカイスフィアを描画する
			// -----------------------------------

			// スカイスフィアを描画する
			Mesh::Draw(skySphere, progUnlit, skySphere->GetMaterialList());

			// カメラの位置を戻したいので
			// カメラパラメータをシェーダに設定し直す
			camera.SetToShader();

			// 深度バッファへの書き込みを許可
			glDepthMask(GL_TRUE);

			// 標準シェーダに戻す
			Shader::UseProgram(progUnlit);
		}
	}
}