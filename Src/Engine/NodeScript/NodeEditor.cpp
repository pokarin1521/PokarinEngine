/**
* @file NodeEditor.cpp
*/
#include "NodeEditor.h"

#include "ImGui/imgui.h"

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

		nodeEditorContext = ImNodes::EditorContextCreate();
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
		// ウィンドウが選択された時に処理する
		if(ImGui::Begin(name.c_str(), &isOpen))
		{
			// ImGuiウィンドウを登録
			if (!imGuiWindow)
			{
				imGuiWindow = ImGui::GetCurrentWindow();
			}

			ImNodes::EditorContextSet(nodeEditorContext);

			// ノードエディタを作成
			ImNodes::BeginNodeEditor();
			{
				ImNodes::EndNodeEditor();
			}

			ImNodes::EditorContextSet(nullptr);

			ImGui::End();

			// 選択されているのでtrueを返す
			return true;
		}


		// 選択状態に関わらずBegin関数が呼ばれているので
		// End関数を呼ぶ
		ImGui::End();

		// 選択されてないのでfalseを返す
		return false;
	}

} // namespace PokarinEngine