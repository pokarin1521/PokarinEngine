/**
* @file GamaObject.cpp
*/
#include "GameObject.h"

#include "NodeScript/NodeScript.h"
#include "NodeScript/NodeEditor.h"

#include "Scene.h"

#include <algorithm>

namespace PokarinEngine
{
	/// <summary>
	/// ゲームオブジェクトから削除予定のコンポーネントを削除する 
	/// </summary>
	void GameObject::RemoveDestroyedComponent()
	{
		if (components.empty())
		{
			return; // コンポーネントを持っていなければ何もしない
		}

		// -------- 削除処理がしやすいように破棄予定の有無で振り分け ---------

		/* 移動させるためなのでstable_partitionを使う

		破棄予定のないコンポーネントを配列の前側に、
		破棄予定のあるコンポーネントを後ろ側に集める
		std::stable_partition(範囲の先頭, 範囲の終端, 振り分け条件) */

		// iterには破棄予定のあるコンポーネントの先頭イテレータが入る
		const auto iter = std::stable_partition(
			components.begin(), components.end(),
			[](const auto& e) {return !e->IsDestroyed(); });

		// 破棄予定のコンポーネントを別の配列に移動
		std::vector<ComponentPtr> destroyList(
			std::move_iterator(iter),
			std::move_iterator(components.end()));

		// 移動させて空になった配列を削除
		components.erase(iter, components.end());

		// コライダーはcollidersにも登録されているので、両方から削除する必要がある
		// 削除するためなので、remove_ifを使う
		const auto iter2 = std::remove_if(
			colliders.begin(), colliders.end(),
			[](const auto& p) {return p->IsDestroyed(); });

		// 破棄予定のものを削除
		colliders.erase(iter2, colliders.end());

		// 破棄予定のコンポーネントのOnDestroyを実行
		for (auto& e : destroyList)
		{
			e->OnDestroy();
		}

		// ここで実際にコンポーネントが削除される(destroyListの寿命が終わるため)
	}

	/// <summary>
	/// 初期化
	/// </summary>
	void GameObject::Initialize()
	{
		// Transformコンポーネントを追加
		if (!transform)
		{
			transform = AddComponent<Transform>();
		}

		// ノードエディタを作成
		if (!nodeEditor)
		{
			nodeEditor = NodeEditor::Create(this);
		}
	}

	/// <summary>
	/// 更新
	/// </summary>
	/// <param name="deltaTime"> 前回の更新からの経過時間(秒) </param>
	void GameObject::Update(float deltaTime)
	{
		// コンポーネントのStartを１度だけ実行
		// 途中で追加されることを想定して、Update内で実行
		for (auto& e : components)
		{
			if (!e->isStarted)
			{
				e->Start();
				e->isStarted = true;
			}
		}

		// コンポーネントを更新
		for (auto& e : components)
		{
			e->Update(deltaTime);
		}

		// コンポーネントの削除を確定させる
		RemoveDestroyedComponent();
	}

	/// <summary>
	/// 衝突時の処理
	/// </summary>
	/// <param name="self"> 衝突したコンポーネント(自分) </param>
	/// <param name="other"> 衝突したコンポーネント(相手) </param>
	void GameObject::OnCollision(const ComponentPtr& self, const ComponentPtr& other)
	{
		for (auto& e : components)
		{
			e->OnCollision(self, other);
		}
	}

	/// <summary>
	/// 削除
	/// </summary>
	void GameObject::OnDestroy()
	{
		// コンポーネントを削除
		for (auto& e : components)
		{
			e->OnDestroy();
		}
	}

	/// <summary>
	/// ノードエディタを開く
	/// </summary>
	void GameObject::OpenNodeEditor() const
	{
		NodeScript::OpenNodeEditor(nodeEditor);
	}

} // namespace PokarinEngine