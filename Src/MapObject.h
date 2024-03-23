/**
* @file MapObject.h
*/
#ifndef MAPOBJECT_H_INCLUDED
#define MAPOBJECT_H_INCLUDED

#include "Engine/Components/Component.h"

#include <vector>

namespace PokarinEngine
{
	// -------------------
	// 前方宣言
	// -------------------

	class MapObject;

	// ------------------------
	// 型の別名を定義
	// ------------------------

	using MapObjectPtr = std::shared_ptr<MapObject>;

	/**
	* マップに配置するオブジェクト
	*/
	class MapObject : public Component
	{
	public: // コンストラクタ・デストラクタ

		MapObject() = default;
		virtual ~MapObject() = default;

	public: // 分岐用

		// Action関数の実行結果
		enum class ActionResult
		{
			noRemoteAction, // リモートアクションを実行しない
			executeRemoteAction, // リモートアクションを実行する
		};

	public: // イベント制御

		/// <summary>
		/// クリックされたときに呼び出される関数
		/// </summary>
		void OnClick()
		{
			if (Action() == ActionResult::executeRemoteAction)
			{
				// 遠隔操作を実行
				InvokeRemoteAction();
			}
		}

		/// <summary>
		/// 遠隔操作を実行する
		/// </summary>
		void InvokeRemoteAction()
		{
			for (auto& e : remoteObjects)
			{
				e->RemoteAction(this);
			}
		}

		/// <summary>
		/// 遠隔操作するマップオブジェクトを追加する
		/// </summary>
		/// <param name="object"> 追加するマップオブジェクト </param>
		void AddRemoteObject(const MapObjectPtr& object)
		{
			remoteObjects.push_back(object);
		}

		/// <summary>
		/// 自分がクリックされたときに実行する仮想関数
		/// </summary>
		/// <returns> 実行結果を表す列挙体 </returns>
		virtual ActionResult Action()
		{
			return ActionResult::executeRemoteAction;
		}

		/// <summary>
		/// 関連付けられたオブジェクトが
		/// クリックされた時に呼び出される仮想関数
		/// </summary>
		/// <param name="linkedObject"> 呼び出し元のマップオブジェクト </param>
		virtual void RemoteAction(MapObject* linkedObject) {}

	private: // マップオブジェクト

		// 遠隔操作するマップオブジェクトの配列
		std::vector<MapObjectPtr> remoteObjects;
	};

} // namespace PokarinEngine

#endif // !MAPOBJECT_H_INCLUDED
