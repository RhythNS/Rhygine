#include <Scenes/BulletTestScene.h>
#include <ECS/GameObject.h>
#include <ECS/Components/Transform.h>
#include <Factories/TestLight.h>
#include <CC/TestLightComponent.h>
#include <Factories/TestLitPlate.h>
#include <ECS/GameObjectFactory.h>
#include <ECS/Components/Camera.h>
#include <Bullet/RigidBody.h>
#include <Components/TestPhysicsMover.h>
#include <Components/AwakeRigidbodies.h>
#include <Components/TestLitPlateComponent.h>

#include <vector>

void BulletTestScene::InnerInit()
{
	stage->Get3DCamera()->GetTransform()->localPosition.z -= 10.0f;

	TestLight tl;

	TestLightComponent* testLight = GameObjectFactory::Add(stage.get(), &tl)->GetComponent<TestLightComponent>();

	TestLitPlate tlp = TestLitPlate(testLight);
	GameObject* box = GameObjectFactory::Add(stage.get(), &tlp);
	box->GetComponent<TestLitPlateComponent>()->SetLight(testLight);
	box->GetComponent<Transform>()->localPosition = RhyM::Vec3(0.0f, 0.0f, 0.0f);
	RigidBody* boxBody = box->AddComponent<RigidBody>();
	boxBody->Create(1.0f, std::make_unique<btBoxShape>(btVector3(0.5f, 0.5f, 0.5f)), RhyM::Vec3(0.5f, 0.5f, 0.5f));

	GameObject* staticBox = GameObjectFactory::Add(stage.get(), &tlp);
	staticBox->GetComponent<TestLitPlateComponent>()->SetLight(testLight);
	staticBox->GetComponent<Transform>()->localPosition = RhyM::Vec3(0.3f, -5.0f, 0.0f);
	RigidBody* staticBoxBody = staticBox->AddComponent<RigidBody>();
	staticBoxBody->Create(boxBody);
	staticBoxBody->GetBody()->applyImpulse(btVector3(0.0f, 20.0f, 0.0f), btVector3(0.0f, 0.0f, 0.0f));
	staticBox->AddComponent<TestPhysicsMover>();

	GameObject* ground = GameObjectFactory::Add(stage.get(), &tlp);
	ground->GetComponent<TestLitPlateComponent>()->SetLight(testLight);
	ground->GetComponent<Transform>()->localScale = { 20.0f, 1.0f, 20.0f };
	ground->GetComponent<Transform>()->localPosition = { 0.0f, -7.0f, 0.0f };
	RigidBody* groundBody = ground->AddComponent<RigidBody>();
	groundBody->Create(0.0f, std::make_unique<btBoxShape>(btVector3(10.0f, 0.5f, 10.0f)));

	staticBoxBody->GetBody()->setActivationState(WANTS_DEACTIVATION);
	boxBody->GetBody()->setActivationState(WANTS_DEACTIVATION);
	std::vector<RigidBody*> bodies = { boxBody, staticBoxBody };

	ground->AddComponent<AwakeRigidbodies>()->Set(bodies);
}
