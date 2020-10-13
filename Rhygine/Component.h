#pragma once

class GameObject;

class Component
{
	friend class GameObject;
public:
	virtual void Init() = 0;
	GameObject* GetGameObject();
private:
	void SetGameObject(GameObject* gameObject);

	GameObject* gameObject;
};
