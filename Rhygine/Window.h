#pragma once

#include <Windows.h>
#include <string>
#include <vector>

#include "Keys.h"
#include "Time.h"
#include "Mouse.h"

class Gfx;
class Scene;
class Tickable;

class Window
{
public:

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
	};

	Window() = delete;
	Window(WindowDefinition definition);
	~Window();
	
 	static Window* GetInstance();
	Gfx* GetGfx();
	Scene* GetCurrentScene();
	HWND* GetWindowHandle();
	HINSTANCE* GetHInstance();
	int GetWidth();
	int GetHeight();

	int MainLoop();

	void AddTickable(Tickable* tickable);

	Keys keys;
	Time time;
	Mouse mouse;
private:
	static LRESULT CALLBACK ProcessPassthrough(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);
	LRESULT ProcessMessage(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);

	static Window* instance;
	static std::string className;
	
	std::vector<Tickable*> tickables;
	HWND windowHandle;
	HINSTANCE hInstance;
	Gfx* gfx;
	Scene* currentScene;

	int width;
	int height;
};

