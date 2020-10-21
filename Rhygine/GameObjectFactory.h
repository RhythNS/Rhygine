#pragma once
#include "GameObject.h"
#include "Stage.h"
#include "Transform.h"
#include "Drawer.h"

/// <summary>
/// Helper class for constructing GameObjects.
/// </summary>
class GameObjectFactory
{
public:
	/// <summary>
	/// Adds components to the gameobjects and configures them.
	/// </summary>
	/// <param name="toAddTo"></param>
	virtual void AddData(GameObject* toAddTo) = 0;

	/// <summary>
	/// Create a gameobject and add data to it.
	/// </summary>
	/// <param name="stage"></param>
	/// <param name="factory">The factory that constructs the gameobject.</param>
	/// <returns>A reference to the gameobject.</returns>
	static GameObject* Add(Stage* stage, GameObjectFactory* factory)
	{
		GameObject* obj = stage->CreateGameObject();
		factory->AddData(obj);
		return obj;
	}

protected:
	/// <summary>
	/// Adds a transform to a gameobject.
	/// </summary>
	/// <param name="toAddTo">The gameobject the transform is added to.</param>
	/// <returns>A reference to the transform.</returns>
	virtual Transform* AddTransform(GameObject* toAddTo) { return toAddTo->AddComponent<Transform>(); };
	/// <summary>
	/// Adds a drawer to a gameobject.
	/// </summary>
	/// <param name="toAddTo">The gameobject the drawer is added to.</param>
	/// <returns>A reference to the drawer.</returns>
	virtual Drawer* AddDrawer(GameObject* toAddTo) { return toAddTo->AddComponent<Drawer>(); };
};
