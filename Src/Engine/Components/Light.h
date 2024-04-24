/**
* @file Light.h
*/
#ifndef LIGTH_H_INCLUDED
#define LIGTH_H_INCLUDED

#include "Component.h"

#include "../LightParameter.h"
#include "../Math/Vector.h"

namespace PokarinEngine
{
	/// <summary>
	/// ライトコンポーネント
	/// </summary>
	class Light : public Component
	{
	public: // -------- コンストラクタ・デストラクタ ---------

		Light() = default;

		/// <summary>
		/// ライトを解放するデストラクタ
		/// </summary>
		~Light();

	public: // ------------- コンポーネント制御 -------------

		/// <summary>
		/// <para> コンポーネントを初期化 </para> 
		/// <para> ゲームオブジェクトに追加したときに実行 </para>
		/// </summary>
		void Awake() override;

		/// <summary>
		/// コンポーネントを更新
		/// </summary>
		void Update() override;

		/// <summary>
		/// エディタに情報を描画する
		/// </summary>
		void RenderInfo() override;

	public: // ----------------- 種類の設定 ----------------

		/// <summary>
		/// ライトの種類を設定する
		/// </summary>
		/// <param name="type"> ライトの種類 </param>
		void SetType(LightParameter::Type type)
		{
			lightData.type = type;
		}

	private: // ------------------- 情報 -------------------

		// ライトデータ
		LightParameter::LightData lightData;

	private: // ------------------ 管理用 ------------------

		// ライトを取得してない時の番号
		const int invaild = -1;

		// ライトの要素番号
		int lightIndex = invaild;
	};

} // namespace PokarinEngine

#endif // !LIGTH_H_INCLUDED
