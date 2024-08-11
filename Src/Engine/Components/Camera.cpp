/**
* @file Camera.cpp
*/
#include "Camera.h"

#include "glad/glad.h"

#include "ImGui/imgui.h"

#include "../GameObject.h"
#include "../Window.h"
#include "../TextureGetter.h"
#include "../Shader/Shader.h"

#include "../Mesh/Mesh.h"

#include "../ShaderConfig.h"

#include "../CameraManager.h"

namespace PokarinEngine
{
	/// <summary>
	/// カメラを追加するコンストラクタ
	/// </summary>
	Camera::Camera()
	{
		// ゲームオブジェクトと同期できなかったとき用
		transform = std::make_shared<Transform>();
	}

	/// <summary>
	/// カメラを削除するデストラクタ
	/// </summary>
	Camera::~Camera()
	{
		CameraManager::DestroyCamera(*this);
	}

	/// <summary>
	/// ゲームオブジェクトに追加された時の初期化
	/// </summary>
	void Camera::Awake()
	{
		// 持ち主であるゲームオブジェクトのTransformと同期する
		transform = GetOwnerObject().transform;

		// カメラを追加する
		CameraManager::AddCamera(*this);
	}

	/// <summary>
	/// GPUに情報をコピーする
	/// </summary>
	void Camera::CopyToGPU() const
	{
		// ----------------------------
		// 情報を取得する
		// ----------------------------
		
		// カメラの位置
		Vector3 position = transform->position;

		// 今は左手座標系の値になっていて、
		// OpenGLは右手座標系なので、右手座標系にする
		position.z *= -1;

		// カメラの回転角度
		// オブジェクトはカメラの回転方向とは逆に動くことになるので、符号を逆にする
		Vector3 rotation = -transform->rotation;

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
			glProgramUniform2f(prog, ShaderConfig::Uniform::aspectRatioAndScaleFov,
				1 / aspectRatio, inverseFovScale);

			// カメラの位置をGPUにコピー
			glProgramUniform3fv(prog, ShaderConfig::Uniform::cameraPosition, 1, &position.x);

			// カメラの回転角度をGPUにコピー
			glProgramUniform3fv(prog, ShaderConfig::Uniform::cameraRotation, 1, &rotation.x);
		}
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
	/// <param name="[in] data"> 情報を格納しているJson型 </param>
	void Camera::FromJson(const Json& data)
	{

	}
}