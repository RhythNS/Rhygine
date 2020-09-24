#pragma once
#include <bitset>
#include <queue>

#include "Tickable.h"

class Keys : public Tickable
{
	friend class Window;
public:
	class KeyEvent {
	public:
		KeyEvent(bool keyDown, unsigned char key) : keyDown(keyDown), key(key) {}
		bool GetWasKeyDown() { return keyDown; }
		char GetKey() { return key; }
	private:
		bool keyDown;
		char key;
	};

	bool IsKeyDown(unsigned char key);
	bool IsKeyDownThisFrame(unsigned char key);
	bool IsKeyUp(unsigned char key);
	bool IsKeyUpThisFrame(unsigned char key);

	bool HasKeyEvent();
	KeyEvent PeekKey();
	KeyEvent PopKey();

	bool HasChar();
	unsigned char PeekChar();
	unsigned char PopChar();

private:
	void PressKey(unsigned char key);
	void ReleaseKey(unsigned char key);

	void CharTyped(unsigned char key);

	void Tick();
	void ResetCurrentKeys();
	void ResetQueues();
	void ResetAll();

	std::bitset<0xFF> currentKeys, prevKeys;
	std::queue<KeyEvent> events;
	std::queue<unsigned char> chars;
};

