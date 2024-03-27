/**
* @file UILayout.cpp
*/
#include "UILayout.h"
#include "../Engine.h"
#include "../Scene.h"

namespace PokarinEngine
{
	/// <summary> 
	/// UIレイアウトの実行を開始する 
	/// </summary>
	void UILayout::Start()
	{
		// 最初に指定された座標を「カメラから見た座標」とする
		basePosition = GetOwner().transform->position;
	}

	/// <summary>
	/// 更新イベント
	/// </summary>
	/// <param name="deltaTime"> 
	/// 前回の更新からの経過時間(秒) 
	/// </param>
	void UILayout::Update(float deltaTime)
	{
		/* 常にカメラの方を向くように
		ゲームオブジェクトの向きを調整(ビルボードと同じ) */

		// コンポーネントの持ち主
		const GameObject& owner = GetOwner();

		// 持ち主を管理しているシーン
		const Scene& scene = owner.GetScene();

		// ゲームオブジェクトの角度をカメラの角度に合わせる
		const GameObject& mainCamera = scene.GetMainCamera();
		owner.transform->rotation.y = mainCamera.transform->rotation.y;

		/* 常にカメラの正面に位置するように
		ゲームオブジェクトの座標を調整

		回転の公式によってカメラとの相対座標を求める */

		// 角度0度の場合の座標
		Vec3 pos = basePosition;

		// Y座標の-1～+1が、
		// カメラに写る範囲の上端と下端になるように、Z座標を調整
		// 視野角が広がれば近くに、
		// 狭くなれば遠くに配置することでサイズが変わらないようにする
		pos.z = -scene.GetCameraInfo().GetFovScale();

		// 回転の計算用
		const float s = sin(mainCamera.transform->rotation.y);
		const float c = cos(mainCamera.transform->rotation.y);

		// 座標の回転 
		pos = {
			pos.x * c + pos.z * s,
			pos.y,
			pos.x * -s + pos.z * c,
		};

		// カメラとの相対座標
		owner.transform->position = mainCamera.transform->position + pos;
	}

} // namespace PokarinEngine
