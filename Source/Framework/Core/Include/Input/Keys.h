#pragma once
#include <Core/Tickable.h>

#include <bitset>
#include <queue>

/// <summary>
/// Representation of the Keyboard. Keyboard is used to access the state of the keyboard.
/// </summary>
class Keys : public Tickable
{
	friend class Window;
public:
	/// <summary>
	/// Event for when a keyboard key was pressed.
	/// </summary>
	class KeyEvent {
	public:
		KeyEvent(bool keyDown, unsigned char key) : keyDown(keyDown), key(key) {}
		bool GetWasKeyDown() { return keyDown; }
		char GetKey() { return key; }
	private:
		bool keyDown;
		char key;
	};

	/// <summary>
	/// Checks wheter a specific key was pressed.
	/// </summary>
	/// <param name="key">The id of the key. Uses the win32 keycodes.</param>
	/// <returns>Wheter the key is pressed.</returns>
	bool IsKeyDown(unsigned char key);
	/// <summary>
	/// Checks wheter a specific key was just pressed.
	/// </summary>
	/// <param name="key">The id of the key. Uses the win32 keycodes.</param>
	/// <returns>Wheter the key was pressed this frame.</returns>
	bool IsKeyDownThisFrame(unsigned char key);
	/// <summary>
	/// Checks wheter a specific key is not pressed.
	/// </summary>
	/// <param name="key">The id of the key. Uses the win32 keycodes.</param>
	/// <returns>Wheter the key is not pressed.</returns>
	bool IsKeyUp(unsigned char key);
	/// <summary>
	/// Checks wheter a specific key was released this frame.
	/// </summary>
	/// <param name="key">The id of the key. Uses the win32 keycodes.</param>
	/// <returns>Wheter the key was just released.</returns>
	bool IsKeyUpThisFrame(unsigned char key);

	/// <summary>
	/// Check if the keyboard queue has an event.
	/// </summary>
	/// <returns>True when an event was found.</returns>
	bool HasKeyEvent();
	/// <summary>
	/// Get the first event in the queue without removing it.
	/// </summary>
	/// <returns>The first event in the queue.</returns>
	KeyEvent PeekKey();
	/// <summary>
	/// Get the first event in the queue and remove it.
	/// </summary>
	/// <returns>The first event in the queue.</returns>
	KeyEvent PopKey();

	/// <summary>
	/// Check if the keyboard char queue has an event.
	/// </summary>
	/// <returns>True when an char was found.</returns>
	bool HasChar();
	/// <summary>
	/// Get the first char in the char queue without removing it.
	/// </summary>
	/// <returns>The first char in the queue.</returns>
	unsigned char PeekChar();
	/// <summary>
	/// Get the first char in the char queue and remove it.
	/// </summary>
	/// <returns>The first char in the queue.</returns>
	unsigned char PopChar();

private:
	/// <summary>
	/// Presses a keyboard key.
	/// </summary>
	/// <param name="key">The id of the key. Uses the win32 keycodes.</param>
	void PressKey(unsigned char key);
	/// <summary>
	/// Releases a keyboard key.
	/// </summary>
	/// <param name="key">The id of the key. Uses the win32 keycodes.</param>
	void ReleaseKey(unsigned char key);
	
	/// <summary>
	/// Adds a typed char to the char queue.
	/// </summary>
	/// <param name="key">The id of the key. Uses the win32 keycodes.</param>
	void CharTyped(unsigned char key);

	/// <summary>
	/// Updates the values for the beginning of the frame.
	/// </summary>
	void Tick();
	/// <summary>
	/// Update values for the end of the frame.
	/// </summary>
	void EndTick();
	/// <summary>
	/// Resets all current pressed keys.
	/// </summary>
	void ResetCurrentKeys();
	/// <summary>
	/// Resets all queues.
	/// </summary>
	void ResetQueues();
	/// <summary>
	/// Resets the pressed keys and all queues.
	/// </summary>
	void ResetAll();

	std::bitset<0xFF> currentKeys, prevKeys;
	std::queue<KeyEvent> events;
	std::queue<unsigned char> chars;
};

