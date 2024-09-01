/**
* @file Pin.h
*/
#ifndef POKARINENGINE_PIN_H_INCLUDED
#define POKARINENGINE_PIN_H_INCLUDED

#include "Json/UsingNameJson.h"

#include <memory>
#include <unordered_map>
#include <typeindex>

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

	/// <summary>
	/// ピンの入出力属性
	/// </summary>
	enum class PinAttribute
	{
		Input,  // 入力用
		Output, // 出力用
	};

	/// <summary>
	/// ピンの種類
	/// </summary>
	enum class PinType
	{
		Run, // 実行用
		Data, // 値用
	};

	/// <summary>
	/// ノードで使うピンの基底クラス
	/// </summary>
	class Pin
	{
	public: // ------------------- コンストラクタ・デストラクタ --------------------

		Pin(Node& node, int pinID, const std::string& pinName, PinAttribute pinAttribute)
			:ownerNode(&node), id(pinID), name(pinName), attribute(pinAttribute) {}

		~Pin() = default;

	public: // -------------------------- 型の別名を定義 ---------------------------

		// <リンク識別番号, リンク中のピン>
		using LinkList = std::unordered_map<int, Pin*>;

	public: // ------------------------------- 表示 --------------------------------

		/// <summary>
		/// ピンの表示を開始する
		/// </summary>
		void Begin() const;

		/// <summary>
		/// ピンの表示を終了する
		/// </summary>
		void End() const;

	public: // -------------------------------- 種類 -------------------------------

		/// <summary>
		/// ピンの種類を取得する
		/// </summary>
		/// <returns> ピンの種類 </returns>
		virtual PinType GetType() const = 0;

		/// <summary>
		/// ピンの型を取得する
		/// </summary>
		/// <returns> ピンの型(typeid) </returns>
		virtual std::type_index GetClass() const = 0;

		/// <summary>
		/// ピンの入出力属性を取得する
		/// </summary>
		/// <returns> ピンの入出力属性 </returns>
		PinAttribute GetAttribute() const
		{
			return attribute;
		}

	public: // ------------------------------- 持ち主 ------------------------------

		/// <summary>
		/// 持ち主であるノードを取得する
		/// </summary>
		/// <returns> 持ち主であるノード </returns>
		Node& GetOwnerNode() const
		{
			return *ownerNode;
		}

	public: // ------------------------------ 識別番号 -----------------------------

		/// <summary>
		/// 識別番号を取得する
		/// </summary>
		/// <returns> ピンの識別番号 </returns>
		int GetID() const
		{
			return id;
		}

	public: // -------------------------------- 名前 -------------------------------

		/// <summary>
		/// 名前を取得する
		/// </summary>
		/// <returns> ピンの名前 </returns>
		const std::string& GetName() const
		{
			return name;
		}

	public: // ------------------------------- リンク ------------------------------

		/// <summary>
		/// リンク中のピン配列を取得する
		/// </summary>
		/// <returns> [リンク識別番号, リンク中のピン] </returns>
		const LinkList& GetLinkList() const
		{
			return linkList;
		}

		/// <summary>
		/// ピンをリンクさせる
		/// </summary>
		/// <param name="[in] linkID"> リンク識別番号 </param>
		/// <param name="[in] pin"> リンクさせるピン </param>
		virtual void Link(int linkID, Pin& pin) = 0;

		/// <summary>
		/// ピン同士のリンクを解除する
		/// </summary>
		/// <param name="[in] linkID"> リンク識別番号 </param>
		void UnLink(int linkID);

		/// <summary>
		/// 全てのピン同士のリンクを解除する
		/// </summary>
		void UnLinkAll();

	public: // -------------------------------- Json -------------------------------

		/// <summary>
		/// 情報をJson型に格納する
		/// </summary>
		/// <param name="[out] json"> 情報を格納するJson型 </param>
		void ToJson(Json& json) const;

		/// <summary>
		/// 情報をJson型から取得する
		/// </summary>
		/// <param name="[in] json"> 情報を格納しているJson型 </param>
		void FromJson(const Json& json);

	protected: // ------------------------ リンク中のピン --------------------------

		// リンク中のピン管理用配列
		// <リンク識別番号, リンク中のピン>
		LinkList linkList;

	private: // ------------------------------- Json -------------------------------

		/// <summary>
		/// ピン別の情報をJson型に格納する
		/// </summary>
		/// <param name="[out] json"> 情報を格納するJson型 </param>
		virtual void PinToJson(Json& json) const {}

		/// <summary>
		/// ピン別の情報をJson型から取得する
		/// </summary>
		/// <param name="[in] json"> 情報を格納しているJson型 </param>
		virtual void PinFromJson(const Json& json) {}

	private: // ------------------------------- 情報 -------------------------------

		// ピンの持ち主であるノードの識別番号
		Node* ownerNode = nullptr;

		// 識別番号
		int id = 0;

		// 名前
		std::string name = "";

		// ピンの入出力属性
		PinAttribute attribute = PinAttribute::Input;
	};
}

#endif // !POKARINENGINE_PIN_H_INCLUDED
