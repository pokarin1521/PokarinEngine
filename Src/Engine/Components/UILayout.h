/**
* @file UILayout.h
*/
#ifndef UILAYOUT_H_INCLUDED
#define UILAYOUT_H_INCLUDED

#include "Component.h"
#include "../Math/Vector.h"

namespace PokarinEngine
{
	/**
	* UIを画面空間で配置するためのコンポーネント
	*/
	class UILayout : public Component
	{
	public: // コンストラクタ・デストラクタ

		UILayout() = default;
		~UILayout() = default;

	public: // コンポーネント制御

		/// <summary>
		/// UIレイアウトの実行を開始する 
		/// </summary>
		void Start() override;

		/// <summary>
		/// 更新イベント
		/// </summary>
		/// <param name="deltaTime"> 
		/// 前回の更新からの経過時間(秒) 
		/// </param>
		void Update(float deltaTime) override;

	public: // イベント用変数

		// カメラから見たUIの座標
		Vector3 basePosition = { 0, 0, 0 };
	};

} // namespace PokarinEngine

#endif // !UILAYOUT_H_INCLUDED
