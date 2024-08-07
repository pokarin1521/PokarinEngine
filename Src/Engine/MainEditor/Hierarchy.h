/**
* @file Hierarchy.h
*/
#ifndef POKARINENGINE_HIERARCHY_H_INCLUDED
#define POKARINENGINE_HIERARCHY_H_INCLUDED

#include "../UsingNames/UsingScene.h"
#include "../UsingNames/UsingGameObject.h"

#include <memory>
#include <vector>

namespace PokarinEngine
{
	// ---------------------
	// 前方宣言
	// ---------------------

	class Scene;

	/// <summary>
	/// ヒエラルキー(シーン内のオブジェクト管理用ウィンドウ)
	/// </summary>
	class Hierarchy
	{
	public: // ---------------- コンストラクタ・デストラクタ ---------------

		Hierarchy() = default;
		~Hierarchy() = default;

	public: // ------------------------- 禁止事項 --------------------------

		/* エディタ管理がおかしくなるので禁止する */

		// コピーコンストラクタの禁止
		Hierarchy(const Hierarchy&) = delete;

		// 代入の禁止
		Hierarchy& operator=(const Hierarchy&) = delete;

	public: // --------------------------- 制御 ----------------------------

		/// <summary>
		/// 更新
		/// </summary>
		/// <param name="[in] currentScene"> 現在のシーン </param>
		void Update(const ScenePtr& currentScene);

	public: // -------------------- オブジェクトの取得 ---------------------

		/// <summary>
		/// 選択中のオブジェクトを取得する
		/// </summary>
		/// <returns> ウィンドウ内で選択中のオブジェクト </returns>
		GameObjectPtr& GetSelectObject();

	private: // -------------------- ウィンドウ内の処理 --------------------

		/// <summary>
		/// ヒエラルキーウィンドウ内での操作
		/// </summary>
		void HierarchyControl();

		/// <summary>
		/// メニューの処理
		/// </summary>
		void Mene();

		/// <summary>
		/// ゲームオブジェクト表示用ツリーノードの処理
		/// </summary>
		void ObjectTree();

		/// <summary>
		/// 選択中のオブジェクトに対する操作
		/// </summary>
		void ScelectObjectControl();

	private: // -------------------- オブジェクト作成用 --------------------

		/// <summary>
		/// ゲームオブジェクト作成用ポップアップの処理
		/// </summary>
		void CreateObjectPopup();

		/// <summary>
		/// ゲームオブジェクト作成用ボタンの処理
		/// </summary>
		/// <param name="[in] kindName"> 作成するゲームオブジェクトの種類 </param>
		/// <param name="[in] staticMeshFile"> スタティックメッシュのファイル名 </param>
		void CreateObjectButton(const char* typeName, const char* staticMeshFile);

	private: // --------------------------- 情報 ---------------------------

		// 非選択時の要素番号
		const int unselected = -1;

		// 選択中オブジェクトの要素番号
		int selectObjectIndex = unselected;

		// 選択中のオブジェクト
		GameObjectPtr selectObject;

		// 現在のシーン
		ScenePtr currentScene;

		// オブジェクト作成用ポップアップの名前
		const char* createObjectPopupName = "CreateObject";
	};

} // namespace PokarinEngine

#endif // !POKARINENGINE_HIERARCHY_H_INCLUDED
