/**
* @GameObject.h
*/
#ifndef GAMEOBJECT_H_INCLUDED
#define GAMEOBJECT_H_INCLUDED

#include "Components/Transform.h"
#include "Components/Colliders/AabbCollider.h"
#include "Texture.h"

#include "VecMath.h"
#include "Color.h"

#include "UsingNames/UsingMesh.h"
#include "UsingNames/UsingNodeEditor.h"

#include <string>
#include <vector>
#include <memory>
#include <type_traits>

namespace PokarinEngine
{
	// ----------------
	// 前方宣言
	// ----------------

	// ---------- クラス ----------

	class Engine;
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
		// エンジンクラスに情報公開
		friend Engine;

		// シーンクラスに情報公開
		friend Scene;

	public: // ---------------- コンストラクタ・デストラクタ ----------------

		/// <summary>
		/// コンストラクタ
		/// </summary>
		GameObject() = default;
		virtual ~GameObject() = default;

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
		/// <returns> 追加したコンポーネントのポインタ </returns>
		template<typename T>
		std::shared_ptr<T> AddComponent()
		{
			// コンポーネント作成
			auto p = std::make_shared<T>();

			// コンポーネントの所有者を登録
			p->owner = this;

			// コライダーなのか判定する
			// Tが判定するベースクラスと同じ、または基底クラスならtrueを返す
			if constexpr (std::is_base_of_v<Collider, T>)
			{
				// コライダーの追加
				colliders.push_back(p);
			}

			// コンポーネントの追加
			components.push_back(p);

			// コンポーネント追加時の処理を実行
			p->Awake();

			return p;
		}

		/// <summary>
		/// ゲームオブジェクトが持っているコンポーネントを取得
		/// </summary>
		/// <typeparam name="T"> 取得するコンポーネント </typeparam>
		/// <returns> 検索したコンポーネント </returns>
		template<typename T>
		std::shared_ptr<T> GetComponent()
		{
			// コンポーネント
			for (auto& e : components)
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
		virtual void Initialize();

		/// <summary>
		/// 更新
		/// </summary>
		/// <param name="deltaTime"> 前回の更新からの経過時間(秒) </param>
		virtual void Update(float deltaTime);

		/// <summary>
		/// 衝突時の処理
		/// </summary>
		/// <param name="self"> 衝突したコンポーネント(自分) </param>
		/// <param name="other"> 衝突したコンポーネント(相手) </param>
		virtual void OnCollision(const ComponentPtr& self, const ComponentPtr& other);

		/// <summary>
		/// 削除
		/// </summary>
		virtual void OnDestroy();

	public: // ------------------------- ノードエディタ -------------------------

		/// <summary>
		/// ノードエディタを開く
		/// </summary>
		void OpenNodeEditor() const;

	public: // --------------------------- 情報の取得 ---------------------------

		/// <summary>
		/// エンジンの機能を取得する
		/// </summary>
		/// <returns></returns>
		const Engine& GetEngine() const
		{
			return *engine;
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

		// true : 足場となる物体の上に乗っている
		// false : 足場となる物体の上に乗っていない
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
		std::vector<ComponentPtr> components;

	public: // -------------------------- コライダー ---------------------------

		// コライダー管理用配列
		std::vector<ColliderPtr> colliders;

	private: // -------------------------- エンジン ----------------------------

		// エンジンクラスのポインタ
		Engine* engine = nullptr;

	public: // ----------------------- ノードエディタ -------------------------

		// ノードエディタ
		NodeEditorPtr nodeEditor;

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