#pragma once
#include "Component.h"
#include "MonoFont.h"

/// <summary>
/// Holds all needed textures needed for the present scene.
/// </summary>
class PresentResources : public Component
{
public:
	void Init();

	~PresentResources();

	static PresentResources* instance;

	std::unique_ptr<Texture> exclamationMark;
	std::unique_ptr<Texture> textBoxBackgroundTexture;
	std::unique_ptr<Texture> testTex;

	std::unique_ptr<Texture> happy, mad, sad, surprised, embarrassed;

	MonoFont defFont{ "PresentScene\\SourceCodePro_Regular_(72)_[0,0,0,0]_45x76.png", 45, 76 };
};
