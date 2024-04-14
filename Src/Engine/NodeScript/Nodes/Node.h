/**
* @file Node.h
*/
#ifndef NODE_H_INCLUDED
#define NODE_H_INCLUDED

#include "../../UsingNames/UsingNodeEditor.h"

#include <string>
#include <vector>
#include <unordered_set>
#include <memory>

namespace PokarinEngine
{
	/// <summary>
	/// ノードの基底クラス
	/// </summary>
	class Node
	{
	public: // ------------------ コンストラクタ・デストラクタ -------------------

		Node() = default;
		virtual ~Node() = default;

	public: // ------------------------------- 制御 ------------------------------

		/// <summary>
		/// ノード作成時の処理
		/// </summary>
		/// <param name="nodeEditor"> 持ち主であるノードエディタ </param>
		void CreateNode(NodeEditor* nodeEditor);

		/// <summary>
		/// ノードを表示する
		/// </summary>
		void Render();

	public: // -------------------------- 識別番号の設定 --------------------------

		/// <summary>
		/// 未設定の場合だけノードの識別番号を設定する
		/// </summary>
		void SetID_OnlyOnce(int setID);

	public: // -------------------------------- 名前 -------------------------------

		// ノードの名前
		inline static const char* name = "No Name";

	protected: // ----------------------- 識別番号の取得 -------------------------

		/// <summary>
		/// ノードの識別番号を取得する
		/// </summary>
		/// <returns> ノードの識別番号 </returns>
		int GetID() const
		{
			return id;
		}

		/// <summary>
		/// 重複しない入出力用ピンの識別番号を取得する
		/// </summary>
		/// <returns> 重複しない識別番号 </returns>
		int GetSinglePinID();

	protected: // ------------------------ ノード表示用 --------------------------

		/// <summary>
		/// ノードの入力用ピンを設定する
		/// </summary>
		/// <param name="pinID"> 設定するピンの識別番号 </param>
		/// <param name="pinName"> 設定するピンの名前 </param>
		void SetInputPin(int pinID, const char* pinName);

		/// <summary>
		/// ノードの出力用ピンを設定する
		/// </summary>
		/// <param name="pinID"> 設定するピンの識別番号 </param>
		/// <param name="pinName"> 設定するピンの名前 </param>
		void SetOutputPin(int pinID, const char* pinName);

		/// <summary>
		/// ノードの入出力用ピンを設定する
		/// </summary>
		/// <param name="inputID"> 入力用ピンの識別番号 </param>
		/// <param name="outputID"> 出力用ピンの識別番号 </param>
		/// <param name="inputName"> 入力用ピンの名前 </param>
		/// <param name="outputName"> 出力用ピンの名前 </param>
		void SetInOutPin(int inputID, int outputID, const char* inputName, const char* outputName);

	private: // ------------------------------ 制御 ------------------------------

		/// <summary>
		/// ノードの情報を表示する
		/// </summary>
		virtual void RenderInfo() = 0;

		/// <summary>
		/// 初期化
		/// </summary>
		virtual void Initialize() {}

	private: // ------------------------------ 情報 ------------------------------

		// ノードの識別番号が設定済みならtrue
		bool isSetID = false;

		// ノードの識別番号
		int id = 0;

		// 持ち主であるノードエディタ
		NodeEditor* owner = nullptr;
	};

} // namespace PokarinEngine

#endif // !NODE_H_INCLUDED
