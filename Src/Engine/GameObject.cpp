/**
* @file GamaObject.cpp
*/
#include "GameObject.h"

#include "Json/Json.h"

#include "Components/Colliders/Collider.h"

#include "Math/Matrix.h"

#include "NodeScript/NodeScript.h"
#include "NodeScript/NodeEditor.h"

#include "Scene.h"
#include "Random.h"

#include <algorithm>
#include <filesystem>

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

		// ------------------------------------- 
		// コンポーネント配列から削除 
		// -------------------------------------
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

			// 削除するコンポーネント配列
			// 削除時の処理でコンポーネントが追加される可能性があるので、
			// 削除するコンポーネントを移動させる
			std::vector<ComponentPtr> destroyList(
				std::move_iterator(destroyStart), std::move_iterator(componentList.end()));

			// 破棄予定のコンポーネントを削除
			componentList.erase(destroyStart, componentList.end());

			// コンポーネントの削除時の処理を実行
			for (auto& destroyComponent : destroyList)
			{
				destroyComponent->OnDestroy();
			}
		}

		// ------------------------------------- 
		// コライダー配列から削除 
		// -------------------------------------
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
			transform = AddComponent<Transform>().first;
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
	/// <param name="[in] isPlayGame"> 作成中のゲームが再生中ならtrue </param>
	void GameObject::Update(bool isPlayGame)
	{
		// コンポーネントの更新
		UpdateComponent(isPlayGame);

		// ゲームが再生されたら実行する
		if (isPlayGame)
		{
			// ノードエディタで設定したノードの実行
			nodeEditor->Run();
		}

		// 座標変換行列の更新
		UpdateMatrix();
	}

	/// <summary>
	/// ゲームオブジェクトにあるコンポーネントを更新する
	/// </summary>
	/// <param name="[in] isPlayGame"> 作成中のゲームが再生中ならtrue </param>
	void GameObject::UpdateComponent(bool isPlayGame)
	{
		// コンポーネントのStartを１度だけ実行
		// 途中で追加されることを想定して、Update内で実行
		for (auto& component : componentList)
		{
			component->Initialize();
		}

		// コンポーネントを更新
		for (auto& component : componentList)
		{
			// 更新
			component->Update();

			// ゲーム再生中の更新処理
			if (isPlayGame)
			{
				component->Update_PlayGame();
			}
		}

		// コンポーネントの削除を確定させる
		RemoveDestroyedComponent();
	}

	/// <summary>
	/// ゲームオブジェクトの座標変換行列を更新する
	/// </summary>
	void GameObject::UpdateMatrix()
	{
		// ----------------------------------
		// 自身の座標変換行列を求める
		// ----------------------------------

		// オブジェクトの座標
		// 左手座標系の値なので右手座標系にする
		Vector3 position = transform->position;
		position.z *= -1;

		// 自身の座標変換行列
		Matrix4x4 transformMatrix = Matrix4x4::CreateTransformMatrix(
			position, transform->rotation, transform->scale);

		// 自身の法線変換行列
		Matrix3x3 normalMatrix = Matrix3x3::CreateRotationMatrix(transform->rotation);

		// 座標変換行列を設定
		transform->transformMatrix = transformMatrix;

		// 法線変換行列を設定
		transform->normalMatrix = normalMatrix;

		// ------------------------------------
		// 親の座標変換行列を反映する
		// ------------------------------------

		// 親をたどっていく
		for (Transform* parent = transform->GetParent(); parent; parent = parent->GetParent())
		{
			// 親の座標変換行列
			Matrix4x4 parentTransformMatrix = parent->transformMatrix;

			// 親の法線変換行列
			Matrix3x3 parentNormalMatrix = parent->normalMatrix;

			// 親の変換行列を掛け合わせる
			transformMatrix *= parentTransformMatrix;
			normalMatrix *= parentNormalMatrix;
		}
	}

	/// <summary>
	/// 削除
	/// </summary>
	void GameObject::OnDestroy()
	{
		// コンポーネントを削除
		for (auto& component : componentList)
		{
			component->OnDestroy();
		}

		// ノードエディタを閉じる
		NodeScript::CloseNodeEditor(nodeEditor);
	}

	/// <summary>
	/// コンポーネントをエディタに表示する
	/// </summary>
	void GameObject::RenderComponent()
	{
		// コンポーネントを表示
		for (auto& component : componentList)
		{
			component->RenderInfo();
		}
	}

	/// <summary>
	/// ノードエディタを開く
	/// </summary>
	void GameObject::OpenNodeEditor() const
	{
		NodeScript::OpenNodeEditor(nodeEditor);
	}

	/// <summary>
	/// ゲームオブジェクトの情報を保存する
	/// </summary>
	/// <param name="[in] sceneFolderName"> シーンフォルダ名 </param>
	void GameObject::SaveGameObject(const std::string& sceneFolderName) const
	{
		// ----------------------------------
		// フォルダを作成する
		// ----------------------------------

		// 保存先のフォルダ名
		std::string folderName = sceneFolderName + "/" + std::to_string(id);

		// フォルダが存在しない場合は作成する
		std::filesystem::create_directories(folderName);

		// ----------------------------------------
		// コンポーネントの情報を保存する
		// ----------------------------------------

		for (auto& component : componentList)
		{
			//component->SaveInfo(folderName);
		}
	}

	/// <summary>
	/// コンポーネント識別番号を取得する
	/// </summary>
	/// <returns> 重複しない識別番号 </returns>
	int GameObject::GetSingleComponentID()
	{
		// コンポーネント識別番号
		int componentID = Random::Range(INT_MIN, INT_MAX);

		// コンポーネント識別番号を追加
		// 重複している場合は、再取得する
		while (!componentIDList.emplace(componentID).second)
		{
			componentID = Random::Range(INT_MIN, INT_MAX);
		}

		return componentID;
	}

} // namespace PokarinEngine