/**
* @GameObject.h
*/
#ifndef GAMEOBJECT_H_INCLUDED
#define GAMEOBJECT_H_INCLUDED

#include "Components/Transform.h"
#include "Components/Colliders/AabbCollider.h"
#include "Texture.h"

#include "Math/Vector.h"
#include "Color.h"

#include "UsingNames/UsingMesh.h"
#include "UsingNames/UsingNodeEditor.h"

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
		// シーンに情報を公開
		friend Scene;

	public: // ---------------- コンストラクタ・デストラクタ ----------------

		GameObject() = default;
		~GameObject() = default;

	public: // -------------------------- 禁止事項 --------------------------

		// コピーの禁止
		GameObject(GameObject&) = delete;

		// 代入の禁止
		GameObject& operator=(GameObject&) = delete;

	public: // ----------------------- コンポーネント -----------------------

		/// <summary>
		/// ゲームオブジェクトにコンポーネント、コライダーを追加する 
		/// </summary>
		/// <typeparam name="T"> コンポーネントクラス </typeparam>
		/// <param name="coponentName"> コンポーネントの名前 </param>
		/// <returns> 
		/// <para> first : 追加したコンポーネントのポインタ </para>
		/// <para> second : 追加できたらtrue, 追加済みで追加できなかったらfalse </para> 
		/// </returns>
		template <class T>
		std::pair<std::shared_ptr<T>, bool> AddComponent()
		{
			// コライダーならtrue
			constexpr bool isCollider = std::is_base_of_v<Collider, T>;

			// 追加済みのコンポーネントか確認するために取得を試みる
			auto component = GetComponent<T>();

			// コライダー以外で、追加済みのコンポーネントなら
			// 取得したコンポーネントを返す
			if (component && !isCollider)
			{
				return { component, false };
			}

			// コンポーネント作成
			auto p = std::make_shared<T>();

			// コンポーネントの所有者を登録
			p->ownerObject = this;

			// コライダーなのか判定する
			// Tが判定するベースクラスと同じ、または基底クラスならtrueを返す
			if constexpr (isCollider)
			{
				// コライダーの追加
				colliderList.push_back(p);
			}

			// コンポーネントの追加
			componentList.push_back(p);

			// コンポーネント追加時の処理を実行
			p->Awake();

			return { p, true };
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
		/// ゲームオブジェクトからコンポーネントを削除する
		/// </summary>
		void RemoveDestroyedComponent();

	public: // ------------------- ゲームオブジェクト制御 -------------------

		/// <summary>
		/// 初期化
		/// </summary>
		void Initialize();

		/// <summary>
		/// 更新
		/// </summary>
		/// <param name="isPlayGame"> 作成中のゲームが再生中ならtrue </param>
		void Update(bool isPlayGame);

		/// <summary>
		/// 削除
		/// </summary>
		void OnDestroy();

	public: // ------------------------- ノードエディタ -------------------------

		/// <summary>
		/// ノードエディタを開く
		/// </summary>
		void OpenNodeEditor() const;

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
		/// ゲームオブジェクトの名前を取得する
		/// </summary>
		/// <returns> ゲームオブジェクトの名前 </returns>
		const char* GetName() const
		{
			return name.c_str();
		}

		/// <summary>
		/// オブジェクト名の最大文字数を取得する
		/// </summary>
		/// <returns> オブジェクト名の最大文字数 </returns>
		size_t GetNameSize() const
		{
			return nameSize;
		}

		/// <summary>
		/// 識別番号を取得する
		/// </summary>
		/// <returns> 識別番号 </returns>
		int GetID() const
		{
			return id;
		}

	public: // -------------------------- 基本の情報 ---------------------------

		// 座標・回転・拡大率の制御用コンポーネント
		TransformPtr transform;

		// 物体の色
		Color color = { 1,1,1,1 };


		bool isGrounded = false;

	public: // ------------------------- 描画系の情報 --------------------------

		// 表示するスタティックメッシュ
		StaticMeshPtr staticMesh;

		// ゲームオブジェクト固有のマテリアル配列
		MaterialList materials;

		// 描画の優先度
		int renderQueue = renderQueue_geometry;

	public: // ------------------------ コンポーネント -------------------------

		// コンポーネント管理用配列
		std::vector<ComponentPtr> componentList;

	public: // -------------------------- コライダー ---------------------------

		// コライダー管理用配列
		std::vector<ColliderPtr> colliderList;

	private: // --------------------------- シーン -----------------------------

		// 持ち主であるシーン
		Scene* ownerScene = nullptr;

	public: // ----------------------- ノードエディタ -------------------------

		/* ヘッダにインクルードしたくないのでポインタ */

		// ノードエディタ
		NodeEditorPtr nodeEditor;

	private: // ---------------------------- 更新 ------------------------------

		/// <summary>
		/// ゲームオブジェクトにあるコンポーネントを更新する
		/// </summary>
		/// <param name="isPlayGame"> 作成中のゲームが再生中ならtrue </param>
		void UpdateComponent(bool isPlayGame);

		/// <summary>
		/// ゲームオブジェクトの座標変換行列を更新する
		/// </summary>
		void UpdateMatrix();

	private: // --------------------------- 管理用 -----------------------------

		// 削除されたらtrue
		bool isDestroyed = false;

		// 識別番号
		int id = 0;

	private: // ---------------------------- 名前 ------------------------------

		// オブジェクトの名前
		std::string name = "";

		// オブジェクトの種類名
		std::string typeName = "";

		// オブジェクト名の最大文字数
		size_t nameSize = 32;
	};

} // namespace PokarinEngine

#endif // !GAMEOBJECT_H_INCLUDED