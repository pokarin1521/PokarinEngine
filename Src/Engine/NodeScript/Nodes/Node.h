/**
* @file Node.h
*/
#ifndef NODE_H_INCLUDED
#define NODE_H_INCLUDED

#include "ImGui/imgui.h"
#include "ImGui/imnodes.h"

#include "../../Window.h"
#include "../../InputManager.h"

#include <string>
#include <vector>
#include <unordered_set>
#include <memory>

namespace PokarinEngine
{
	// -----------------------
	// 前方宣言
	// -----------------------

	class Node;
	class NodeEditor;

	// -----------------------------
	// 型の別名を定義
	// -----------------------------

	using NodePtr = std::shared_ptr<Node>;
	using NodeList = std::unordered_set<NodePtr>;

	/// <summary>
	/// ノードの基底クラス
	/// </summary>
	class Node
	{
	public: // ------------------ コンストラクタ・デストラクタ -------------------

		Node() = default;
		virtual ~Node() = default;

	public: // ---------------------------- ノード制御 ---------------------------

		/// <summary>
		/// 初期化
		/// </summary>
		virtual void Initialize() {}

		/// <summary>
		/// ノードを表示する
		/// </summary>
		virtual void RenderNode() = 0;

		/// <summary>
		/// ノード追加時の処理
		/// </summary>
		/// <param name="nodeEditor"> 持ち主であるノードエディタ </param>
		void AddNode(NodeEditor* nodeEditor)
		{
			// 持ち主であるノードエディタを設定
			owner = nodeEditor;

			// マウスカーソルの位置にノードを設置する
			ImVec2 mousePos = Input::Mouse::GetScreenPos(WindowID::NodeScript);
			ImNodes::SetNodeScreenSpacePos(id, mousePos);

			// 初期化
			Initialize();
		}
	
	public: // -------------------------- 識別番号の設定 --------------------------

		/// <summary>
		/// 未設定の場合だけノードの識別番号を設定する
		/// </summary>
		void SetID_OnlyOnce(int setID)
		{
			// 設定済みなら何もしない
			if (isSetID)
			{
				return;
			}

			// 識別番号を設定
			id = setID;

			// 設定済みにする
			isSetID = true;
		}

	protected: // ------------------------- 情報の取得 ---------------------------

		/// <summary>
		/// ノードの識別番号を取得する
		/// </summary>
		/// <returns> ノードの識別番号 </returns>
		int GetID() const
		{
			return id;
		}

		/// <summary>
		/// 持ち主であるノードエディタを取得する
		/// </summary>
		/// <returns> 持ち主であるノードエディタ </returns>
		NodeEditor& GetOwner()
		{
			return *owner;
		}

	protected: // ------------------------ ノード作成用 --------------------------

		/// <summary>
		/// ノードのタイトルを設定する
		/// </summary>
		/// <param name="title"> タイトル </param>
		void SetTitle(const char* title)
		{
			ImNodes::BeginNodeTitleBar();
			ImGui::Text(title);
			ImNodes::EndNodeTitleBar();
		}

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
