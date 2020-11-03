#include "BulletTestScene.h"
#include "Window.h"
#include "Time.h"
#include "Gameobject.h"
#include "Transform.h"
#include "TestLight.h"
#include "TestLitPlateComponent.h"
#include "TestLitPlate.h"
#include "GameObjectFactory.h"
#include "Camera.h"

BulletTestScene::BulletTestScene()
{
	collisionConfiguration = new btDefaultCollisionConfiguration();
	dispatcher = new btCollisionDispatcher(collisionConfiguration);

	broadphase = new btDbvtBroadphase();
	constraintSolver = new btSequentialImpulseConstraintSolver();

	world = new btDiscreteDynamicsWorld(dispatcher, broadphase, constraintSolver, collisionConfiguration);
	debugDraw.setDebugMode(btIDebugDraw::DBG_DrawWireframe);
	world->setDebugDrawer(&debugDraw);

	boxMotion = new btDefaultMotionState();
	boxShape = new btBoxShape(btVector3(0.5f, 0.5f, 0.5f));
	boxBody = new btRigidBody(1.0f, boxMotion, boxShape, btVector3(0.5f, 0.5f, 0.5f));

	staticBoxMotion = new btDefaultMotionState();
	staticBoxBody = new btRigidBody(1.0f, staticBoxMotion, boxShape, btVector3(0.5f, 0.5f, 0.5f));

	groundMotion = new btDefaultMotionState();
	groundShape = new btBoxShape(btVector3(10.0f, 0.5f, 10.0f));
	groundBody = new btRigidBody(0.0f, staticBoxMotion, groundShape);
}

BulletTestScene::~BulletTestScene()
{
	delete world;
	delete boxBody;
	delete boxMotion;
	delete boxShape;
	delete groundBody;
	delete groundMotion;
	delete groundShape;
	delete dispatcher;
	delete broadphase;
	delete constraintSolver;
	delete collisionConfiguration;
}

void BulletTestScene::InnerInit()
{
	stage->GetCamera()->GetTransform()->position.m_floats[2] -= 10.0f;
	//body->applyImpulse(btVector3(25.0f, 0.0f, 0.0f), btVector3(0.0f, 10.0f, 0.0f));
	//boxBody->setAngularVelocity(btVector3(2.0f, 2.0f, 2.0f));
	//staticBoxBody->setFlags(btRigidBody::CF_NO_CONTACT_RESPONSE | btRigidBody::CF_STATIC_OBJECT);
	debugDraw.Init(stage.get());

	staticBoxBody->translate(btVector3(0.9f, -5.0f, 0.0f));
	staticBoxBody->applyImpulse(btVector3(0.0f, 20.0f, 0.0f), btVector3(0.0f, 0.0f, 0.0f));

	groundBody->translate(btVector3(0.0, -7.0f, 0.0f));

	world->setGravity({ 0.0f, -9.81f, 0.0f });
	world->addRigidBody(boxBody);
	world->addRigidBody(staticBoxBody);
	world->addRigidBody(groundBody);

	TestLight tl;
	TestLitPlate tlp;
	TestLightComponent* testLight = GameObjectFactory::Add(stage.get(), &tl)->GetComponent<TestLightComponent>();
	box = GameObjectFactory::Add(stage.get(), &tlp);
	box->GetComponent<TestLitPlateComponent>()->SetLight(0, testLight);

	staticBox = GameObjectFactory::Add(stage.get(), &tlp);
	staticBox->GetComponent<TestLitPlateComponent>()->SetLight(0, testLight);

	ground = GameObjectFactory::Add(stage.get(), &tlp);
	ground->GetComponent<TestLitPlateComponent>()->SetLight(0, testLight);
	ground->GetComponent<Transform>()->scale = { 20.0f, 1.0f, 20.0f };
}

void BulletTestScene::InnerPreUpdate()
{
	world->stepSimulation(Window::GetInstance()->time.GetDelta());
	btQuaternion quat = boxBody->getOrientation();
	btVector3 pos = boxBody->getWorldTransform().getOrigin();
	Transform* trans = box->GetComponent<Transform>();

	trans->position = pos;
	trans->rotation = quat;

	quat = staticBoxBody->getOrientation();
	pos = staticBoxBody->getWorldTransform().getOrigin();
	trans = staticBox->GetComponent<Transform>();
	trans->position = pos;
	trans->rotation = quat;

	quat = groundBody->getOrientation();
	pos = groundBody->getWorldTransform().getOrigin();
	trans = ground->GetComponent<Transform>();
	trans->position = pos;
	trans->rotation = quat;

	if (Window::GetInstance()->keys.IsKeyDown('X'))
		staticBoxBody->applyImpulse(btVector3(0.0f, 10.0f, 0.0f), btVector3(0.0f, 0.0f, 0.0f));
	if (Window::GetInstance()->keys.IsKeyDown('Y'))
		staticBoxBody->applyImpulse(btVector3(1.0f, 0.0f, 0.0f), btVector3(0.0f, 0.0f, 0.0f));
	if (Window::GetInstance()->keys.IsKeyDown('C'))
		staticBoxBody->applyImpulse(btVector3(-1.0f, 0.0f, 0.0f), btVector3(0.0f, 0.0f, 0.0f));
	else
		return;
	staticBoxBody->activate(true);
}

void BulletTestScene::InnerAfterDraw()
{
	world->debugDrawWorld();
}
