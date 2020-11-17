#pragma once
#include "GameObjectFactory.h"
#include "Texture.h"
#include "RhyWin.h"

#include <vector>
#include <memory>

/// <summary>
/// Factory for building a plane with a test texture on it.
/// </summary>
class TestTexture : public GameObjectFactory
{
public:
	void AddData(GameObject* toAddTo);
};
