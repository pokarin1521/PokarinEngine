/**
* @file Node.h
*/
#ifndef POKARINENGINE_NODE_H_INCLUDED
#define POKARINENGINE_NODE_H_INCLUDED

#include "../NodeEditor.h"

#include "../../UsingNames/UsingGameObject.h"

#include "../Pin/Pin.h"

#include <string>
#include <memory>
#include <unordered_set>

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

	public: // ----------------------------- 禁止事項 -----------------------------

		/* 識別できなくなるので、禁止する */
		
		// コピーコンストラクタの禁止
		Node(const Node&) = delete;

		// 代入の禁止
		Node& operator=(const Node&) = delete;

	public: // ------------------------------- 作成 -------------------------------

		/// <summary>
		/// ノード作成時の処理
		/// </summary>
		/// <param name="[in] nodeEditor"> 持ち主であるノードエディタ </param>
		/// <param name="[in] nodeID"> ノードの識別番号 </param>
		/// <param name="[in] nodeTitle"> ノードのタイトル </param>
		void CreateNode(NodeEditor& nodeEditor, int nodeID, const std::string& nodeTitle);

	public: // ------------------------------- 制御 -------------------------------

		/// <summary>
		/// 実行処理
		/// </summary>
		virtual void Run() = 0;

		/// <summary>
		/// ノードを表示する
		/// </summary>
		void Render();

	public: // ------------------------- ノードの識別番号 -------------------------

		/// <summary>
		/// ノードの識別番号を取得する
		/// </summary>
		/// <returns> ノードの識別番号 </returns>
		int GetID() const
		{
			return id;
		}

		/// <summary>
		/// ノードの識別番号を文字列で取得する
		/// </summary>
		/// <returns> ノードの識別番号(文字列) </returns>
		std::string GetID_String() const
		{
			return std::to_string(id);
		}

	public: // ---------------------------- ピンの取得 ---------------------------

		/// <summary>
		/// 持っている全てのピンのリンクを解除する
		/// </summary>
		void UnLinkAllPin()
		{
			for (auto& pin : pinList)
			{
				pin->UnLinkAll();
			}
		}
 
	protected: // ------------------------- ピン作成用 ---------------------------

		/// <summary>
		/// ピンを作成する
		/// </summary>
		/// <typeparam name="T"> 作成するピンクラス </typeparam>
		/// <param name="[in] pinAttribute"> ピンの入出力属性 </param>
		/// <returns> 作成したピンの識別番号 </returns>
		template <class T>
		std::shared_ptr<T> CreatePin(PinAttribute pinAttribute)
		{
			// ピンを作成し、配列に追加する
			auto pin = ownerEditor->CreatePin<T>(*this, pinAttribute);
			pinList.push_back(pin);
			
			// 作成したピンを返す
			return pin;
		}

	protected: // ---------------------- ゲームオブジェクト ----------------------

		/// <summary>
		/// ノードエディタの持ち主であるゲームオブジェクトを取得する
		/// </summary>
		/// <returns> 持ち主であるゲームオブジェクト </returns>
		GameObject& GetOnwerObject();

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

		// ピン管理用配列
		std::vector<PinPtr> pinList;
	};

} // namespace PokarinEngine

#endif // !POKARINENGINE_NODE_H_INCLUDED
