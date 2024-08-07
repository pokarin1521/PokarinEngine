/**
* @GameObject.h
*/
#ifndef POKARINENGINE_GAMEOBJECT_H_INCLUDED
#define POKARINENGINE_GAMEOBJECT_H_INCLUDED

#include "Json/UsingNameJson.h"

#include "Components/Rigidbody.h"
#include "Components/Transform.h"
#include "Components/ComponentAdder.h"

#include "Mesh/StaticMesh.h"

#include "Math/Vector.h"
#include "Color.h"

#include "UsingNames/UsingNodeEditor.h"
#include "UsingNames/UsingCollider.h"

#include "Configs/MeshConfig.h"

#include <string>
#include <vector>
#include <unordered_set>
#include <memory>
#include <type_traits>

namespace PokarinEngine
{
	// ----------------
	// 前方宣言
	// ----------------

	class Scene;
	class Camera;

	/// int型としても使うので、普通の列挙型
	/// <summary>
	/// 描画の優先度
	/// </summary>
	enum RenderQueue
	{
		renderQueue_geometry = 2000,	// 一般的な図形
		renderQueue_transparent = 3000,	// 半透明な図形
		renderQueue_overlay = 4000,		// UI, 全画面エフェクトなど
		renderQueue_max = 5000,			// キューの最大値
	};

	/// <summary>
	/// ゲームに登場する様々なオブジェクトを表す基本クラス
	/// </summary>
	class GameObject
	{
	public: // ---------------- コンストラクタ・デストラクタ ----------------

		GameObject() = default;
		~GameObject() = default;

	public: // -------------------------- 禁止事項 --------------------------

		/* 識別できなくなるので、禁止する */

		// コピーコンストラクタの禁止
		GameObject(GameObject&) = delete;

		// 代入の禁止
		GameObject& operator=(GameObject&) = delete;

	public: // ----------------------- コンポーネント -----------------------

		/// <summary>
		/// ゲームオブジェクトにコンポーネントを追加する 
		/// </summary>
		/// <typeparam name="T"> コンポーネントクラス </typeparam>
		/// <returns> 
		/// <para> first : 追加したコンポーネントのポインタ </para>
		/// <para> second : 追加できたらtrue, 追加済みで追加できなかったらfalse </para> 
		/// </returns>
		template <class T>
		std::shared_ptr<T> AddComponent()
		{
			// コライダーならtrue
			constexpr bool isCollider = std::is_base_of_v<Collider, T>;

			// ---------------------------------------------------
			// コンポーネントが追加済みか確認する
			// ---------------------------------------------------

			// コライダーは複数追加可能なので確認しない
			if (!isCollider)
			{
				// 追加済みのコンポーネントか確認するために取得を試みる
				auto component = GetComponent<T>();

				// 追加済みのコンポーネントなら
				// 取得したコンポーネントを返す
				if (component)
				{
					return component;
				}
			}

			// -------------------------------------------
			// コンポーネントを追加する
			// -------------------------------------------

			// コンポーネント作成
			auto component = std::make_shared<T>();

			// コンポーネントの名前
			const std::string componentName = ComponentAdder::GetName<T>();

			// コンポーネントの表示用タイトル
			// 区別できるように、名前の後ろにゲームオブジェクトの識別番号を付ける
			// (エディタでは非表示になるように、##を付ける)
			const std::string componentTitle = componentName + "##" + std::to_string(id);

			// コンポーネント識別番号
			const int componentID = GetSingleComponentID();

			// 追加時の処理を実行
			component->AddComponent(*this, componentName, componentTitle, componentID);

			// コライダーなのか判定する
			// Tが判定するベースクラスと同じ、または基底クラスならtrueを返す
			if constexpr (isCollider)
			{
				// コライダーの追加
				colliderList.push_back(component);
			}

			// コンポーネントの追加
			componentList.push_back(component);

			// 物理挙動用コンポーネントなら保持
			if constexpr (std::is_base_of_v<Rigidbody, T>)
			{
				rigidbody = component;
			}

			// カメラコンポーネントならシーンに追加する
			if constexpr (std::is_base_of_v<Camera, T>)
			{
				ownerScene->AddCamera(component);
			}

			return component;
		}

		/// <summary>
		/// ゲームオブジェクトが持っているコンポーネントを取得
		/// </summary>
		/// <typeparam name="T"> 取得するコンポーネント </typeparam>
		/// <returns> 検索したコンポーネント </returns>
		template <class T>
		std::shared_ptr<T> GetComponent() const
		{
			// コンポーネント
			for (auto& e : componentList)
			{
				// コンポーネントがT型
				// またはTの派生クラスの場合に変換が成功する
				// (失敗した場合、空のポインタが入る)
				auto p = std::dynamic_pointer_cast<T>(e);

				// コンポーネントを見つけた
				if (p)
				{
					// 見つけたコンポーネントを返す
					return p;
				}
			}

			// コンポーネントが見つからなかったので、
			// nullptrを返す
			return nullptr;
		}

	public: // ------------------ ゲームオブジェクトの管理 ------------------

		/// <summary>
		/// ゲームオブジェクトを削除する
		/// </summary>
		void Destroy() { isDestroyed = true; }

