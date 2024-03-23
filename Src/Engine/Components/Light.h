/**
* @file Light.h
*/
#ifndef LIGTH_H_INCLUDED
#define LIGTH_H_INCLUDED

#include "Component.h"
#include "../VecMath.h"

namespace PokarinEngine
{
	/**
	* ライトコンポーネント
	*/
	class Light : public Component
	{
	public: // -------- コンストラクタ・デストラクタ ---------

		Light() = default;

		/// <summary>
		/// ライトを解放するデストラクタ
		/// </summary>
		virtual ~Light();

	public: // ------------ コンポーネント制御 --------------

		/// <summary>
		/// コンポーネントを初期化
		/// ゲームオブジェクトに追加したときに実行
		/// </summary>
		virtual void Awake() override;

		/// <summary>
		/// コンポーネントを更新
		/// </summary>
		/// <param name="deltaTime"> 前回の更新からの経過時間(秒) </param>
		virtual void Update(float deltaTime) override;

	public: // ----------- ライトの種類 -------------

		// ライトの種類
		enum class Type
		{
			pointLight, // 点光源
			spotLight,  // スポット光源
		};

		// ライトの種類
		// 初期 : 点光源
		Type type = Type::pointLight;

	public: // ----------- ライトの情報 -------------

		// 色
		Vec3 color = { 1, 1, 1 };

		// 明るさ
		float intensity = 1;

		// ライトが届く最大半径
		float radius = 1;

	public: // ---------- スポットライト用 -----------

		// スポットライトが照らす角度
		float coneAngle = Radians(30);

		// スポットライトの減衰開始角度
		float falloffAngle = Radians(20);

	private: // 管理用

		// ライトインデックス
		// -1 : 無効
		int lightIndex = -1;
	};

} // namespace PokarinEngine

#endif // !LIGTH_H_INCLUDED
