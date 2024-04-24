/**
* @file TestNode.h
*/
#ifndef TESTNODE_H_INCLUDED
#define TESTNODE_H_INCLUDED

#include "Functional/FunctionalNode.h"

#include "../../Time.h"

#include "../../GameObject.h"

namespace PokarinEngine
{
	/// <summary>
	/// テスト用ノード
	/// </summary>
	class TestNode : public FunctionalNode
	{
	public:

		TestNode() = default;
		~TestNode() = default;

	public:

		void Run() override
		{
			GetGameObject().transform->rotation.y += Time::DeltaTime();
		}

	private:

		void CreateDataPin() override
		{
			inputPin = CreatePin(PinType::Data);
			outputPin = CreatePin(PinType::Data);
		}

		void RenderDataPin() override
		{
			BeginDataPin(inputPin, PinAttribute::Input);
			ImGui::Text("Input");
			EndPin(PinAttribute::Input);
			
			PinSameLin();
			
			BeginDataPin(outputPin, PinAttribute::Output);
			ImGui::Text("Output");
			EndPin(PinAttribute::Output);
		}

	private:

		int inputPin = 0;
		int outputPin = 0;
	};
}

#endif // !TESTNODE_H_INCLUDED
