#pragma once
#include "Component.h"
#include "Updatable.h"
#include "Texture.h"
#include "RigidBody.h"
#include "RhyMath.h"

#include <memory>
#include <vector>

class BulletShowcaser : public Component, public Updatable
{
public:
	void Init();
	void Update();

	void DeleteCallback(RigidBody* body);
private:
	RigidBody* CreateBox(RhyM::Vec3 position, RhyM::Vec3 size, float mass);

	std::vector<RigidBody*> bodies;

	float timer = 0;
	float secondsPerBox = 0.5f;
	float explosionFactor = 10.0f;

	RhyM::Vec3 baseSpawnPoint = RhyM::Vec3(15.0f, 0.0f, 0.0f);
	RhyM::Vec3 baseSpawnSize = RhyM::Vec3(5.0f, 5.0f, 0.0f);
	std::unique_ptr<Texture> boxTexture;
};
