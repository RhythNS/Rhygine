#pragma once
#include "UIFont.h"
#include "Updatable.h"
#include "VNCharacter.h"

#include <vector>
#include <functional>

class VNTextbox : public UIFont, public Updatable
{
public:
	struct Node {
		std::string text;
		VNCharacter::Emotion emotion;

		Node(std::string text, VNCharacter::Emotion emotion) :
			text(text), emotion(emotion)
		{ }
	};

	void DisplayScene(std::vector<Node> scene, VNCharacter* character, std::function<void()> finishCallback, float defWidth, float timeForCharacter = 0.05f);
	void Update();
	void InnerDraw(SpriteBatch* batch);

	float defWidth;

private:
	std::function<void()> finishCallback;
	std::vector<Node> currentScene;
	int atNode;
	VNCharacter* character;

	int atCharacter;
	float timeForCharacter;
	float timer;

};
