/**
* @file EventNode.h
*/
#ifndef EVENTNODE_H_INCLUDED
#define EVENTNODE_H_INCLUDED

#include "Node.h"

namespace PokarinEngine
{
	class EventNode : public Node
	{
	public:

		EventNode() = default;
		~EventNode() = default;

	public:

		/// <summary>
		/// èâä˙âª
		/// </summary>
		void Initialize() override
		{
			auto& owner = GetOwner();
			inputPin = owner.GetSinglePinID();
			outputPin = owner.GetSinglePinID();
		}

		void RenderNode() override
		{
			NodeEditor& owner = GetOwner();

			ImNodes::BeginNode(GetID());
			{
				SetTitle("Event");

				ImNodes::BeginOutputAttribute(outputPin);
				ImGui::Text("Out");
				ImNodes::EndOutputAttribute();

				ImNodes::BeginInputAttribute(inputPin);
				ImGui::Text("In");
				ImNodes::EndInputAttribute();

				ImNodes::EndNode();
			}
		}

	private:

		int inputPin = 0;
		int outputPin = 0;
	};

} // namespace PokarinEngine

#endif // !EVENTNODE_H_INCLUDED
