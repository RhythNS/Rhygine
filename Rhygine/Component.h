#pragma once

class GameObject;
class Keys;
class Mouse;

/// <summary>
/// Components can implement updatable and/ or drawable for functionality.
/// Components add functionalty to the gameobjects on which they are on.
/// </summary>
class Component
{
	friend class GameObject;
public:
	/// <summary>
	/// Called after the the reference to the gameobject was set.
	/// Used for initializing the component.
	/// </summary>
	virtual void Init() = 0;
	/// <summary>
	/// Gets a reference to the gameobject.
	/// </summary>
	GameObject* GetGameObject();

protected:
	/// <summary>
	/// Get the delta time since last frame.
	/// </summary>
	float GetDelta();
	/// <summary>
	/// Get a pointer to the mouse instance.
	/// </summary>
	Mouse* GetMouse();
	/// <summary>
	/// Get a popinter to the keys instance.
	/// </summary>
	/// <returns></returns>
	Keys* GetKeys();
private:
	void SetGameObject(GameObject* gameObject);

	GameObject* gameObject;
};
