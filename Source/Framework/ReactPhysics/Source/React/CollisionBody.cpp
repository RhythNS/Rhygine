#include <React/CollisionBody.h>
#include <React/ReactConverter.h>
#include <reactphysics3d/engine/PhysicsWorld.h>
#include <ECS/GameObject.h>
#include <ECS/Components/Transform.h>	

using namespace RhyReact;

CollisionBody::~CollisionBody()
{
	GetWorld()->destroyCollisionBody(collisionBody);
}

void CollisionBody::Init()
{
	transform = GetGameObject()->GetComponent<Transform>();
	assert(transform);
	collisionBody = GetWorld()->createCollisionBody(TransformRhyToReact(transform->GetWorldPosition(), transform->GetWorldRotation()));
	collisionBody->setUserData(this);
}

void CollisionBody::OnEnabled()
{
	collisionBody->setIsActive(true);
}

void CollisionBody::OnDisabled()
{
	collisionBody->setIsActive(false);
}
