/**
* @file SkySphere.cpp
*/
#include "SkySphere.h"

#include "Mesh/Mesh.h"
#include "Shader/Shader.h"
#include "Math/Matrix.h"
#include "Components/Camera.h"

#include "ShaderConfig.h"

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
			static const GLuint progUnlit = Shader::GetProgram(Shader::ProgType::Unlit);

			// 空にライティングすると不自然なので
			// アンリットシェーダで描画
			glUseProgram(progUnlit);

			// 深度バッファへの書き込みを禁止
			glDepthMask(GL_FALSE);

			// ----------------------------------------
			// 座標変換行列をGPUにコピーする
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

			// 座標変換行列をGPUにコピー
			glProgramUniformMatrix4fv(progUnlit, ShaderConfig::Uniform::transformMatrix,
				1, GL_FALSE, &transformMatrix[0].x);

			// -----------------------------------
			// 色をGPUメモリにコピー
			// -----------------------------------

			// 色はマテリアルカラーで調整するので白を設定
			// (実際に描画される色は「オブジェクトカラー」と「マテリアルカラー」の乗算)
			static const Color color = Color::white;
			glProgramUniform4fv(progUnlit, ShaderConfig::Uniform::color, 1, &color.r);

			// -----------------------------------
			// カメラの座標をGPUにコピー
			// -----------------------------------

			camera.CopyToGPU();

			// スカイスフィアは常にカメラを中心に描画したいので、
			// カメラを一時的に原点に移動させる
			glProgramUniform3fv(progUnlit, ShaderConfig::Uniform::cameraPosition,
				1, &Vector3::zero.x);

			// -----------------------------------
			// スカイスフィアを描画する
			// -----------------------------------

			// スカイスフィアを描画する
			Mesh::Draw(skySphere, progUnlit, skySphere->GetMaterialList());

			// カメラパラメータをGPUにコピーし直す
			camera.CopyToGPU();

			// 深度バッファへの書き込みを許可
			glDepthMask(GL_TRUE);

			// 標準シェーダに戻す
			glUseProgram(Shader::GetProgram(Shader::ProgType::Standard));
		}
	}
}