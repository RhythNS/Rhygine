#include "BulletTestScene.h"
#include "Window.h"
#include "Time.h"
#include "Gameobject.h"
#include "Transform.h"
#include "TestLight.h"
#include "TestLitPlateComponent.h"
#include "TestLitPlate.h"
#include "GameObjectFactory.h"


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
	boxBody = new btRigidBody(1.0f, boxMotion, boxShape);

	staticBoxMotion = new btDefaultMotionState();
	staticBoxBody = new btRigidBody(0.0f, staticBoxMotion, boxShape);
}

BulletTestScene::~BulletTestScene()
{
	delete world;
	delete boxBody;
	delete boxMotion;
	delete boxShape;
	delete dispatcher;
	delete broadphase;
	delete constraintSolver;
	delete collisionConfiguration;
}

void BulletTestScene::InnerInit()
{
	//body->applyImpulse(btVector3(25.0f, 0.0f, 0.0f), btVector3(0.0f, 10.0f, 0.0f));
	//boxBody->setAngularVelocity(btVector3(2.0f, 2.0f, 2.0f));
	//staticBoxBody->setFlags(btRigidBody::CF_NO_CONTACT_RESPONSE | btRigidBody::CF_STATIC_OBJECT);
	debugDraw.Init(stage.get());

	staticBoxBody->translate(btVector3(0.9f, -2.0f, 0.0f));

	world->setGravity({ 0.0f, -9.81f, 0.0f });
	world->addRigidBody(boxBody);
	world->addRigidBody(staticBoxBody);

	TestLight tl;
	TestLitPlate tlp;
	TestLightComponent* testLight = GameObjectFactory::Add(stage.get(), &tl)->GetComponent<TestLightComponent>();
	box = GameObjectFactory::Add(stage.get(), &tlp);
	box->GetComponent<TestLitPlateComponent>()->SetLight(0, testLight);

	staticBox = GameObjectFactory::Add(stage.get(), &tlp);
	staticBox->GetComponent<TestLitPlateComponent>()->SetLight(0, testLight);
}

void BulletTestScene::InnerPreUpdate()
{
	world->stepSimulation(Window::GetInstance()->time.GetDelta());
	btQuaternion quat = boxBody->getOrientation();
	btVector3 pos = boxBody->getWorldTransform().getOrigin();
	Transform* trans = box->GetComponent<Transform>();

	trans->position.Set(pos.m_floats[0], pos.m_floats[1], pos.m_floats[2]);
	trans->rotation.x = quat.getX();
	trans->rotation.y = quat.getY();
	trans->rotation.z = quat.getZ();
	trans->rotation.w = quat.getW();

	quat = staticBoxBody->getOrientation();
	pos = staticBoxBody->getWorldTransform().getOrigin();
	trans = staticBox->GetComponent<Transform>();
	trans->position.Set(pos.m_floats[0], pos.m_floats[1], pos.m_floats[2]);
	trans->rotation.x = quat.getX();
	trans->rotation.y = quat.getY();
	trans->rotation.z = quat.getZ();
	trans->rotation.w = quat.getW();
}

void BulletTestScene::InnerAfterDraw()
{
	world->debugDrawWorld();
}
