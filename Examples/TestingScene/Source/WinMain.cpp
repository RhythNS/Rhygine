#include <RhyWin.h>
#include <RhyException.h>

#include <Core/Window.h>

#include <Scenes/TestScene.h>
#include <Scenes/LightTestScene.h>
#include <Scenes/BulletTestScene.h>
#include <Scenes/MemoryLeakTestScene.h>
#include <Scenes/NewFeatureTestScene.h>
#include <Scenes/OptScene.h>
#include <Scenes/PresentScene.h>

#include <Bullet/Physics.h>
#include <imgui/ImGuiModule.h>
#include <Model3D/Model3DModule.h>

int CALLBACK WinMain(
	_In_ HINSTANCE hInstance,
	_In_opt_ HINSTANCE hPrevInstance,
	_In_ LPSTR lpCmdLine,
	_In_ int nCmdShow)
{
	/*
	// Memory leak breakpoint setter
	_CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF);
	_CrtSetBreakAlloc(2073);
	*/
	try {

		// BulletTestScene* scene = new BulletTestScene();
		// TestScene* scene = new TestScene();
		// LightTestScene* scene = new LightTestScene();
		// OptScene* scene = new OptScene();
		PresentScene* scene = new PresentScene();
		// MemoryLeakTestScene* scene = new MemoryLeakTestScene();
		// NewFeatureTestScene* scene = new NewFeatureTestScene();

		// Get all information needed for the start and put it into WindowDefinition.
		Window::WindowDefinition winDef;
		winDef.hInstance = hInstance;
		winDef.lpCmdLine = lpCmdLine;
		winDef.nCmdShow = nCmdShow;
		winDef.startScene = scene;
		winDef.width = 1600;
		winDef.height = 800;
		winDef.top = 100;
		winDef.left = 100;
		winDef.targetFramesPerSecond = -1;
		winDef.enableVSync = false;
		winDef.coreCountOverride = -1;
		winDef.modules.push_back(new Physics(0.02f));
		winDef.modules.push_back(new ImGuiModule());
		winDef.modules.push_back(new Model3DModule());

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
