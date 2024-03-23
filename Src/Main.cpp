/**
* @file Main.cpp
*/
#include "Engine/Engine.h"

#pragma warning(push)
#pragma warning(disable:4005)
#include <Windows.h>
#pragma warning(pop)

/// <summary>
/// エントリーポイント
/// </summary>
int WINAPI WinMain(
	_In_ HINSTANCE hInstance,
	_In_opt_ HINSTANCE hPrevInstance,
	_In_ LPSTR lpCmdLine,
	_In_ int nShowCmd)
{
	PokarinEngine::Engine engine;
	return engine.Run();
}