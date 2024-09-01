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

	public: // --------------------- ピンの情報 ---------------------

		/// <summary>
		/// ピンの種類を取得する
		/// </summary>
		/// <returns> ピンの種類 </returns>
		PinType GetType() const
		{
			return PinType::Run;
		}

		/// <summary>
		/// ピンの型を取得する
		/// </summary>
		/// <returns> ピンの型(typeid) </returns>
		std::type_index GetClass() const override
		{
			// 特定の型と比較する場合があるので、thisポインタではなく型を指定する
			return  typeid(RunPin);
		}

	public: // ----------------------- リンク -----------------------

		/// <summary>
		/// ピン同士をリンクした時の処理
		/// </summary>
		/// <param name="[in] linkID"> リンク識別番号 </param>
		/// <param name="[in] pin"> リンクさせるピン </param>
		void Link(int linkID, Pin& pin) override;

		/// <summary>
		/// <para> リンク先のノードを実行する </para>
		/// <para> 出力用ピンでなければ実行できない </para>
		/// </summary>
		void RunLinkNode();
	};
}

#endif // !POKARINENGINE_RUNPIN_H_INCLUDED
