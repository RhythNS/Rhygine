#pragma once
#include <ECS/Component.h>
#include <ECS/Updatable.h>
#include <ECS/Bindable/Texture.h>
#include <React/RigidBody.h>
#include <RhyMath.h>

#include <memory>

/// <summary>
/// Component for showing of bullet3.
/// </summary>
class ReactShowcaser : public Component, public Updatable
{
public:
	void Init();
	void Update();
private:
	RhyReact::RigidBody* CreateBox(RhyM::Vec3 position, RhyM::Vec3 size, float mass);

	float timer = 0;
	float secondsPerBox = 0.5f;

	RhyM::Vec3 baseSpawnPoint = RhyM::Vec3(15.0f, 0.0f, 0.0f);
	RhyM::Vec3 baseSpawnSize = RhyM::Vec3(5.0f, 5.0f, 0.0f);
	std::unique_ptr<Texture> boxTexture;
};
