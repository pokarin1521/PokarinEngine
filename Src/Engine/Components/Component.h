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

		// 毎フレーム呼び出される
		virtual void Update() {}

		// 作成中のゲームが再生中の時に
		// 毎フレーム呼び出される
		virtual void Update_PlayGame() {}

		// ゲームオブジェクトが削除されるときに呼び出される
		virtual void OnDestroy() {}

	public: // ----------------------- エディタ用 --------------------------

		// 情報を編集できるように表示する
		virtual void InfoEditor() = 0;

	public: // ------------------------ 名前の取得 -------------------------

		// 名前を取得する
		virtual const std::string& GetName() = 0;

	public: // --------------------------- 名前 ----------------------------

		// コンポーネントの名前
		inline static const std::string name = "No Name";

	protected: // -------------------- 持ち主の取得 ------------------------

		/// <summary>
		/// 持ち主であるゲームオブジェクトを取得する
		/// </summary>
		/// <returns> 持ち主であるゲームオブジェクト </returns>
		const GameObject& GetOwnerObject() const { return *ownerObject; }

		/// <summary>
		/// 持ち主であるゲームオブジェクトを取得する
		/// </summary>
		/// <returns> 持ち主であるゲームオブジェクト </returns>
		GameObject& GetOwnerObject() { return *ownerObject; }

	private: // ----------------- コンポーネントの情報 ---------------------

		// 持ち主であるゲームオブジェクト
		GameObject* ownerObject = nullptr;

		// 初期化済みならtrue
		bool isStarted = false;

		// 削除済みならtrue
		bool isDestroyed = false;
	};

} // namespace PokarinEngine

#endif // !COMPONENT_H_INCLUDED
