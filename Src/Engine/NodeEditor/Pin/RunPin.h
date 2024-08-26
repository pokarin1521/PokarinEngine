/**
* @file RunPin.h
*/
#ifndef POKARINENGINE_RUNPIN_H_INCLUDED
#define POKARINENGINE_RUNPIN_H_INCLUDED

#include "Pin.h"

namespace PokarinEngine
{
	// -----------------------
	// 前方宣言
	// -----------------------

	class RunPin;

	// ---------------------------------
	// 型の別名を定義
	// ---------------------------------

	using RunPinPtr = std::shared_ptr<RunPin>;

	/// <summary>
	/// 実行用ピン
	/// </summary>
	class RunPin : public Pin
	{
	public: // ----------- コンストラクタ・デストラクタ -------------

		// 基底クラスの全てのコンストラクタを継承する
		using Pin::Pin;

		~RunPin() = default;

	public: // --------------------- ピンの種類 ---------------------

		/// <summary>
		/// ピンの種類を取得する
		/// </summary>
		/// <returns> ピンの種類 </returns>
		PinType GetType() const override
		{
			return  PinType::Run;
		}

	public: // ----------------------- リンク -----------------------

		/// <summary>
		/// ピン同士をリンクした時の処理
		/// </summary>
		/// <param name="pin"> リンクさせるピン </param>
		void LinkPin(Pin& pin) override;

		/// <summary>
		/// <para> リンク先のノードを実行する </para>
		/// <para> 出力用ピンでなければ実行できない </para>
		/// </summary>
		void RunLinkNode();
	};
}

#endif // !POKARINENGINE_RUNPIN_H_INCLUDED
