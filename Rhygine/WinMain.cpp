#include "RhyWin.h"

#include "Window.h"
#include "RhyException.h"

#include "TestScene.h"
#include "LightTestScene.h"
#include "BulletTestScene.h"

int CALLBACK WinMain(
	HINSTANCE hInstance,
	HINSTANCE hPrevInstance,
	LPSTR     lpCmdLine,
	int       nCmdShow)
{
	try {

		// Get all information needed for the start and put it into WindowDefinition.
		BulletTestScene scene;
		Window::WindowDefinition winDef;
		winDef.hInstance = hInstance;
		winDef.lpCmdLine = lpCmdLine;
		winDef.nCmdShow = nCmdShow;
		winDef.startScene = &scene;
		winDef.width = 1600;
		winDef.height = 800;
		winDef.top = 100;
		winDef.left = 100;
		winDef.targetFramesPerSecond = 144;
		winDef.enablePhysics = true;
		winDef.physicsStartDebugMode = true;

		// Create the window.
		Window window(winDef);

		// Execute the main loop and return the exit code as an exit code.
		return window.MainLoop();
	}
	// catch any exceptions and display a MessageBox with the exception info.
	catch (RhyException& rhy) {
		MessageBoxA(nullptr, rhy.what(), "Rhygine Failure", MB_OK | MB_ICONEXCLAMATION);
	}
	catch (std::exception exep) {
		MessageBoxA(nullptr, exep.what(), "Uncaught Exception", MB_OK | MB_ICONEXCLAMATION);
	}
	catch (...) {
		MessageBoxA(nullptr, "Unknown error...", "Uncaught Exception", MB_OK | MB_ICONEXCLAMATION);
	}
	// If an exception occured simply return the exit code -1
	return -1;
}
