/**
* @file Light.h
*/
#ifndef POKARINENGINE_LIGTH_H_INCLUDED
#define POKARINENGINE_LIGTH_H_INCLUDED

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
		/// ゲームオブジェクトに追加された時の初期化
		/// </summary>
		void Awake() override;

		/// <summary>
		/// コンポーネントを更新
		/// </summary>
		void Update() override;

	public: // ----------------- 種類の設定 ----------------

		/// <summary>
		/// ライトの種類を設定する
		/// </summary>
		/// <param name="[in] type"> ライトの種類 </param>
		void SetType(LightParameter::Type type)
		{
			lightData.type = type;
		}

	public: // -------------------- Json -------------------

		/// <summary>
		/// コンポーネントの情報をJson型に格納する
		/// </summary>
		/// <param name="[out] Json"> 情報を格納するJson型 </param>
		void ToJson(Json& data) const override;

		/// <summary>
		/// コンポーネントの情報をJson型から取得する
		/// </summary>
		/// <param name="[out] data"> 情報を格納しているJson型 </param>
		void FromJson(const Json& data) override;

	private: // ---------------- エディタ用 ----------------

		/// <summary>
		/// 情報を編集できるように表示する
		/// </summary>
		void InfoEditor() override;

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

#endif // !POKARINENGINE_LIGTH_H_INCLUDED
