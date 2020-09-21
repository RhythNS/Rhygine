#include "Mouse.h"
#include <Windows.h>

inline int Mouse::GetX()
{
	return x;
}

inline int Mouse::GetY()
{
	return y;
}

inline float Mouse::GetScrollDelta()
{
	return prevScroll;
}

inline bool Mouse::IsButtonDown(int button)
{
	return currentButtons[button];
}

inline bool Mouse::IsButtonDownThisFrame(int button)
{
	return currentButtons[button] && !prevButtons[button];
}

inline bool Mouse::IsButtonUp(int button)
{
	return !currentButtons[button];
}

inline bool Mouse::IsButtonUpThisFrame(int button)
{
	return !currentButtons[button] && !prevButtons[button];
}

inline bool Mouse::HasButtonEvent()
{
	return events.size() > 0;
}

inline Mouse::ButtonEvent Mouse::PeekButton()
{
	return events.front();
}

inline Mouse::ButtonEvent Mouse::PopButton()
{
	ButtonEvent buttonEvent = events.front();
	events.pop();
	return buttonEvent;
}

void Mouse::Move(int _x, int _y)
{
	x = _x;
	y = _y;
}

void Mouse::ButtonClicked(int button)
{
	currentButtons[button] = true;
	events.push(ButtonEvent(true, (Button)button));
}

void Mouse::ButtonReleased(int button)
{
	currentButtons[button] = false;
	events.push(ButtonEvent(false, (Button)button));
}

void Mouse::Scroll(int distance)
{
	curScroll += ((float)distance) / WHEEL_DELTA;
}

void Mouse::Tick()
{
	prevButtons = currentButtons;
	prevScroll = curScroll;
	curScroll = 0;
}

void Mouse::ResetCurrentKeys()
{
	for (int i = 0; i < currentButtons.size(); i++)
	{
		if (currentButtons[i]) {
			events.push(ButtonEvent(false, static_cast<Button>(i)));
		}
	}
	currentButtons = 0;
}

void Mouse::ResetQueue()
{
	events = {};
}

void Mouse::ResetAll()
{
	currentButtons = 0;
	events = {};
}
