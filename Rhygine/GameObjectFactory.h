#pragma once
#include "GameObject.h"
#include "Stage.h"
#include "Transform.h"
#include "Drawer.h"

class GameObjectFactory
{
public:
	virtual void AddData(GameObject* toAddTo) = 0;

	static GameObject* Add(Stage* stage, GameObjectFactory* factory)
	{
		GameObject* obj = stage->CreateGameObject();
		factory->AddData(obj);
		return obj;
	}

protected:
	virtual Transform* AddTransform(GameObject* toAddTo) { return toAddTo->AddComponent<Transform>(); };
	virtual Drawer* AddDrawer(GameObject* toAddTo) { return toAddTo->AddComponent<Drawer>(); };
};
