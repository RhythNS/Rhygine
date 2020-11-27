#pragma once
#include "Component.h"
#include "RhyMath.h"
#include "Texture.h"

#include <memory>

/// <summary>
/// Tests to see the frame time of basic boxes.
/// </summary>
class OptBasic : public Component
{
public:
	void Init();
private:
	void CreateBox(RhyM::Vec3 pos);
	std::unique_ptr<Texture> tex;
};
