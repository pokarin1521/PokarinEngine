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
	// ------------------------
	// 前方宣言
	// ------------------------

	class Engine;

	/// <summary>
	/// インスペクター(シーン内のオブジェクト制御用ウィンドウ)
	/// </summary>
	class Inspector
	{
	public: // --------------- コンストラクタ・デストラクタ ------------------

		Inspector() = default;
		~Inspector() = default;

	public: // ---------------------------- 制御 -----------------------------

		/// <summary>
		/// 初期化
		/// </summary>
		void Initialize(Engine& e);

		/// <summary>
		/// 更新
		/// </summary>
		/// <param name="selectObject"> ヒエラルキーで選択中のオブジェクト </param>
		void Update(GameObjectPtr selectObject);

	private: // --------------------- オブジェクト制御用 ---------------------

		// 選択中のオブジェクトの名前
		std::string selectObjectName = "";

	private: // -------------------------- エンジン --------------------------

		Engine* engine = nullptr;
	};

} // namespace PokarinEngine

#endif // !INSPECTOR_H_INCLUDED

