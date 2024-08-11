/**
* @file GameView.cpp
*/
#include "GameView.h"

#include "ImGui/imgui.h"

#include "../Scene.h"
#include "../Color.h"
#include "../SkySphere.h"
#include "../LightManager.h"
#include "../FramebufferObject.h"
#include "../CameraManager.h"

#include "../Components/Camera.h"

namespace PokarinEngine
{
	/// <summary>
	/// 初期化
	/// </summary>
	void GameView::Initialize()
	{
		fbo = FramebufferObject::Create();
	}

	/// <summary>
	/// 更新
	/// </summary>
	void GameView::Update()
	{
		// ---------------------------
		// ウィンドウを作成
		// ---------------------------

		// ウィンドウの丸みを無くす
		ImGui::PushStyleVar(ImGuiStyleVar_::ImGuiStyleVar_WindowPadding, ImVec2(0, 0));

		// シーンビュー用ウィンドウ
		ImGui::Begin("Game", nullptr,
			ImGuiWindowFlags_::ImGuiWindowFlags_NoScrollbar |
			ImGuiWindowFlags_::ImGuiWindowFlags_NoScrollWithMouse);
		{
			// FBOが作成されているか確認
			if (fbo)
			{
				// FBOのテクスチャをウィンドウに描画する
				// 描画サイズはウィンドウの大きさで設定する
				ImGui::Image(*fbo->GetTexture(), ImGui::GetWindowSize());
			}

			// ウィンドウの丸み設定を終了
			ImGui::PopStyleVar();

			// ウィンドウを終了
			ImGui::End();
		}

		// -------------------------------
		// カラーバッファをクリア
		// -------------------------------

		//fbo->ClearColor(Color::black);
	}

	/// <summary>
	/// 描画
	/// </summary>
	/// <param name="_currentScene"> 現在のシーン </param>
	void GameView::Render(const ScenePtr& currentScene)
	{
		// 0番のカメラを取得する
		// 後々エディタ画面で番号を指定できるようにする
		const Camera* camera = CameraManager::GetCamera(0);

		// カメラがないなら描画しない
		if (!camera)
		{
			return;
		}
		
		// FBOが作成されていないなら描画しない
		if (!fbo)
		{
			return;
		}

		// FBOをバインドする
		fbo->Bind();

		// カメラ情報をGPUにコピーする
		camera->CopyToGPU();

		// ライト情報をGPUにコピーする
		LightManager::CopyGPU(*camera);

		// スカイスフィアを描画する
		SkySphere::Draw(*camera);

		// 現在のシーンのゲームオブジェクトを描画する
		currentScene->DrawGameObjectAll();

		// FBOのバインドを解除する
		fbo->UnBind();
	}

} // namespace PokarinEngine