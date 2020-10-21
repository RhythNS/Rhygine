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

/// <summary>
/// Representation of the mouse. Mouse is used to access the state of the mouse.
/// </summary>
class Mouse : public Tickable
{
	friend class Window;
public:
	/// <summary>
	/// Buttons of the mouse.
	/// </summary>
	enum class Button {
		Left = 0x0,
		Right = 0x1,
		Middle = 0x2,
		X1 = 0x3,
		X2 = 0x4
	};
	/// <summary>
	/// Event for when a mouse button was pressed or released.
	/// </summary>
	class ButtonEvent {
	public:
		ButtonEvent(bool buttonDown, Button button) : buttonDown(buttonDown), button(button) {}
		bool GetWasButtonDown() { return buttonDown; }
		Button GetButton() { return button; }
	private:
		bool buttonDown;
		Button button;
	};

	/// <summary>
	/// Gets the movement of the mouse from last frame to this frame.
	/// </summary>
	/// <returns>A dumb pointer to the vector which has the relative position of the mouse.</returns>
	RhyM::Vec2I* GetRelativePosition();
	/// <summary>
	/// Gets the coordinates of the mouse. 0/0 is the top left corner.
	/// </summary>
	/// <returns>A dumb pointer to the vector which has the absolute position of the mouse.</returns>
	RhyM::Vec2I* GetAbsolutePosition();
	/// <summary>
	/// Gets the delta of the vertical scroll since last frame.
	/// </summary>
	float GetVertScrollDelta();
	/// <summary>
	/// Gets the delta of the horizontal scroll since last frame.
	/// </summary>
	float GetHoriScrollDelta();
	/// <summary>
	/// Checks wheter a specific button of the mouse was pressed.
	/// </summary>
	/// <param name="button">The id of the Button. Use RH_MOUSE_***.</param>
	/// <returns>Wheter the button is pressed.</returns>
	bool IsButtonDown(int button);
	/// <summary>
	/// Checks wheter a specific button of the mouse was pressed this frame.
	/// </summary>
	/// <param name="button">The id of the Button. Use RH_MOUSE_***.</param>
	/// <returns>Wheter the button was just pressed this frame.</returns>
	bool IsButtonDownThisFrame(int button);
	/// <summary>
	/// Checks wheter a specific button of the mouse is not pressed.
	/// </summary>
	/// <param name="button">The id of the Button. Use RH_MOUSE_***.</param>
	/// <returns>Wheter the button is not pressed.</returns>
	bool IsButtonUp(int button);
	/// <summary>
	/// Checks wheter a specific button of the mouse was released this frame.
	/// </summary>
	/// <param name="button">The id of the Button. Use RH_MOUSE_***.</param>
	/// <returns>Wheter the button was just released this frame.</returns>
	bool IsButtonUpThisFrame(int button);

	/// <summary>
	/// Check if the mouse event queue has an event.
	/// </summary>
	/// <returns>True when an event was found.</returns>
	bool HasButtonEvent();
	/// <summary>
	/// Get the first event in the queue without removing it.
	/// </summary>
	/// <returns>The first event in the queue.</returns>
	ButtonEvent PeekButton();
	/// <summary>
	/// Get the first event in the queue and remove it.
	/// </summary>
	/// <returns>The first event in the queue.</returns>
	ButtonEvent PopButton();

private:
	/// <summary>
	/// Update the absolute coordinates of the mouse.
	/// </summary>
	/// <param name="x">The x coordinate of the mouse.</param>
	/// <param name="y">The y coordinate of the mouse.</param>
	void AbsoluteMove(int x, int y);
	/// <summary>
	/// Update the relative coordinates of the mouse.
	/// </summary>
	/// <param name="x">The x coordinate of the mouse.</param>
	/// <param name="y">The y coordinate of the mouse.</param>
	void RelativeMove(int x, int y);
	/// <summary>
	/// Presses a specific button of the mouse.
	/// </summary>
	/// <param name="button">The id of the Button. Use RH_MOUSE_***.</param>
	void ButtonClicked(int button);
	/// <summary>
	/// Releases a specific button of the mouse.
	/// </summary>
	/// <param name="button">The id of the Button. Use RH_MOUSE_***.</param>
	void ButtonReleased(int button);
	/// <summary>
	/// Scroll the mouse wheel in the vertical direction.
	/// </summary>
	/// <param name="distance">The distance of the scroll.</param>
	void VertScroll(int distance);
	/// <summary>
	/// Scroll the mouse wheel in the horizontal direction.
	/// </summary>
	/// <param name="distance">The distance of the scroll.</param>
	void HoriScroll(int distance);

	/// <summary>
	/// Update values for the beginning of the frame.
	/// </summary>
	void Tick();
	/// <summary>
	/// Releases all current pressed keys.
	/// </summary>
	void ResetCurrentKeys();
	/// <summary>
	/// Removes all events from the queues.
	/// </summary>
	void ResetQueue();
	/// <summary>
	/// Resets all current pressed keys and removes all events from the queue.
	/// </summary>
	void ResetAll();

	RhyM::Vec2I relativePosCur, relativePosPrev, absolutePos;
	float prevVertScroll = 0, curVertScroll = 0;
	float prevHoriScroll = 0, curHoriScroll = 0;
	std::bitset<5> currentButtons, prevButtons;
	std::queue<ButtonEvent> events;
};
