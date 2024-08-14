/**
* @file Pin.h
*/
#ifndef POKARINENGINE_PIN_H_INCLUDED
#define POKARINENGINE_PIN_H_INCLUDED

#include "PinType.h"

#include <memory>

namespace PokarinEngine
{
	/// <summary>
	/// ノードのピン
	/// </summary>
	class Pin
	{
	public: // ------------------- コンストラクタ・デストラクタ --------------------

		Pin(int _nodeID, int pinID, PinType pinType) :nodeID(_nodeID), id(pinID), type(pinType) {}
		~Pin() = default;

	public: // ----------------------------- ピンの種類 ----------------------------

		/// <summary>
		/// ピンの種類を取得する
		/// </summary>
		/// <returns> ピンの種類 </returns>
		PinType GetType() const
		{
			return type;
		}

		/// <summary>
		/// 持ち主であるノードの識別番号を取得する
		/// </summary>
		/// <returns> 持ち主であるノードの識別番号 </returns>
		int GetNodeID() const
		{
			return nodeID;
		}

	private: // ------------------------------- 情報 -------------------------------

		// ピンの種類
		PinType type = PinType::Data;

		// ピンの持ち主であるノードの識別番号
		int nodeID = 0;

		// 識別番号
		int id = 0;
	};
}

#endif // !POKARINENGINE_PIN_H_INCLUDED
