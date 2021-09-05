#pragma once
#include <UI/Elements/UIFont.h>
#include <ECS/Updatable.h>
#include <Present/VNCharacter.h>

#include <vector>
#include <functional>

/// <summary>
/// Inherits from UIFont to display a given scene.
/// The scene advances when the user clicks the left mouse button.
/// </summary>
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

	/// <summary>
	/// Displays a scene.
	/// </summary>
	/// <param name="scene">The scene to be displayed.</param>
	/// <param name="character">The character of the scene.</param>
	/// <param name="finishCallback">The function that is called, when the scene is over.</param>
	/// <param name="defWidth">The width of the prefered with so the font can be scaled.</param>
	/// <param name="timeForCharacter">The amount of time a character takes to be displayed.</param>
	void DisplayScene(std::vector<Node> scene, VNCharacter* character, std::function<void()> finishCallback, float defWidth, float timeForCharacter = 0.05f);
	void Update();
	void InnerDraw(SpriteBatch* batch);

	float defWidth = 100.0f;

private:
	/// <summary>
	/// Callback for when the scene is finished.
	/// </summary>
	std::function<void()> finishCallback;
	std::vector<Node> currentScene;
	int atNode = 0;
	VNCharacter* character = nullptr;

	int atCharacter = 0;
	float timeForCharacter = 0.2f;
	float timer = 0;

};
