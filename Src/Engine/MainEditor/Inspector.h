/**
* @file Inspector.h
*/
#ifndef INSPECTOR_H_INCLUDED
#define INSPECTOR_H_INCLUDED

#include "ImGui/imgui.h"

#include "../UsingNames/UsingGameObject.h"

#include <string>
#include <memory>

namespace PokarinEngine
{
	/// <summary>
	/// インスペクター(シーン内のオブジェクト制御用ウィンドウ)
	/// </summary>
	class Inspector
	{
	public: // ---------------- コンストラクタ・デストラクタ -----------------

		Inspector() = default;
		~Inspector() = default;

	public: // ---------------------------- 制御 -----------------------------

		/// <summary>
		/// 更新
		/// </summary>
		/// <param name="[in,out] hierarchySelect"> ヒエラルキーで選択中のオブジェクト </param>
		void Update(GameObjectPtr& hierarchySelect);

	private: // ------------------- オブジェクト名の表示 ---------------------

		/// <summary>
		/// オブジェクト名を表示する
		/// </summary>
		void RenderName();

	private: // ------------------- コンポーネント追加用 ---------------------

		/// <summary>
		/// コンポーネント追加用ポップアップの処理
		/// </summary>
		void AddComponentPopup();

		/// <summary>
		/// コンポーネント追加用リストを表示する
		/// </summary>
		void AddComponentList();

	private: // ------------------- 選択中のオブジェクト ---------------------

		// 選択中のゲームオブジェクト
		GameObjectPtr selectObject;
	};

} // namespace PokarinEngine

#endif // !INSPECTOR_H_INCLUDED

