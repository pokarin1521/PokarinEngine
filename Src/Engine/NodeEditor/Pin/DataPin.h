/**
* @file DataPin.h
*/
#ifndef POKARINENGINE_DATAPIN_H_INCLUDED
#define POKARINENGINE_DATAPIN_H_INCLUDED

#include "Pin.h"

#include "ImGui/imgui.h"

namespace PokarinEngine
{
	// ---------------------
	// 前方宣言
	// ---------------------

	template <class T>
	class DataPin;

	// ---------------------------
	// 型の別名を定義
	// ---------------------------

	template <class T>
	using DataPinPtr = std::shared_ptr<DataPin<T>>;

	/// <summary>
	/// 値用ピン
	/// </summary>
	template <class T>
	class DataPin : public Pin
	{
	public: // ------------ コンストラクタ・デストラクタ ------------

		// 基底クラスの全てのコンストラクタを継承する
		using Pin::Pin;

		~DataPin() = default;

	public: // --------------------- ピンの種類 ---------------------

		/// <summary>
		/// ピンの種類を取得する
		/// </summary>
		/// <returns> ピンの種類 </returns>
		PinType GetType() const override
		{
			return PinType::Data;
		}

	public: // ----------------------- リンク -----------------------

		/// <summary>
		/// ピン同士をリンクさせる
		/// </summary>
		/// <param name="pin"> リンクさせるピン </param>
		void LinkPin(Pin& pin)
		{
			linkList.emplace(&pin);
		}

	public: // ------------------------- 値 -------------------------

		// ピンが持つ値
		T data;
	};
}

#endif // !POKARINENGINE_DATAPIN_H_INCLUDED
