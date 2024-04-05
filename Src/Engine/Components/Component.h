/**
* @file Component.h
*/
#ifndef COMPONENT_H_INCLUDED
#define COMPONENT_H_INCLUDED

#include "ImGui/imgui.h"

#include <string>
#include <memory>

namespace PokarinEngine
{
	// ------------------
	// 前方宣言
	// ------------------

	class GameObject;
	class Component;

	// -----------------------
	// 型の別名を定義
	// -----------------------

	using GameObjectPtr = std::shared_ptr<GameObject>;
	using ComponentPtr = std::shared_ptr<Component>;

	/// <summary>
	/// コンポーネント基底クラス
	/// </summary>
	class Component
	{
		// エンジンクラスに情報公開
		friend GameObject;

	public: // -------------------- コンストラクタ・デストラクタ ----------------------

		Component() = default;
		virtual ~Component() = default;

	public: // ---------------------- コンポーネントの情報管理 ------------------------

		/// <summary>
		/// コンポーネントの所有者を取得する(読み取り専用)
		/// </summary>
		/// <returns> 所有者の参照 </returns>
		const GameObject& GetOwner() const { return *owner; }

		/// <summary>
		/// コンポーネントの所有者を取得する
		/// </summary>
		/// <returns> 所有者の参照 </returns>
		GameObject& GetOwner() { return *owner; }

		/// <summary>
		/// コンポーネントをゲームオブジェクトから削除する
		/// </summary>
		void Destroy() { isDestroyed = true; }

		/// <summary>
		/// コンポーネントが破壊されているか確認
		/// </summary>
		/// <returns>
		/// <para> true : 破壊されている </para>
		/// <para> false : 破壊されていない </para>
		/// </returns>
		bool IsDestroyed() const { return isDestroyed; }

	public: // ------------------------ コンポーネント制御 -------------------------

		// ゲームオブジェクトに追加された時に呼び出される
		// 自分以外を参照しない場合は、この関数で初期化する
		virtual void Awake() {}

		// 最初のUpdateの直前で呼び出される
		// 自分以外を参照する場合は、この関数で初期化する
		virtual void Start() {}

		// 毎フレーム１回呼び出される
		virtual void Update(float deltaTime) {}

		// 衝突が起きたときに呼び出される
		virtual void OnCollision(const ComponentPtr& self, const ComponentPtr& other) {}

		// ゲームオブジェクトが削除されるときに呼び出される
		virtual void OnDestroy() {}

	public: // ----------------------- エディタ用 --------------------------

		// エディタに情報を表示する
		virtual void RenderEditor() = 0;

	private: // ----------------- コンポーネントの情報 ---------------------

		GameObject* owner = nullptr;	// このコンポーネントの所有者

		bool isStarted = false;			// Startが実行されたらtrueになる
		bool isDestroyed = false;		// Destroyが実行されたらtrueになる
	};

} // namespace PokarinEngine

#endif // !COMPONENT_H_INCLUDED
