#pragma once
#include <Windows.h>
#include <string>
#include <vector>

#include "Keys.h"
#include "Time.h"
#include "Mouse.h"
#include "Gfx.h"

class Window
{
public:
	Window() = delete;
	Window(HINSTANCE hInstance, LPSTR lpCmdLine, int nCmdShow);
	~Window();
	
 	static Window* GetInstance();
	inline HWND GetWindowHandle();
	inline HINSTANCE GetHInstance();
	
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
};

