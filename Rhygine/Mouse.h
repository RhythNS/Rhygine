#pragma once
#include <bitset>
#include <queue>

#include "Tickable.h"
#include <Windows.h>

constexpr auto RH_MOUSE_LEFT = 0;
constexpr auto RH_MOUSE_RIGHT = 1;
constexpr auto RH_MOUSE_MIDDLE = 2;
constexpr auto RH_MOUSE_X1 = 3;
constexpr auto RH_MOUSE_X2 = 4;

class Mouse : public Tickable
{
	friend class Window;
public:
	enum class Button {
		Left = 0x0,
		Right = 0x1,
		Middle = 0x2,
		X1 = 0x3,
		X2 = 0x4
	};
	class ButtonEvent {
	public:
		ButtonEvent(bool buttonDown, Button button) : buttonDown(buttonDown), button(button) {}
		inline bool GetWasButtonDown() { return buttonDown; }
		inline Button GetButton() { return button; }
	private:
		bool buttonDown;
		Button button;
	};

	inline int GetX();
	inline int GetY();
	inline float GetScrollDelta();
	inline bool IsButtonDown(int button);
	inline bool IsButtonDownThisFrame(int button);
	inline bool IsButtonUp(int button);
	inline bool IsButtonUpThisFrame(int button);

	inline bool HasButtonEvent();
	inline ButtonEvent PeekButton();
	inline ButtonEvent PopButton();

private:
	void Move(int x, int y);
	void ButtonClicked(int button);
	void ButtonReleased(int button);
	void Scroll(int distance);

	void Tick();
	void ResetCurrentKeys();
	void ResetQueue();
	void ResetAll();

	int x = 0, y = 0;
	float prevScroll = 0, curScroll = 0;
	std::bitset<5> currentButtons, prevButtons;
	std::queue<ButtonEvent> events;
};

