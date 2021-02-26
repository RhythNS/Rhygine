#pragma once
#include "Component.h"
#include "Drawable.h"
#include "Updatable.h"
#include "IndexBuffer.h"
#include "VertBuffer.h"

#include <vector>
#include <memory>
#include <DirectXMath.h>

class Bindable;
class UnBindable;
class Transform;
class Camera;
class DrawerUpdatable;

/// <summary>
/// The drawer holds bindables which are used for drawing something onto the screen.
/// </summary>
class Drawer : public Component, public Drawable
{
public:
	enum class DrawMode
	{
		List, Indexed
	};

	void Init();
	void Draw();

	/// <summary>
	/// Adds a bindable to the drawer.
	/// </summary>
	/// <param name="bindable">The bindable to be added.</param>
	/// <returns>A pointer to the added bindable.</returns>
	Bindable* AddBindable(std::unique_ptr<Bindable> bindable);
	/// <summary>
	/// Removes a bindable.
	/// </summary>
	/// <param name="bindable">The bindable that should be removed.</param>
	/// <returns>Wheter the bindable was removed or not.</returns>
	bool RemoveBindable(Bindable* bindable);

	/// <summary>
	/// Gets the first bindable with a given type.
	/// </summary>
	/// <typeparam name="T">The type of bindable.</typeparam>
	/// <returns>A reference to the bindable or nullptr if it was not found.</returns>
	template <class T>
	T* GetBindable()
	{
		T* t;
		for (auto& bindable : bindables)
		{
			if (t = dynamic_cast<T*>(bindable.get()))
				return t;
		}
		return nullptr;
	}

	/// <summary>
	/// Gets a reference to the transform.
	/// </summary>
	Transform* GetTransform();
	/// <summary>
	/// Gets a reference to the camera.
	/// </summary>
	Camera* Get3DCamera();
	/// <summary>
	/// What draw function should be called inside Gfx when drawn.
	/// </summary>
	DrawMode drawMode = DrawMode::Indexed;
private:
	/// <summary>
	/// Checks the bindable and sets needed references or changes modes
	/// of the drawer.
	/// </summary>
	/// <param name="bindable">The bindable that was just added.</param>
	inline void AnalyseBindable(Bindable* bindable);

	std::vector<std::unique_ptr<Bindable>> bindables;
	std::vector<Updatable*> updatables;
	std::vector<UnBindable*> unBindables;
	std::vector<DrawerUpdatable*> drawerUpdatables;
	Transform* transform = nullptr;
	IndexBufferAmount* indexAmount = nullptr;
	VertBufferAmount* vertAmount = nullptr;
};
