#pragma once
#include "RhyWin.h"
#include <string>
#include <vector>

#include "Keys.h"
#include "Time.h"
#include "Mouse.h"

class Gfx;
class Physics;
class Scene;
class Tickable;

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
		bool enablePhysics = true;
		bool physicsStartDebugMode = false;
		float physicsUpdateTime = 0.02f;
		bool mouseCaptured = false;
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
	/// Executes the main game loop.
	/// </summary>
	/// <returns>The exit code of the program.</returns>
	int MainLoop();

	/// <summary>
	/// Adds a tickable which gets updated at the start of a new frame.
	/// </summary>
	void AddTickable(Tickable* tickable);

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

	static Window* instance;
	static std::string className;

	bool capturingMouse;

	int width;
	int height;
	RECT beforeFullscreenWindowPos;
	bool inFullscreen;
	
	std::vector<Tickable*> tickables;
	HWND windowHandle;
	HINSTANCE hInstance;
	Physics* physics = nullptr;
	Gfx* gfx = nullptr;
	Scene* currentScene;
};
