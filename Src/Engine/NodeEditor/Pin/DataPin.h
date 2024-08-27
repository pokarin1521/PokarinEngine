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

	public: // --------------------- ピンの情報 ---------------------

		/// <summary>
		/// ピンの種類を取得する
		/// </summary>
		/// <returns> ピンの種類 </returns>
		PinType GetType() const
		{
			return PinType::Data;
		}

		/// <summary>
		/// ピンの型を取得する
		/// </summary>
		/// <returns> ピンの型(typeid) </returns>
		std::type_index GetClass() const override
		{
			// 特定の型と比較する場合があるので、thisポインタではなく型を指定する
			return typeid(DataPin<T>);
		}

	public: // ----------------------- リンク -----------------------

		/// <summary>
		/// ピン同士をリンクさせる
		/// </summary>
		/// <param name="[in] linkID"> リンク識別番号 </param>
		/// <param name="[in] pin"> リンクさせるピン </param>
		void Link(int linkID, Pin& pin)
		{
			linkList.emplace(linkID, &pin);
		}

	public: // ------------------------- 値 -------------------------

		// ピンが持つ値
		T data;
	};
}

#endif // !POKARINENGINE_DATAPIN_H_INCLUDED
