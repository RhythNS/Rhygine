#include "VNTextbox.h"
#include "Mouse.h"

void VNTextbox::DisplayScene(std::vector<Node> _scene, VNCharacter* _character, std::function<void()> _finishCallback, float _defWidth, float _timeForCharacter)
{
	currentScene = _scene;
	timeForCharacter = _timeForCharacter;
	character = _character;
	finishCallback = _finishCallback;
	defWidth = _defWidth;
	atNode = 0;
}

void VNTextbox::Update()
{
	if (!character)
		return;

	if (GetMouse()->IsButtonDown(RH_MOUSE_LEFT))
	{
		int a = 2;
		a -= 1;
	}

	// was mouse clicked
	if (GetMouse()->IsButtonDownThisFrame(RH_MOUSE_LEFT))
	{
		// Is node finished
		if (atCharacter == currentScene[atNode].text.size() - 1)
		{
			++atNode;
			// is scene finished
			if (atNode == currentScene.size())
			{
				finishCallback();
				return;
			}
			// prepare next node
			atCharacter = 0;
			character->currentEmotion = currentScene[atNode].emotion;
			timer = 0;
			text = "";
		}
		// skip node
		else
		{
			atCharacter = currentScene[atNode].text.size() - 1;
			text = currentScene[atNode].text;
		}
		return;
	}

	// is already fully displayed?
	if (atCharacter == currentScene[atNode].text.size() - 1)
		return;

	timer -= GetDelta();
	if (timer < 0)
	{
		++atCharacter;
		text = currentScene[atNode].text.substr(0, atCharacter + 1);
		timer = timeForCharacter;
	}
}

void VNTextbox::InnerDraw(SpriteBatch* batch)
{
	textScale = bounds.width / defWidth;
	UIFont::InnerDraw(batch);
}
