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
		inline bool GetWasKeyDown() { return keyDown; }
		inline char GetKey() { return key; }
	private:
		bool keyDown;
		char key;
	};

	inline bool IsKeyDown(unsigned char key);
	inline bool IsKeyDownThisFrame(unsigned char key);
	inline bool IsKeyUp(unsigned char key);
	inline bool IsKeyUpThisFrame(unsigned char key);

	inline bool HasKeyEvent();
	inline KeyEvent PeekKey();
	inline KeyEvent PopKey();

	inline bool HasChar();
	inline unsigned char PeekChar();
	inline unsigned char PopChar();

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

