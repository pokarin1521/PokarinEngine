/**
* @file Camera.cpp
*/
#include "Camera.h"

#include "ImGui/imgui.h"

#include "../GameObject.h"
#include "../Window.h"
#include "../TextureGetter.h"
#include "../Shader/Shader.h"
#include "../Configs/ShaderConfig.h"

#include "../Mesh/Mesh.h"

namespace PokarinEngine
{
	/// <summary>
	/// 更新
	/// </summary>
	void Camera::Update()
	{
		// 持ち主であるゲームオブジェクトの位置・回転角度
		TransformPtr owner = GetOwnerObject().transform;

		// 位置・回転角度を更新する
		owner->position = transform.position;
		owner->rotation = transform.rotation;
	}

	/// <summary>
	/// GPUに情報をコピーする
	/// </summary>
	void Camera::CopyToGPU()
	{
		// ----------------------------
		// 情報を取得する
		// ----------------------------
		
		// カメラの位置
		Vector3 position = transform.position;

		// 今は左手座標系の値になっていて、
		// OpenGLは右手座標系なので、右手座標系にする
		position.z *= -1;

		// カメラの回転角度
		// オブジェクトはカメラの回転方向とは逆に動くことになるので、符号を逆にする
		Vector3 rotation = -transform.rotation;

		// ----------------------------
		// GPUにコピーする
		// ----------------------------

		// 全てのシェーダプログラム
		const auto& allProg = Shader::GetAllProgram();

		// アスペクト比
		const float aspectRatio = Window::GetAspectRatio(WindowID::Main);

		// 全てのシェーダプログラムにコピーする
		for (const auto& [type, prog] : allProg)
		{
			// アスペクト比と視野角による拡大率を設定
			// GPU側での除算を避けるため、逆数にして渡す
			glProgramUniform2f(prog, UniformLocation::aspectRatioAndScaleFov,
				1 / aspectRatio, inverseFovScale);

			// カメラの位置をGPUにコピー
			glProgramUniform3fv(prog, UniformLocation::cameraPosition, 1, &position.x);

			// カメラの回転角度をGPUにコピー
			glProgramUniform3fv(prog, UniformLocation::cameraRotation, 1, &rotation.x);
		}
	}

	/// <summary>
	/// スカイスフィアを描画する
	/// </summary>
	void Camera::DrawSkySphere()
	{
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
		const float scale = drawRange.far * 0.95f / skySphereRadius;

		// 座標変換行列
		// 移動も回転もしないので拡大率だけ
		const Matrix4x4 transformMatrix = {
			{ scale,     0,     0,     0 },
			{     0, scale,     0,     0 },
			{     0,     0, scale,     0 },
			{     0,     0,     0,     1 },
		};

		// 座標変換行列をGPUにコピー
		glProgramUniformMatrix4fv(progUnlit, UniformLocation::transformMatrix,
			1, GL_FALSE, &transformMatrix[0].x);

		// -----------------------------------
		// 色をGPUメモリにコピー
		// -----------------------------------

		// 色はマテリアルカラーで調整するので白を設定
		// (実際に描画される色は「オブジェクトカラー」と「マテリアルカラー」の乗算)
		static const Color color = { 1, 1, 1, 1 };
		glProgramUniform4fv(progUnlit, UniformLocation::color, 1, &color.r);

		// -----------------------------------
		// カメラの座標をGPUにコピー
		// -----------------------------------

		// スカイスフィアは常にカメラを中心に描画したいので、
		// カメラを一時的に原点に移動させる
		glProgramUniform3fv(progUnlit, UniformLocation::cameraPosition,
			1, &Vector3::zero.x);

		// -----------------------------------
		// スカイスフィアを描画する
		// -----------------------------------

		// スカイスフィアを描画する
		Mesh::Draw(skySphere, progUnlit, skySphere->GetMaterialList());

		// カメラパラメータをGPUにコピーし直す
		CopyToGPU();

		// 深度バッファへの書き込みを許可
		glDepthMask(GL_TRUE);

		// 標準シェーダに戻す
		glUseProgram(Shader::GetProgram(Shader::ProgType::Standard));
	}

	/// <summary>
	/// 情報を編集できるように表示する
	/// </summary>
	void Camera::InfoEditor()
	{
		ImGui::Text("Camera Information");
	}

	/// <summary>
	/// コンポーネントの情報をJson型に格納する
	/// </summary>
	/// <param name="[out] Json"> 情報を格納するJson型 </param>
	void Camera::ToJson(Json& data) const
	{

	}

	/// <summary>
	/// コンポーネントの情報をJson型から取得する
	/// </summary>
	/// <param name="[out] data"> 情報を格納しているJson型 </param>
	void Camera::FromJson(const Json& data)
	{

	}
}