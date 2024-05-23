/**
* @file Node.h
*/
#ifndef NODE_H_INCLUDED
#define NODE_H_INCLUDED

#include "../../UsingNames/UsingNodeEditor.h"
#include "../../UsingNames/UsingGameObject.h"

#include "../Pin/PinType.h"

#include <string>
#include <memory>

namespace PokarinEngine
{
	/// <summary>
	/// ノードの基底クラス
	/// </summary>
	class Node
	{
	public: // ------------------ コンストラクタ・デストラクタ --------------------

		Node() = default;
		virtual ~Node() = default;

	public: // ------------------------------- 作成 -------------------------------

		/// <summary>
		/// ノード作成時の処理
		/// </summary>
		/// <param name="nodeEditor"> 持ち主であるノードエディタ </param>
		/// <param name="nodeID"> ノードの識別番号 </param>
		void CreateNode(NodeEditor& nodeEditor, int nodeID, const std::string& nodeTitle);

	public: // ------------------------------- 制御 -------------------------------

		/// <summary>
		/// 実行処理
		/// </summary>
		void Run();

		/// <summary>
		/// ノードを表示する
		/// </summary>
		void Render();

	public: // ------------------------- 次のノードを設定 -------------------------

		/// <summary>
		/// 次に実行するノードを設定する
		/// </summary>
		/// <param name="node"> 次に実行するノード </param>
		void SetNextNode(Node* node)
		{
			nextNode = node;
		}

	public: // ------------------------- ノードの識別番号 -------------------------

		/// <summary>
		/// ノードの識別番号を取得する
		/// </summary>
		/// <returns> ノードの識別番号 </returns>
		int GetID() const
		{
			return id;
		}

	protected: // ------------------------- ピン作成用 ---------------------------

		/// <summary>
		/// ピンを作成する
		/// </summary>
		/// <param name="pinType"> ピンの種類 </param>
		/// <returns> 作成したピンの識別番号 </returns>
		int CreatePin(PinType pinType);

	protected: // ------------------------- ピン設定用 ---------------------------

		/// <summary>
		/// ピンの入出力属性
		/// </summary>
		enum class PinAttribute
		{
			Input,  // 入力用
			Output, // 出力用
		};

		/// <summary>
		/// データピンの表示を開始する
		/// </summary>
		/// <param name="pinID"> ピンの識別番号 </param>
		/// <param name="pinAttribute"> ピンの属性 </param>
		void BeginDataPin(int pinID, PinAttribute pinAttribute);

		/// <summary>
		/// 実行ピンの表示を開始する
		/// </summary>
		/// <param name="pinID"> ピンの識別番号 </param>
		/// <param name="pinAttribute"> ピンの属性 </param>
		void BeginRunPin(int pinID, PinAttribute pinAttribute);

		/// <summary>
		/// ピンの表示を終了する
		/// </summary>
		/// <param name="pinAttribute"> ピンの属性 </param>
		void EndPin(PinAttribute pinAttribute);

		/// <summary>
		/// 次に設定するピンを同じ行に表示する
		/// </summary>
		void PinSameLin();

	protected: // ---------------------- ゲームオブジェクト ----------------------

		/// <summary>
		/// ノードエディタの持ち主であるゲームオブジェクトを取得する
		/// </summary>
		/// <returns> 持ち主であるゲームオブジェクト </returns>
		GameObject& GetOnwerObject();

	private: // --------------------------- ピンの設定 ---------------------------

		/// <summary>;
		/// ピンの形
		/// </summary>
		enum class PinShape;

		/// <summary>
		/// ピンの設定を開始する
		/// </summary>
		/// <param name="pinID"> ピンの識別番号 </param>
		/// <param name="pinAttribute"> ピンの属性 </param>
		/// <param name="pinShape"> ピンの形 </param>
		void BeginPin(int pinID, PinAttribute pinAttribute, PinShape pinShape);

	private: // ------------------------------ 実行 ------------------------------

		/// <summary>
		/// 次のノードの処理を実行する
		/// </summary>
		void RunNextNode()
		{
			// 次のノードが設定されているなら実行する
			if (nextNode)
			{
				nextNode->Run();
			}
		}

		/// <summary>
		/// ノード別の実行処理
		/// </summary>
		/// <returns>
		/// <para> true : 次のノードの処理を実行する </para>
		/// <para> false : 次のノードの処理を実行しない </para>
		/// </returns>
		virtual bool RunNode() = 0;

	private: // ----------------------------- 初期化 -----------------------------

		/// <summary>
		/// 初期化
		/// </summary>
		virtual void Initialize() = 0;

	private: // ------------------------------ 表示 ------------------------------

		/// <summary>
		/// ピンを表示する
		/// </summary>
		virtual void RenderPin() = 0;

		/// <summary>
		/// タイトルを表示する
		/// </summary>
		void RenderTitle();

	private: // ------------------------------ 情報 ------------------------------

		// ノードの識別番号が設定済みならtrue
		bool isSetID = false;

		// ノードの識別番号
		int id = 0;

		// ノードのタイトル
		std::string title = "";

		// 持ち主であるノードエディタ
		NodeEditor* ownerEditor = nullptr;

		// 次に実行するノード
		Node* nextNode = nullptr;
	};

} // namespace PokarinEngine

#endif // !NODE_H_INCLUDED
