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
		// コンポーネントを持っていなければ何もしない
		if (componentList.empty())
		{
			return;
		}

		// --------------------------------------- 
		// コンポーネントを削除
		// --------------------------------------- 

		// --------------- コンポーネント配列から削除 ------------------
		{
			/* 削除時の処理を実行出来るようにstable_partitionを使う
			(remove_ifだと削除される可能性がある)

			破棄予定のないコンポーネントを配列の前側に、
			破棄予定のあるコンポーネントを後ろ側に集める
			std::stable_partition(範囲の先頭, 範囲の終端, 振り分け条件) */

			// 破棄予定のコンポーネントの先頭イテレータ
			auto destroyStart = std::stable_partition(
				componentList.begin(), componentList.end(),
				[](const auto& e) {return !e->IsDestroyed(); });

			// 削除時の処理を実行
			for (auto& itr = destroyStart; itr != componentList.end(); ++itr)
			{
				auto& destroyComponent = *itr;
				destroyComponent->OnDestroy();
			}

			// 破棄予定のコンポーネントを削除
			componentList.erase(destroyStart, componentList.end());
		}

		// ----------------- コライダー配列から削除 --------------------
		{
			// コライダーはコライダー配列にも登録されているので、両方から削除する必要がある
			// 削除するだけなので、remove_ifを使う
			const auto destroyStart = std::remove_if(
				colliderList.begin(), colliderList.end(),
				[](const auto& p) {return p->IsDestroyed(); });

			// 破棄予定のものを削除
			colliderList.erase(destroyStart, colliderList.end());
		}
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
			nodeEditor = std::make_shared<NodeEditor>(*this);
		}
	}

	/// <summary>
	/// 更新
	/// </summary>
	void GameObject::Update()
	{
		// コンポーネントのStartを１度だけ実行
		// 途中で追加されることを想定して、Update内で実行
		for (auto& e : componentList)
		{
			if (!e->isStarted)
			{
				e->Start();
				e->isStarted = true;
			}
		}

		// コンポーネントを更新
		for (auto& e : componentList)
		{
			e->Update();
		}

		nodeEditor->Run();

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
		for (auto& e : componentList)
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
		for (auto& e : componentList)
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