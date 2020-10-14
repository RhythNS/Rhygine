#include "RhyWin.h"

#include "Window.h"
#include "RhyException.h"

#include "TestScene.h"
#include "LightTestScene.h"

int CALLBACK WinMain(
	HINSTANCE hInstance,
	HINSTANCE hPrevInstance,
	LPSTR     lpCmdLine,
	int       nCmdShow)
{
	try {

		TestScene scene;
		
		Window::WindowDefinition winDef = { 0 };
		winDef.hInstance = hInstance;
		winDef.lpCmdLine = lpCmdLine;
		winDef.nCmdShow = nCmdShow;
		winDef.startScene = &scene;
		winDef.width = 1600;
		winDef.height = 800;
		winDef.top = 100;
		winDef.left = 100;

		Window window(winDef);

		return window.MainLoop();
	}
	catch (RhyException& rhy) {
		MessageBoxA(nullptr, rhy.what(), "Rhygine Failure", MB_OK | MB_ICONEXCLAMATION);
	}
	catch (std::exception exep) {
		MessageBoxA(nullptr, exep.what(), "Uncaught Exception", MB_OK | MB_ICONEXCLAMATION);
	}
	catch (...) {
		MessageBoxA(nullptr, "Unknown error...", "Uncaught Exception", MB_OK | MB_ICONEXCLAMATION);
	}
	return -1;
}



