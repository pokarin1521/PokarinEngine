/**
* @file TestNode.h
*/
#ifndef TESTNODE_H_INCLUDED
#define TESTNODE_H_INCLUDED

#include "EventNode.h"

namespace PokarinEngine
{
	/// <summary>
	/// テスト用ノード
	/// </summary>
	class TestNode : public EventNode
	{
	public:

		TestNode() = default;
		~TestNode() = default;

	public:

		inline static const char* name = "Test";

	private:

		void Initialize() override
		{
			inputPin = GetSinglePinID();
			outputPin = GetSinglePinID();
		}

		void RenderNode() override
		{
			SetInOutPin(inputPin, outputPin, "Input", "Output");
		}

	private:

		int inputPin = 0;

		int outputPin = 0;
	};
}

#endif // !TESTNODE_H_INCLUDED
