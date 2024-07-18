/**
* @file AddWorldPositionNode.h
*/
#ifndef ADDWORLDPOSITIONNODE_H_INCLUDED
#define ADDWORLDPOSITIONNODE_H_INCLUDED

#include "FunctionalNode.h"

#include "../../../Math/Vector.h"

namespace PokarinEngine
{
	/// <summary>
	/// 座標を加算するノード
	/// </summary>
	class AddWorldPositionNode : public FunctionalNode
	{
	public: // ----------------- コンストラクタ・デストラクタ --------------------

		AddWorldPositionNode() = default;
		~AddWorldPositionNode() = default;

	private: // ---------------------------- 実行処理 ----------------------------

		/// <summary>
		/// ノード別の実行処理
		/// </summary>
		/// <returns>
		/// <para> true : 次のノードを実行する </para>
		/// <para> false : 次のノードを実行しない </para>
		/// </returns>
		bool RunNode() override;

	private: // --------------------------- データピン ---------------------------

		/// <summary>
		/// データピンを作成する
		/// </summary>
		void CreateDataPin() override
		{
			inputPositionPin = CreatePin(PinType::Data);
		}

		/// <summary>
		/// データピンを表示する
		/// </summary>
		void RenderDataPin() override;

	private: // ------------------------------ 情報 -----------------------------

		// 回転角度入力用データピンの識別番号
		int inputPositionPin = 0;

		// 移動量
		Vector3 translate = { 0, 0, 0 };
	};
}

#endif // !ADDWORLDPOSITIONNODE_H_INCLUDED
