#include <Present/PresentResources.h>

void PresentResources::Init()
{
    instance = this;

    exclamationMark = std::make_unique<Texture>("Assets\\PresentScene\\exclamationmark.png", 0);
    textBoxBackgroundTexture = std::make_unique<Texture>("Assets\\PresentScene\\textBox.png", 0);

	happy = std::make_unique<Texture>("Assets\\PresentScene\\happy.png", 0);
	mad = std::make_unique<Texture>("Assets\\PresentScene\\mad.png", 0);
	sad = std::make_unique<Texture>("Assets\\PresentScene\\sad.png", 0);
	surprised = std::make_unique<Texture>("Assets\\PresentScene\\surprised.png", 0);
	embarrassed = std::make_unique<Texture>("Assets\\PresentScene\\embarrassed.png", 0);
}

PresentResources::~PresentResources()
{
	instance = nullptr;
}

PresentResources* PresentResources::instance;
