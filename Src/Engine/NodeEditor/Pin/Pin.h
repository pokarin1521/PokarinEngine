/**
* @file Pin.h
*/
#ifndef POKARINENGINE_PIN_H_INCLUDED
#define POKARINENGINE_PIN_H_INCLUDED

#include "ImGui/imnodes.h"

#include <memory>
#include <unordered_set>

namespace PokarinEngine
{
	// ------------------------
	// 前方宣言
	// ------------------------

	class Node;
	class Pin;

	// ------------------------
	// 型の別名を定義
	// ------------------------

	using PinPtr = std::shared_ptr<Pin>;
	using LinkList = std::unordered_set<Pin*>;

	/// <summary>
	/// ピンの種類
	/// </summary>
	enum class PinType
	{
		/* ピンごとに形を変えたいので、形用の番号と同じにする */

		// データピン
		Data = ImNodesPinShape_CircleFilled,   
		
		// 実行ピン
		Run = ImNodesPinShape_TriangleFilled,
	};

	/// <summary>
	/// ピンの入出力属性
	/// </summary>
	enum class PinAttribute
	{
		Input,  // 入力用
		Output, // 出力用
	};

	/// <summary>
	/// ノードで使うピンの基底クラス
	/// </summary>
	class Pin
	{
	public: // ------------------- コンストラクタ・デストラクタ --------------------

		Pin(Node& node, int pinID, PinAttribute pinAttribute)
			:ownerNode(&node), id(pinID), attribute(pinAttribute) {}

		~Pin() = default;

	public: // ------------------------------- 表示 --------------------------------

		/// <summary>
		/// ピンの表示を開始する
		/// </summary>
		void Begin();

		/// <summary>
		/// ピンの表示を終了する
		/// </summary>
		void End();

	public: // ----------------------------- 情報の取得 ----------------------------

		/// <summary>
		/// ピンの種類を取得する
		/// </summary>
		/// <returns> ピンの種類 </returns>
		virtual PinType GetType() const = 0;

		/// <summary>
		/// ピンの入出力属性を取得する
		/// </summary>
		/// <returns> ピンの入出力属性 </returns>
		PinAttribute GetAttribute() const
		{
			return attribute;
		}

		/// <summary>
		/// 持ち主であるノードを取得する
		/// </summary>
		/// <returns> 持ち主であるノード </returns>
		Node& GetOwnerNode() const
		{
			return *ownerNode;
		}

		/// <summary>
		/// リンク中のピン配列を取得する
		/// </summary>
		/// <returns> リンク中のピン管理用配列 </returns>
		const LinkList& GetLinkList() const
		{
			return linkList;
		}

		/// <summary>
		/// 識別番号を取得する
		/// </summary>
		/// <returns> ピンの識別番号 </returns>
		int GetID() const
		{
			return id;
		}

	public: // ------------------------------- リンク ------------------------------

		/// <summary>
		/// ピン同士をリンクさせる
		/// </summary>
		/// <param name="pin"> リンクさせるピン </param>
		virtual void LinkPin(Pin& pin) = 0;

		/// <summary>
		/// ピン同士のリンクを解除する
		/// </summary>
		/// <param name="pin"> リンク中のピン </param>
		void UnLinkPin(Pin& pin)
		{
			linkList.erase(&pin);
		}

	protected: // ------------------------ リンク中のピン --------------------------

		// リンク中のピン管理用配列
		LinkList linkList;

	private: // ------------------------------- 情報 -------------------------------

		// ピンの持ち主であるノードの識別番号
		Node* ownerNode = nullptr;

		// 識別番号
		int id = 0;

		// ピンの入出力属性
		PinAttribute attribute = PinAttribute::Input;
	};
}

#endif // !POKARINENGINE_PIN_H_INCLUDED
