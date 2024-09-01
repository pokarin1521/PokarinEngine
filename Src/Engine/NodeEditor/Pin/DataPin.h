/**
* @file DataPin.h
*/
#ifndef POKARINENGINE_DATAPIN_H_INCLUDED
#define POKARINENGINE_DATAPIN_H_INCLUDED

#include "Json/Json.h"

#include "Pin.h"
#include "../../UsingNames/UsingDataPin.h"

namespace PokarinEngine
{
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

	public: // ------------------------ Json ------------------------

		/// <summary>
		/// 情報をJson型に格納する
		/// </summary>
		/// <param name="[out] json"> 情報を格納するJson型 </param>
		void PinToJson(Json& json) const override
		{	
			json["Data"] = data;
		}

		/// <summary>
		/// 情報をJson型から取得する
		/// </summary>
		/// <param name="[in] json"> 情報を格納しているJson型 </param>
		void PinFromJson(const Json& json) override
		{
			json["Data"].get_to(data);
		}

	public: // ------------------------- 値 -------------------------

		// ピンが持つ値
		T data;
	};
}

#endif // !POKARINENGINE_DATAPIN_H_INCLUDED
