#pragma once
#include "Scene.h"
#include "BulletDebugDraw.h"

#include "bullet\btBulletDynamicsCommon.h"

class BulletTestScene : public Scene
{
public:
	BulletTestScene();
	~BulletTestScene();
	void InnerInit();
	void InnerPreUpdate();
//	void InnerAfterUpdate() {}
//	void InnerPreDraw();
	void InnerAfterDraw();
private:
	btDiscreteDynamicsWorld* world;
	btCollisionDispatcher* dispatcher;
	btDbvtBroadphase* broadphase;
	btDefaultCollisionConfiguration* collisionConfiguration;
	btSequentialImpulseConstraintSolver* constraintSolver;

	btRigidBody* boxBody;
	btDefaultMotionState* boxMotion;
	btBoxShape* boxShape;
	GameObject* box;

	btRigidBody* staticBoxBody;
	btDefaultMotionState* staticBoxMotion;
	GameObject* staticBox;

	btRigidBody* groundBody;
	btDefaultMotionState* groundMotion;
	btBoxShape* groundShape;
	GameObject* ground;
	
	BulletDebugDraw debugDraw;
};
