#pragma once
#include <bitset>
#include <queue>

#include "Tickable.h"
#include "RhyWin.h"
#include "RhyMath.h"

constexpr int RH_MOUSE_LEFT = 0;
constexpr int RH_MOUSE_RIGHT = 1;
constexpr int RH_MOUSE_MIDDLE = 2;
constexpr int RH_MOUSE_X1 = 3;
constexpr int RH_MOUSE_X2 = 4;

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
		bool GetWasButtonDown() { return buttonDown; }
		Button GetButton() { return button; }
	private:
		bool buttonDown;
		Button button;
	};

	RhyM::Vec2I* GetRelativePosition();
	RhyM::Vec2I* GetAbsolutePosition();
	float GetVertScrollDelta();
	float GetHoriScrollDelta();
	bool IsButtonDown(int button);
	bool IsButtonDownThisFrame(int button);
	bool IsButtonUp(int button);
	bool IsButtonUpThisFrame(int button);

	bool HasButtonEvent();
	ButtonEvent PeekButton();
	ButtonEvent PopButton();

private:
	void AbsoluteMove(int x, int y);
	void RelativeMove(int x, int y);
	void ButtonClicked(int button);
	void ButtonReleased(int button);
	void VertScroll(int distance);
	void HoriScroll(int distance);

	void Tick();
	void ResetCurrentKeys();
	void ResetQueue();
	void ResetAll();

	RhyM::Vec2I relativePosCur, relativePosPrev, absolutePos;
	float prevVertScroll = 0, curVertScroll = 0;
	float prevHoriScroll = 0, curHoriScroll = 0;
	std::bitset<5> currentButtons, prevButtons;
	std::queue<ButtonEvent> events;
};

