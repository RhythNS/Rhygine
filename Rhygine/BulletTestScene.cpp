#include "BulletTestScene.h"
#include "Gameobject.h"
#include "Transform.h"
#include "TestLight.h"
#include "TestLitPlateComponent.h"
#include "TestLitPlate.h"
#include "GameObjectFactory.h"
#include "Camera.h"
#include "RigidBody.h"
#include "TestPhysicsMover.h"

void BulletTestScene::InnerInit()
{
	stage->GetCamera()->GetTransform()->position.m_floats[2] -= 10.0f;

	TestLight tl;
	TestLitPlate tlp;

	TestLightComponent* testLight = GameObjectFactory::Add(stage.get(), &tl)->GetComponent<TestLightComponent>();

	GameObject* box = GameObjectFactory::Add(stage.get(), &tlp);
	box->GetComponent<TestLitPlateComponent>()->SetLight(0, testLight);
	box->GetComponent<Transform>()->position = btVector3(0.0f, 0.0f, 0.0f);
	RigidBody* boxBody = box->AddComponent<RigidBody>();
	boxBody->Create(1.0f, std::make_unique<btBoxShape>(btVector3(0.5f, 0.5f, 0.5f)), btVector3(0.5f, 0.5f, 0.5f));

	GameObject* staticBox = GameObjectFactory::Add(stage.get(), &tlp);
	staticBox->GetComponent<TestLitPlateComponent>()->SetLight(0, testLight);
	staticBox->GetComponent<Transform>()->position = btVector3(0.9f, -5.0f, 0.0f);
	RigidBody* staticBoxBody = staticBox->AddComponent<RigidBody>();
	staticBoxBody->Create(boxBody);
	staticBoxBody->GetBody()->applyImpulse(btVector3(0.0f, 20.0f, 0.0f), btVector3(0.0f, 0.0f, 0.0f));
	staticBox->AddComponent<TestPhysicsMover>();

	GameObject* ground = GameObjectFactory::Add(stage.get(), &tlp);
	ground->GetComponent<TestLitPlateComponent>()->SetLight(0, testLight);
	ground->GetComponent<Transform>()->scale = { 20.0f, 1.0f, 20.0f };
	ground->GetComponent<Transform>()->position = { 0.0f, -7.0f, 0.0f };
	RigidBody* groundBody = ground->AddComponent<RigidBody>();
	groundBody->Create(0.0f, std::make_unique<btBoxShape>(btVector3(10.0f, 0.5f, 10.0f)));
}