		/// <summary>
		/// ゲームオブジェクトが削除済みか取得する
		/// </summary>
		/// <returns> 
		/// <para> true : 削除されている </para>
		/// <para> false : 削除されていない </para>
		/// </returns>
		bool IsDestroyed() const { return isDestroyed; }

	public: // -------------------- コンポーネントの管理 --------------------

		/// <summary>
		/// 削除予定(削除処理が未実行)のコンポーネントを完全に削除する
		/// </summary>
		void RemoveDestroyedComponent();

	public: // ------------------- ゲームオブジェクト制御 -------------------

		/// <summary>
		/// 初期化
		/// </summary>
		/// <param name="[in] scene"> 持ち主であるシーン </param>
		/// <param name="[in] objectID"> 識別番号 </param>
		/// <param name="[in] objectName"> 名前 </param>
		/// <param name="[in] position"> 位置 </param>
		/// <param name="[in] rotation"> 回転角度 </param>
		void Initialize(Scene& scene, int objectID, const std::string& objectName,
			const Vector3& position, const Vector3& rotation);

		/// <summary>
		/// 更新
		/// </summary>
		/// <param name="[in] isPlayGame"> 作成中のゲームが再生中ならtrue </param>
		void Update(bool isPlayGame);

		/// <summary>
		/// 削除
		/// </summary>
		void OnDestroy();

		/// <summary>
		/// コライダーを描画する
		/// </summary>
		void DrawCollider() const;

	public: // ---------------------------- エディタ ----------------------------

		/// <summary>
		/// ノードエディタを開く
		/// </summary>
		void OpenNodeEditor() const;

		/// <summary>
		/// コンポーネントをエディタに表示する
		/// </summary>
		void RenderComponent();

	public: // --------------------------- 情報の取得 ---------------------------

		/// <summary>
		/// 持ち主であるシーンを取得する
		/// </summary>
		/// <returns> このゲームオブジェクトの持ち主であるシーン </returns>
		const Scene& GetOwnerScene() const
		{
			return *ownerScene;
		}

		/// <summary>
		/// 持ち主であるシーンを取得する
		/// </summary>
		/// <returns> このゲームオブジェクトを持ち主であるシーン </returns>
		Scene& GetOwnerScene()
		{
			return *ownerScene;
		}

		/// <summary>
		/// 識別番号を取得する
		/// </summary>
		/// <returns> 識別番号 </returns>
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

		/// <summary>
		/// 物理挙動用コンポーネントを持っているか取得する
		/// </summary>
		/// <returns>
		/// <para> true : 持っている </para>
		/// <para> false : 持っていない </para>
		/// </returns>
		bool HasRigidbody()
		{
			return rigidbody != nullptr;
		}

	public: // ----------------------------- 保存 ------------------------------

		/// <summary>
		/// ゲームオブジェクトの情報をJson型に格納する
		/// </summary>
		/// <param name="[out] data"> 情報を格納するJson型 </param>
		void ToJson(Json& data) const;

	public: // --------------------------- 読み込み ----------------------------

		/// <summary>
		/// ゲームオブジェクトの情報をJson型から取得する
		/// </summary>
		/// <param name="[in] data"> 情報を格納しているJson型 </param>
		void FromJson(const Json& data);

	public: // -------------------------- 基本の情報 ---------------------------

		// 位置・回転・拡大率の制御用コンポーネント
		TransformPtr transform;

		// 物体の色
		Color color = Color::white;

		// 地面に着地しているならtrue
		bool isGrounded = false;

		// コライダー管理用配列
		std::vector<ColliderPtr> colliderList;

		// オブジェクトの名前
		std::string name = "";

		// オブジェクト名の最大文字数
		const size_t nameSize = 32;

	public: // ------------------------- 描画系の情報 --------------------------

		// 表示するスタティックメッシュ
		StaticMeshPtr staticMesh;

		// ゲームオブジェクト固有のマテリアル配列
		MaterialList materialList;

		// 描画の優先度
		int renderQueue = renderQueue_geometry;

	private: // ------------------- コンポーネント識別番号 ---------------------

		/// <summary>
		/// コンポーネント識別番号を取得する
		/// </summary>
		/// <returns> 重複しない識別番号 </returns>
		int GetSingleComponentID();

	private: // ---------------------------- 更新 ------------------------------

		/// <summary>
		/// ゲームオブジェクトにあるコンポーネントを更新する
		/// </summary>
		/// <param name="[in] isPlayGame"> 作成中のゲームが再生中ならtrue </param>
		void UpdateComponent(bool isPlayGame);

	private: // --------------------------- 管理用 -----------------------------

		// 削除されたらtrue
		bool isDestroyed = false;

		// 識別番号
		int id = 0;

	private: // --------------------------- シーン -----------------------------

		// 持ち主であるシーン
		Scene* ownerScene = nullptr;

	private: // ----------------------- ノードエディタ -------------------------

		// ノードエディタ
		NodeEditorPtr nodeEditor;

	private: // ----------------------- コンポーネント -------------------------

		// コンポーネント管理用配列
		std::vector<ComponentPtr> componentList;

		// コンポーネント識別番号の管理用配列
		std::unordered_set<int> componentIDList;

		// 物理挙動用コンポーネント
		RigidbodyPtr rigidbody;
	};

} // namespace PokarinEngine

#endif // !POKARINENGINE_GAMEOBJECT_H_INCLUDED