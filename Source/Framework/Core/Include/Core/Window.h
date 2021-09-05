#pragma once
#include <RhyWin.h>
#include <string>
#include <vector>

#include <Input/Keys.h>
#include <Input/Mouse.h>
#include <Core/Time.h>

class Module;
class IWin32MessageHandler;
class Gfx;
class Physics;
class Scene;
class Tickable;
class TaskManager;
class ModelLoader;

/// <summary>
/// Representation of a window in win32. Holds the Gfx, manages messages and
/// has the main game loop.
/// </summary>
class Window
{
public:
	/// <summary>
	/// Used for constructing a window.
	/// </summary>
	struct WindowDefinition {
	public:
		HINSTANCE hInstance = 0;
		LPSTR lpCmdLine{};
		int nCmdShow = 0;
		Scene* startScene = nullptr;
		int width = 1600;
		int height = 900;
		int left = 0;
		int top = 0;
		LPCSTR windowName = "Rhygine";
		bool enableVSync = true;
		int targetFramesPerSecond = 144;
		bool mouseCaptured = false;
		int coreCountOverride = -1;
		std::vector<Module*> modules;
	};

	Window() = delete;
	Window(WindowDefinition definition);
	~Window();
	
 	/// <summary>
 	/// Singleton to the window instance.
 	/// </summary>
 	static Window* GetInstance();
	/// <summary>
	/// Gets a Gfx instance.
	/// </summary>
	Gfx* GetGfx();
	/// <summary>
	/// Gets a reference to the current scene.
	/// </summary>
	Scene* GetCurrentScene();
	/// <summary>
	/// Gets a reference to the task manager.
	/// </summary>
	/// <returns></returns>
	TaskManager* GetTaskManager();
	/// <summary>
	/// Gets the window handle.
	/// </summary>
	HWND* GetWindowHandle();
	/// <summary>
	/// Gets the HINSTANCE.
	/// </summary>
	HINSTANCE* GetHInstance();
	/// <summary>
	/// Gets the width of the window.
	/// </summary>
	int GetWidth();
	/// <summary>
	/// Gets the height of the window.
	/// </summary>
	/// <returns></returns>
	int GetHeight();
	/// <summary>
	/// Sets the title of the window.
	/// </summary>
	void SetTitle(LPCSTR lpString);
	/// <summary>
	/// When enabled the mouse pointer is unable to move beyond the borders of the window.
	/// </summary>
	void SetCaptureMouse(bool enable);
	/// <summary>
	/// Checks if the mouse is currently bound to the window.
	/// </summary>
	bool IsCapturingMouse();

	/// <summary>
	/// Enables or disables fullscreen mode.
	/// </summary>
	void SetFullscreen(bool enable);
	/// <summary>
	/// Returns wheter the window is currently in fullscreen mode.
	/// </summary>
	bool IsInFullscreen();
	void Resize(int posX, int posY, int width, int height);

	/// <summary>
	/// Changes the scene to after the current frame.
	/// </summary>
	/// <param name="toChangeTo">The scene to change to.</param>
	void ChangeScene(Scene* toChangeTo);

	/// <summary>
	/// Executes the main game loop.
	/// </summary>
	/// <returns>The exit code of the program.</returns>
	int MainLoop();

	/// <summary>
	/// Adds a tickable which gets updated at the start of a new frame.
	/// </summary>
	void AddTickable(Tickable* tickable);

	void AddModule(Module* module);

	void SortModules();

	template <class T>
	T* GetModule()
	{
		T* t = nullptr;
		for (int i = 0; i < modules.size(); i++)
			if (t = dynamic_cast<T*>(modules[i]))
				return t;

		return nullptr;
	}

	Keys keys;
	Time time;
	Mouse mouse;
private:
	/// <summary>
	/// Message passthrough which is given to win32.
	/// </summary>
	static LRESULT CALLBACK ProcessPassthrough(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);
	/// <summary>
	/// Process all messages from win32.
	/// </summary>
	LRESULT ProcessMessage(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);
	/// <summary>
	/// Captures the mouse to the window.
	/// </summary>
	void CaptureMouse();

	void InitModule(Module* module);

	static Window* instance;
	static std::string className;

	bool capturingMouse;

	int width;
	int height;
	RECT beforeFullscreenWindowPos;
	bool inFullscreen;
	
	std::vector<Module*> modules;
	std::vector<IWin32MessageHandler*> messageHandlers;
	std::vector<Tickable*> tickables;
	HWND windowHandle;
	HINSTANCE hInstance;
	Gfx* gfx = nullptr;
	TaskManager* taskManager = nullptr;
	Scene* currentScene;
	Scene* changeRequest = nullptr;
};
