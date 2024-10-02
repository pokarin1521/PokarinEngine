/**
* @file ImGuiHelper.h
*/
#ifndef POKARINENGINE_IMGUIHELPER_H_INCLUDED
#define POKARINENGINE_IMGUIHELPER_H_INCLUDED

#include "ImGui/imgui.h"

#include "Math/Vector.h"
#include "Color.h"
#include "Window.h"

#include <typeindex>
#include <functional>
#include <unordered_map>

namespace PokarinEngine
{
	/// <summary>
	/// ImGui補助用クラス
	/// </summary>
	class ImGuiHelper
	{
	public: // --------------------- 禁止事項 ----------------------

		// コピーコンストラクタの禁止
		ImGuiHelper(const ImGuiHelper&) = delete;

		// 代入の禁止
		ImGuiHelper& operator=(const ImGuiHelper&) = delete;

	public: // ----------------------- 制御 ------------------------

		/// <summary>
		/// ImGuiの初期化
		/// </summary>
		/// <param name="[in] windowID"> 使用するウィンドウの識別番号 </param>
		static void Initialize(WindowID windowID);

		/// <summary>
		/// ImGuiの更新
		/// </summary>
		/// <param name="[in] context"> 使用するImGuiコンテキスト </param>
		static void Update(ImGuiContext* context);

		/// <summary>
		/// ImGuiの描画
		/// </summary>
		/// <param name="[in] context"> 使用するImGuiコンテキスト </param>
		static void Render(ImGuiContext* context);

		/// <summary>
		/// ImGuiの終了とコンテキストの削除
		/// </summary>
		/// <param name="[in] context"> 使用するImGuiコンテキスト </param>
		static void Finalize(ImGuiContext* context);

	public: // ------------------------ 色 -------------------------

		/// <summary>
		/// ImGuiの色設定を開始する
		/// </summary>
		/// <param name="[in] style"> 色を設定したい項目 </param>
		/// <param name="[in] styleColor"> 設定する色 </param>
		static void PushColor(ImGuiCol style, const Color& styleColor);

		/// <summary>
		/// ImGuiの色設定を終了する
		/// </summary>
		static void PopColor();

	public: // ----------------------- 設定 ------------------------

		/// <summary>
		/// ImGuiで使うフォントを設定する
		/// </summary>
		/// <param name="[out] io"> 設定対象 </param>
		static void SetFont(ImGuiIO& io);

	public: // ----------------- チェックボックス ------------------

		/// <summary>
		/// bool型の値をチェックボックスで表示する
		/// </summary>
		/// <param name="[in] name"> 値の名前 </param>
		/// <param name="[in] id_string"> 識別番号(文字列) </param>
		/// <param name="[in,out] data"> 表示する値 </param>
		static void CheckBox(const std::string& name, const std::string& id_string, bool& data);

	public: // ------------------ ドラッグ操作用 -------------------

		/// <summary>
		/// float型の値をドラッグ操作可能なテキストで表示する
		/// </summary>
		/// <param name="[in] name"> 値の名前 </param>
		/// <param name="[in] id_string"> 識別番号(文字列) </param>
		/// <param name="[in] ShowName"> 名前を表示するならtrue </param>
		/// <param name="[in] width"> テキストの幅 </param>
		/// <param name="[in,out] data"> 表示する値 </param>
		/// <param name="[in] posX"> テキストのX位置 </param>
		static void DragText(const std::string& name, const std::string& id_string,
			float width, float& data, float posX = 0)
		{
			Drag<float>(name, id_string, width, data, posX);
		}

		/// <summary>
		/// Vector3型の値をドラッグ操作可能なテキストで表示する
		/// </summary>
		/// <param name="[in] name"> 値の名前 </param>
		/// <param name="[in] id_string"> 識別番号(文字列) </param>
		/// <param name="[in] width"> テキストの幅 </param>
		/// <param name="[in,out] data"> 表示する値 </param>
		/// <param name="[in] posX"> テキストのX位置 </param>
		static void DragText(const std::string& name, const std::string& id_string,
			float width, Vector3& data, float posX = 0)
		{
			Drag<Vector3>(name, id_string, width, data, posX);
		}

