/**
* @file Component.h
*/
#ifndef COMPONENT_H_INCLUDED
#define COMPONENT_H_INCLUDED

#include "../UsingNames/UsingComponent.h"
#include "../UsingNames/UsingGameObject.h"

#include "Json/UsingNameJson.h"

#include <string>

namespace PokarinEngine
{
	/// <summary>
	/// コンポーネント基底クラス
	/// </summary>
	class Component
	{
	public: // -------------------- コンストラクタ・デストラクタ ----------------------

		Component() = default;
		virtual ~Component() = default;

	public: // ---------------------------- 持ち主の取得 ------------------------------

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

	public: // --------------------------------- 制御 --------------------------------

		/// <summary>
		/// ゲームオブジェクトに追加した時の処理
		/// </summary>
		/// <param name="[in] owner"> 持ち主になるゲームオブジェクト </param>
		/// <param name="[in] componentName"> コンポーネントの名前 </param>
		/// <param name="[in] componentTitle"> コンポーネントの表示用タイトル </param>
		/// <param name="[in] componentID"> コンポーネントの識別番号 </param>
		void AddComponent(GameObject& owner, const std::string& componentName, 
			const std::string& componentTitle, int componentID)
		{
			ownerObject = &owner;
			name = componentName;
			title = componentTitle;
			id = componentID;

			Awake();
		}

		/// <summary>
		/// 最初の更新の直前での初期化
		/// </summary>
		void Initialize()
		{
			if (isStarted)
			{
				return;
			}

			Start();
			isStarted = true;
		}

		/// <summary>
		/// 毎フレーム呼び出される
		/// </summary>
		virtual void Update() {}

		/// <summary>
		/// 作成中のゲームが再生中の時に毎フレーム呼び出される
		/// </summary>
		virtual void Update_PlayGame() {}

		/// <summary>
		/// ゲームオブジェクトが削除されるときに呼び出される
		/// </summary>
		virtual void OnDestroy() {}

	public: // --------------------------------- 削除 --------------------------------

		/// <summary>
		/// コンポーネントをゲームオブジェクトから削除する
		/// </summary>
		void Destroy() { isDestroyed = true; }

		/// <summary>
		/// コンポーネントが削除されているか確認
		/// </summary>
		/// <returns>
		/// <para> true : 削除されている </para>
		/// <para> false : 削除されていない </para>
		/// </returns>
		bool IsDestroyed() const { return isDestroyed; }

	public: // ----------------------------- エディタ用 ------------------------------

		/// <summary>
		/// エディタに情報を表示する
		/// </summary>
		void RenderInfo();

	public: // --------------------------- 保存・読み込み ----------------------------

		/// <summary>
		/// コンポーネントの情報をJson型に格納する
		/// </summary>
		/// <param name="[out] data"> 情報を格納するJson型 </param>
		void ToJson(Json& data) const;

		/// <summary>
		/// コンポーネントの情報をJson型から取得する
		/// </summary>
		/// <param name="[out] data"> 情報を格納しているJson型 </param>
		void FromJson(const Json& data) {}

	public: // ------------------------------- 識別番号 ------------------------------

		/// <summary>
		/// 識別番号を取得する
		/// </summary>
		/// <returns> コンポーネントの識別番号 </returns>
		int GetID() const
		{
			return id;
		}

		/// <summary>
		/// 識別番号を文字列で取得する
		/// </summary>
		/// <returns> 識別番号(文字列) </returns>
		std::string GetID_String() const
		{
			return std::to_string(id);
		}

	private: // ------------------------------- 初期化 -------------------------------

		/// <summary>
		/// <para> ゲームオブジェクトに追加された時に呼び出される </para>
		/// <para> 自分以外を参照しない場合は、この関数で初期化する </para>
		/// </summary>
		virtual void Awake() {}

		/// <summary>
		/// <para> 最初の更新の直前で呼び出される </para>
		/// <para> 自分以外を参照する場合は、この関数で初期化する </para>
		/// </summary>
		virtual void Start() {}

	private: // ----------------------------- エディタ用 ------------------------------

		/// <summary>
		/// 情報を編集できるように表示する
		/// </summary>
		virtual void InfoEditor() = 0;

		/// <summary>
		/// コンポーネント操作用ポップアップの処理
		/// </summary>
		void ControlPopup();

	private: // -------------------------------- 保存 ---------------------------------

		/// <summary>
		/// 各コンポーネントの情報をJson型に格納する
		/// </summary>
		/// <param name="[out] Json"> 情報を格納するJson型 </param>
		virtual void ComponentToJson(Json& data) const = 0;

	private: // -------------------------------- 情報 ---------------------------------

		// 持ち主であるゲームオブジェクト
		GameObject* ownerObject = nullptr;

		// 初期化済みならtrue
		bool isStarted = false;

		// 削除済みならtrue
		bool isDestroyed = false;

		// 名前
		std::string name = "No Name";

		// 表示用タイトル
		std::string title = "No Title";

		// 識別番号
		int id = 0;
	};

} // namespace PokarinEngine

#endif // !COMPONENT_H_INCLUDED
