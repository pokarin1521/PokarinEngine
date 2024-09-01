/**
* @file AddWorldPositionNode.h
*/
#ifndef POKARINENGINE_ADDWORLDPOSITIONNODE_H_INCLUDED
#define POKARINENGINE_ADDWORLDPOSITIONNODE_H_INCLUDED

#include "FunctionalNode.h"

#include "../../../UsingNames/UsingDataPin.h"

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
		void CreateDataPin() override;

		/// <summary>
		/// データピンを表示する
		/// </summary>
		void RenderDataPin() override;

	private: // ------------------------------ 情報 ------------------------------

		// 移動量入力用データピン
		DataPinPtr<Vector3> inputTranslatePin;
	};
}

#endif // !POKARINENGINE_ADDWORLDPOSITIONNODE_H_INCLUDED
