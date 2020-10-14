#pragma once
#include "GameObjectFactory.h"
#include "Texture.h"
#include "RhyWin.h"

#include <vector>
#include <memory>

class TestTexture : public GameObjectFactory
{
public:
	void AddData(GameObject* toAddTo);
};
