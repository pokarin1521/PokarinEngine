/**
* @file NodeEditor.cpp
*/
#include "NodeEditor.h"

#include "ImGui/imgui.h"
#include "ImGui/imnodes.h"

#include "../GameObject.h"

#include "NodeScript.h"

namespace PokarinEngine
{
	/// <summary>
	/// コンストラクタ
	/// </summary>
	/// <param name="object"> 持ち主になるゲームオブジェクト </param>
	NodeEditor::NodeEditor(GameObject* ownerObject)
	{
		// 持ち主を設定
		owner = ownerObject;

		// ノードエディタの名前を設定
		// 分かりやすいように「持ち主の名前 + #持ち主の識別番号」にする
		name = owner->GetName();
		name += " #" + std::to_string(owner->GetID());
	}

	/// <summary>
	/// 更新
	/// </summary>
	/// <returns>
	/// <para> true : ノードエディタのウィンドウが選択されている </para>
	/// <para> false : ノードエディタのウィンドウが選択されていない </para>
	/// </returns>
	bool NodeEditor::Update()
	{
		// ウィンドウが開いてないなら何もしない
		if (!isOpen)
		{
			return false;
		}

		// ImGuiウィンドウを作成
		ImGui::Begin(name.c_str(), &isOpen);
		{
			// ImGuiウィンドウを登録
			if (!imGuiWindow)
			{
				imGuiWindow = ImGui::GetCurrentWindow();
			}

			// フォーカスされてないなら中断
			if (!ImGui::IsWindowFocused())
			{
				ImGui::End();
				return false;
			}

			// ノードエディタを作成
			ImNodes::BeginNodeEditor();
			{
				ImNodes::EndNodeEditor();
			}

			ImGui::End();
		}

		return true;
	}

} // namespace PokarinEngine