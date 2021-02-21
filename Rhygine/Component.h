#pragma once

class GameObject;
class Keys;
class Mouse;
class SpriteBatch;
class ParallelUpdatable;

/// <summary>
/// Components can implement updatable and/ or drawable for functionality.
/// Components add functionalty to the gameobjects on which they are on.
/// </summary>
class Component
{
	friend class GameObject;
public:
	// Destructor so inherited somponents can have a destructor.
	virtual ~Component() {}

	/// <summary>
	/// Called after the the reference to the gameobject was set.
	/// Used for initializing the component.
	/// </summary>
	virtual void Init() {}
	/// <summary>
	/// Gets a reference to the gameobject.
	/// </summary>
	GameObject* GetGameObject();

	/// <summary>
	/// Returns wheter it is enabled or not.
	/// </summary>
	bool IsEnabled();

	/// <summary>
	/// Enables this component.
	/// </summary>
	void Enable();

	/// <summary>
	/// Disables this component.
	/// </summary>
	void Disable();

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
	/// Get a pointer to the keys instance.
	/// </summary>
	Keys* GetKeys();
	/// <summary>
	/// Get a pointer to the spritebatch.
	/// </summary>
	SpriteBatch* GetSpriteBatch();
	/// <summary>
	/// Adds a OneShot to the TaskManager.
	/// </summary>
	/// <param name="parallel">The Parallel to be added.</param>
	void AddOneShot(ParallelUpdatable* parallel);

	/// <summary>
	/// Calles when a component that was disabled was enabled.
	/// </summary>
	virtual void OnEnabled() {}
	/// <summary>
	/// Calles when a component that was enabled was disabled.
	/// </summary>
	virtual void OnDisabled() {}

	/// <summary>
	/// Called when a component is about to be destroyed.
	/// </summary>
	virtual void OnRemove() {}
private:
	/// <summary>
	/// Sets the gameobject on which the component is.
	/// </summary>
	void SetGameObject(GameObject* gameObject);

	GameObject* gameObject = nullptr;

	/// <summary>
	/// Recieves updates and draw calls when enabled.
	/// </summary>
	bool enabled = true;
};
