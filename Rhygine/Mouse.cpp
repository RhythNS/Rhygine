#include "Mouse.h"
#include "RhyWin.h"

RhyM::Vec2I* Mouse::GetRelativePosition()
{
	return &relativePosPrev;
}

RhyM::Vec2I* Mouse::GetAbsolutePosition()
{
	return &absolutePos;
}

float Mouse::GetVertScrollDelta()
{
	return prevVertScroll;
}

float Mouse::GetHoriScrollDelta()
{
	return prevHoriScroll;
}

bool Mouse::IsButtonDown(int button)
{
	return currentButtons[button];
}

bool Mouse::IsButtonDownThisFrame(int button)
{
	return currentButtons[button] && !prevButtons[button];
}

bool Mouse::IsButtonUp(int button)
{
	return !currentButtons[button];
}

bool Mouse::IsButtonUpThisFrame(int button)
{
	return !currentButtons[button] && !prevButtons[button];
}

bool Mouse::HasButtonEvent()
{
	return events.size() > 0;
}

Mouse::ButtonEvent Mouse::PeekButton()
{
	return events.front();
}

Mouse::ButtonEvent Mouse::PopButton()
{
	ButtonEvent buttonEvent = events.front();
	events.pop();
	return buttonEvent;
}

void Mouse::AbsoluteMove(int x, int y)
{
	absolutePos.x = x;
	absolutePos.y = y;
}

void Mouse::RelativeMove(int x, int y)
{
	relativePosCur.x += x;
	relativePosCur.y += y;
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

void Mouse::VertScroll(int distance)
{
	curVertScroll += ((float)distance) / WHEEL_DELTA;
}

void Mouse::HoriScroll(int distance)
{
	curHoriScroll += ((float)distance) / WHEEL_DELTA;
}

void Mouse::Tick()
{
	relativePosPrev = relativePosCur;
	relativePosCur.x = 0;
	relativePosCur.y = 0;

	prevVertScroll = curVertScroll;
	prevHoriScroll = curHoriScroll;
	curVertScroll = 0;
	curHoriScroll = 0;
}

void Mouse::EndTick()
{
	prevButtons = currentButtons;
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
