/**
* @file AddWorldRotationNode.h
*/
#ifndef POKARINENGINE_ADDWORLDROTATION_H_INCLUDED
#define POKARINENGINE_ADDWORLDROTATION_H_INCLUDED

#include "FunctionalNode.h"

#include "../../../Math/Vector.h"

#include "../../Pin/DataPin.h"

namespace PokarinEngine
{
	/// <summary>
	/// 回転角度を加算するノード
	/// </summary>
	class AddWorldRotationNode : public FunctionalNode
	{
	public: // ----------------- コンストラクタ・デストラクタ --------------------

		AddWorldRotationNode() = default;
		~AddWorldRotationNode() = default;

	private: // ----------------------------- 実行処理 ----------------------------

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
			inputDegreePin = CreatePin<DataPin<Vector3>>(PinAttribute::Input);
		}

		/// <summary>
		/// データピンを表示する
		/// </summary>
		void RenderDataPin() override;

	private: // ------------------------------ 情報 -----------------------------

		// 回転量(度数法)入力用データピン
		DataPinPtr<Vector3> inputDegreePin;
	};
}

#endif // !POKARINENGINE_ADDWORLDROTATION_H_INCLUDED
