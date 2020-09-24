#include "Keys.h"

bool Keys::IsKeyDown(unsigned char key)
{
	return currentKeys[key];
}

bool Keys::IsKeyDownThisFrame(unsigned char key)
{
	return currentKeys[key] && !prevKeys[key];
}

bool Keys::IsKeyUp(unsigned char key)
{
	return !currentKeys[key];
}

bool Keys::IsKeyUpThisFrame(unsigned char key)
{
	return !currentKeys[key] && prevKeys[key];
}

bool Keys::HasKeyEvent()
{
	return events.size() > 0;
}

Keys::KeyEvent Keys::PeekKey()
{
	return events.front();
}

Keys::KeyEvent Keys::PopKey()
{
	KeyEvent front = events.front();
	events.pop();
	return front;
}

bool Keys::HasChar()
{
	return chars.size() > 0;
}

unsigned char Keys::PeekChar()
{
	return chars.front();
}

unsigned char Keys::PopChar()
{
	unsigned char frontChar = chars.front();
	chars.pop();
	return frontChar;
}

void Keys::PressKey(unsigned char key)
{
	currentKeys[key] = true;
	events.push(KeyEvent(true, key));
}

void Keys::ReleaseKey(unsigned char key)
{
	currentKeys[key] = false;
	events.push(KeyEvent(false, key));
}

void Keys::CharTyped(unsigned char key)
{
	chars.push(key);
}

void Keys::Tick()
{
	prevKeys = currentKeys;
}

void Keys::ResetCurrentKeys()
{
	for (unsigned char i = 0; i < currentKeys.size(); i++)
	{
		if (currentKeys[i]) {
			events.push(KeyEvent(false, i));
		}
	}
	currentKeys = 0;
}

void Keys::ResetQueues()
{
	events = {};
	chars = {};
}

void Keys::ResetAll()
{
	currentKeys = 0;
	events = {};
	chars = {};
}
