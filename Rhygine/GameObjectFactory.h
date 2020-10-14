#pragma once
#include "GameObject.h"
#include "Transform.h"
#include "Drawer.h"

class GameObjectFactory
{
public:
	virtual void AddData(GameObject* toAddTo) = 0;

	static GameObject* Add(std::vector<std::unique_ptr<GameObject>>* gameObjects, GameObjectFactory* factory)
	{
		gameObjects->push_back(std::make_unique<GameObject>());
		GameObject* obj = (*gameObjects)[gameObjects->size() - 1].get();
		factory->AddData(obj);
		obj->Init();
		return obj;
	}

protected:
	virtual Transform* AddTransform(GameObject* toAddTo) { return toAddTo->AddComponent<Transform>(); };
	virtual Drawer* AddDrawer(GameObject* toAddTo) { return toAddTo->AddComponent<Drawer>(); };
};