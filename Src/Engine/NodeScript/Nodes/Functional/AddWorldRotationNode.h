/**
* @file AddWorldRotationNode.h
*/
#ifndef ADDWORLDROTATION_H_INCLUDED
#define ADDWORLDROTATION_H_INCLUDED

#include "FunctionalNode.h"

#include "../../../Math/Vector.h"

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
			inputRotationPin = CreatePin(PinType::Data);
		}

		/// <summary>
		/// データピンを表示する
		/// </summary>
		void RenderDataPin() override;

	private: // ------------------------------ 情報 -----------------------------

		// 回転角度入力用データピンの識別番号
		int inputRotationPin = 0;

		// 回転量(度数法)
		Vector3 rotationDeg = { 0, 0, 0 };
	};
}

#endif // !ADDWORLDROTATION_H_INCLUDED
