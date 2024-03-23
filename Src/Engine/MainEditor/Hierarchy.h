/**
* @file Hierarchy.h
*/
#ifndef HIERARCHY_H_INCLUDED
#define HIERARCHY_H_INCLUDED

#include "../UsingNames/UsingGameObject.h"

#include <memory>
#include <vector>

namespace PokarinEngine
{
	// ---------------------
	// 前方宣言
	// ---------------------

	class Engine;

	/// <summary>
	/// ヒエラルキー(シーン内のオブジェクト管理用ウィンドウ)
	/// </summary>
	class Hierarchy
	{
	public: // ------------- コンストラクタ・デストラクタ ----------------

		Hierarchy() = default;
		~Hierarchy() = default;

	public: // ------------------------- 制御 ----------------------------

		/// <summary>
		/// 初期化
		/// </summary>
		void Initialize(Engine& e);

		/// <summary>
		/// 更新
		/// </summary>
		void Update();

	public: // ---------------------- 情報の取得 -------------------------

		/// <summary>
		/// 選択中のオブジェクトの要素番号を取得する
		/// </summary>
		/// <returns> ウィンドウ内で選択中のオブジェクトの要素番号 </returns>
		GameObjectPtr GetSelectObject()
		{
			return selectObject;
		}

	private: // ------------------ ウィンドウ内の機能 --------------------

		/// <summary>
		/// ゲームオブジェクト表示用ツリーノードの処理
		/// </summary>
		void ObjectTree();

		/// <summary>
		/// メニューの処理
		/// </summary>
		void Mene();

		/// <summary>
		/// 選択中のオブジェクトに対する制御をまとめた関数
		/// </summary>
		void ScelectObjectControl();

	private: // --------------- ゲームオブジェクト作成用 -----------------

		/// <summary>
		/// ゲームオブジェクト作成用ボタンの処理
		/// </summary>
		/// <param name="kindName"> 作成するゲームオブジェクトの種類 </param>
		/// <param name="staticMeshFile"> スタティックメッシュのファイル名 </param>
		void CreateObjectButton(const char* typeName, const char* staticMeshFile);

		/// <summary>
		/// ゲームオブジェクト作成用ポップアップを展開する
		/// </summary>
		void CreateObjectPopup_Open();

		/// <summary>
		/// ゲームオブジェクト作成用ポップアップを展開した時の処理
		/// </summary>
		void CreateObjectPopup_Run();

	private: // ------------------ オブジェクト管理用 --------------------

		// 非選択時の番号
		const int unselected = -1;

		// 選択中オブジェクトの要素番号
		int selectObjectIndex = unselected;

		// 選択中のオブジェクト
		GameObjectPtr selectObject;

	private: // ----------------------- エンジン -------------------------

		// エンジンクラスのポインタ
		Engine* engine = nullptr;
	};

} // namespace PokarinEngine

#endif // !HIERARCHY_H_INCLUDED
