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
		HINSTANCE hInstance;
		LPSTR lpCmdLine;
		int nCmdShow;
		Scene* startScene;
		int width;
		int height;
		int left;
		int top;
		int targetFramesPerSecond = 144;
		bool enablePhysics = true;
		bool physicsStartDebugMode = false;
		float physicsUpdateTime = 0.02f;
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

	static Window* instance;
	static std::string className;
	
	std::vector<Tickable*> tickables;
	HWND windowHandle;
	HINSTANCE hInstance;
	Physics* physics = nullptr;
	Gfx* gfx = nullptr;
	Scene* currentScene;

	int width;
	int height;
};
