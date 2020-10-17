#pragma once

class GameObject;
class Keys;
class Mouse;

class Component
{
	friend class GameObject;
public:
	virtual void Init() = 0;
	GameObject* GetGameObject();

protected:
	float GetDelta();
	Mouse* GetMouse();
	Keys* GetKeys();
private:
	void SetGameObject(GameObject* gameObject);

	GameObject* gameObject;
};