	private: // ----------- コンストラクタ・デストラクタ -----------

		/* static関数でアクセスしてほしいので、インスタンス化できないようにする */

		ImGuiHelper() = default;
		~ImGuiHelper() = default;

	private: // ------------------ ドラッグ操作用 ------------------

		/// <summary>
		/// 値をドラッグ操作可能なテキストで表示する
		/// </summary>
		/// <typeparam [in] name="T"> 表示する値の型 </typeparam>
		/// <param name="[in] name"> 値の名前 </param>
		/// <param name="[in] id_string"> 識別番号(文字列) </param>
		/// <param name="[in] width"> テキストの幅 </param>
		/// <param name="[in,out] data"> 値 </param>
		/// <param name="[in] posX"> テキストのX位置 </param>
		template <typename T>
		static void Drag(const std::string& name, const std::string& id_string,
			float width, T& data, float posX = 0)
		{
			// 識別名
			// 非表示にしたいので##を付ける
			const std::string label = "##" + name + id_string;

			// 型指定用配列のイテレータ
			// <型(typeid), 値>
			auto itr = dataTypeList.find(typeid(T));

			if (itr != dataTypeList.end())
			{
				// 同じ行に名前を表示する
				ImGui::Text(name.c_str());
				ImGui::SameLine();

				// ドラッグ操作用スライダーの幅を設定し、表示する
				ImGui::PushItemWidth(width);
				ImGui::DragScalar(label.c_str(), itr->second, &data, dragSpeed, 0, 0, floatFormat);
				ImGui::PopItemWidth();
			}
		}

		/// <summary>
		/// Vector3型の値をドラッグ操作可能なテキストで表示する
		/// </summary>
		template <>
		static void Drag<Vector3>(const std::string& name, const std::string& id_string,
			float width, Vector3& data, float posX)
		{
			// 識別名
			// 非表示にしたいので##を付ける
			const std::string label = "##" + name + id_string;
			
			// 同じ行に名前を表示する
			ImGui::Text(name.c_str());
			ImGui::SameLine();

			// 位置が指定されているなら設定する
			if (posX != 0)
			{
				ImGui::SetCursorPosX(posX);
			}

			// スライダーの幅設定を開始
			ImGui::PushItemWidth(width);
			{
				// 各要素の値を同じ行に表示する
				DragVectorValue("X", label, data.x);

				ImGui::SameLine();
				DragVectorValue("Y", label, data.y);

				ImGui::SameLine();
				DragVectorValue("Z", label, data.z);

				// 幅設定を終了
				ImGui::PopItemWidth();
			}
		}

		/// <summary>
		/// Vector型の値をドラッグ操作用スライダーで表示する
		/// </summary>
		/// <param name="[in] valueName"> 表示する値の要素名 </param>
		/// <param name="[in] vectorLabel"> Vector3の識別名 </param>
		/// <param name="[in,out] value"> 表示する値 </param>
		static void DragVectorValue(const std::string& valueName,
			const std::string& vectorLabel, float& value)
		{
			// 識別用ラベル(非表示にしたいので##)
			const std::string label = "##" + vectorLabel + "." + valueName;

			// 値の名前を表示
			ImGui::Text(valueName.c_str());

			// ドラッグ操作用スライダーを
			// 名前と同じ行に表示する
			ImGui::SameLine();
			ImGui::DragFloat(label.c_str(), &value, dragSpeed, 0, 0, floatFormat);
		}

	private: // --------------------- 色設定用 ---------------------

		// ImGuiの色設定の回数
		inline static int pushColorCount = 0;

	private: // ------------------ ドラッグ操作用 ------------------

		// ドラッグ操作速度
		inline static constexpr float dragSpeed = 0.2f;

		// 表示する小数の桁数を指定するフォーマット
		inline static constexpr const char* floatFormat = "%.2f";

		// ImGuiで型指定に使う値の配列
		inline static const std::unordered_map<std::type_index, ImGuiDataType> dataTypeList = {
			{ typeid(float), ImGuiDataType_Float },
		};
	};
}

#endif // !POKARINENGINE_IMGUIHELPER_H_INCLUDED
