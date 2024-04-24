/**
* @file AddRotation.h
*/
#ifndef ADDROTATION_H_INCLUDED
#define ADDROTATION_H_INCLUDED

#include "FunctionalNode.h"

#include "../../../Math/Vector.h"

namespace PokarinEngine
{
	/// <summary>
	/// 回転角度を加算するノード
	/// </summary>
	class AddRotationNode : public FunctionalNode
	{
	public: // ----------------- コンストラクタ・デストラクタ --------------------

		AddRotationNode() = default;
		~AddRotationNode() = default;

	public: // ----------------------------- 実行処理 ----------------------------

		/// <summary>
		/// 実行処理
		/// </summary>
		void Run() override;

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

		// 回転角度の加算値
		Vector3 addRotation = { 0, 0, 0 };
	};
}

#endif // !ADDROTATION_H_INCLUDED
